#include <iostream>
#include <string>
#include <queue>
#include <unordered_set>
#include <cstdio>
#include <cstdlib>
#include <curl/curl.h>
#include <stdexcept>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "rapidjson/error/error.h"
#include <atomic>
// #include "rapidjson/reader.h"
#define MAX_THREADS 8
using namespace std;
using namespace rapidjson;

struct ParseException : std::runtime_error, rapidjson::ParseResult {
    ParseException(rapidjson::ParseErrorCode code, const char* msg, size_t offset) : 
        std::runtime_error(msg), 
        rapidjson::ParseResult(code, offset) {}
};

#define RAPIDJSON_PARSE_ERROR_NORETURN(code, offset) \
    throw ParseException(code, #code, offset)

#include "rapidjson/document.h"
#include <chrono>

class BlockingQueue {
    queue<pair<string, int>> q;
    mutex m;
    condition_variable cv;
    bool ready = false;

public:
    void add(pair<string, int> lq) {
        lock_guard<mutex> lock(m);
        q.push(lq);
        cv.notify_one();
    }

    pair<string, int> remove() {
        unique_lock<mutex> lock(m);
        cv.wait(lock, [this] {return !q.empty() || ready;});
        if (!q.empty()) {
            pair<string, int> item = q.front();
            q.pop();
            return item;
        }
        return {"", -1};
    }

    bool isEmpty() {
        lock_guard<mutex> lock(m);
        return q.empty();
    }

    void finish() {
        lock_guard<mutex> lock(m);
        ready = true;
        cv.notify_all();
    }

};


bool debug = false;

// Updated service URL
const string SERVICE_URL = "http://hollywood-graph-crawler.bridgesuncc.org/neighbors/";

// Function to HTTP ecnode parts of URLs. for instance, replace spaces with '%20' for URLs
string url_encode(CURL* curl, string input) {
  char* out = curl_easy_escape(curl, input.c_str(), input.size());
  string s = out;
  curl_free(out);
  return s;
}

// Callback function for writing response data
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

// Function to fetch neighbors using libcurl with debugging
string fetch_neighbors(CURL* curl, const string& node) {

    string url = SERVICE_URL + url_encode(curl, node);
    string response;

    if (debug)
      cout << "Sending request to: " << url << endl;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // Verbose Logging

    // Set a User-Agent header to avoid potential blocking by the server
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "User-Agent: C++-Client/1.0");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        cerr << "CURL error: " << curl_easy_strerror(res) << endl;
    } else {
      if (debug)
        cout << "CURL request successful!" << endl;
    }

    // Cleanup
    curl_slist_free_all(headers);

    if (debug) 
      cout << "Response received: " << response << endl;  // Debug log

    return (res == CURLE_OK) ? response : "{}";
}

// Function to parse JSON and extract neighbors
vector<string> get_neighbors(const string& json_str) {
    vector<string> neighbors;
    try {
      Document doc;
      doc.Parse(json_str.c_str());
      
      if (doc.HasMember("neighbors") && doc["neighbors"].IsArray()) {
        for (const auto& neighbor : doc["neighbors"].GetArray())
	  neighbors.push_back(neighbor.GetString());
      }
    } catch (const ParseException& e) {
      std::cerr<<"Error while parsing JSON: "<<json_str<<std::endl;
      throw e;
    }
    return neighbors;
}

// BFS Traversal Function
vector<string> bfs(const string& start, int depth) {
    BlockingQueue bq;
    unordered_set<string> visited;
    vector<string> result;
    vector<thread> threadgroup;
    mutex result_mutex;
    mutex visited_mutex;

    atomic<int> active_tasks(1);

    bq.add({start, 0});
    visited.insert(start);

    int num_threads = MAX_THREADS;

    for (int i = 0; i < num_threads; i++) {
        threadgroup.push_back(thread([&]() {
            CURL * local_curl = curl_easy_init();
            while (!bq.isEmpty()) {
                auto [node, level] = bq.remove();

                if (level <= depth) {
                    lock_guard<mutex> lock(result_mutex);
                    result.push_back(node);
                }
                
                if (level < depth) {
                    try {
                        for (const auto& neighbor : get_neighbors(fetch_neighbors(local_curl, node))) {
                            lock_guard<mutex> lock(visited_mutex);
                            if (!visited.count(neighbor)) {
                                visited.insert(neighbor);
                                active_tasks++;
                                bq.add({neighbor, level + 1});
                            }
                        }
                    } catch (const ParseException& e) {
                        std::cerr<<"Error while fetching neighbors of: "<<node<<std::endl;
                        throw e;
                    }
                }
                if (--active_tasks == 0) {
                    bq.finish();
                }
            }
            curl_easy_cleanup(local_curl);
        }));
        
    }
    for (auto& t: threadgroup) {
            t.join();
    }

    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <node_name> <depth>\n";
        return 1;
    }

    string start_node = argv[1];     // example "Tom%20Hanks"
    int depth;
    try {
        depth = stoi(argv[2]);
    } catch (const exception& e) {
        cerr << "Error: Depth must be an integer.\n";
        return 1;
    }

    // CURL* curl = curl_easy_init();
    // if (!curl) {
    //     cerr << "Failed to initialize CURL" << endl;
    //     return -1;
    // }


    const auto start{std::chrono::steady_clock::now()};
    
    
    for (const auto& node : bfs(start_node, depth))
        cout << "- " << node << "\n";

    const auto finish{std::chrono::steady_clock::now()};
    const std::chrono::duration<double> elapsed_seconds{finish - start};
    std::cout << "Time to crawl: "<<elapsed_seconds.count() << "s\n";
    
    // curl_easy_cleanup(curl);

    
    return 0;
}
