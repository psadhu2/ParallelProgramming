#include "curl/curl.h"
#include "rapidjson/document.h"
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>
using namespace std;
using namespace rapidjson;

/*
bfs function will basically take in the head node and will output all the neighbors
based off of the depth inputed we will need to do some kind of json parsing
on the neigbhors and get their output too.
so bfs will need to take in a string name for the url and int depth
bfs will need a helper function called jsonParser to parse through the neighbors of the
head node
*/

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

vector<string> jsonParser(string query) {
    vector<string> neighbors;

    CURL *curl = curl_easy_init();
    CURLcode res;
    string readBuffer;

    char* encoded = curl_easy_escape(curl, query.c_str(), 0);
    string url = "http://hollywood-graph-crawler.bridgesuncc.org/neighbors/" + string(encoded);
    curl_free(encoded);

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        CURLcode result = curl_easy_perform(curl);

        if (result != CURLE_OK) {
            cerr << "CURL error for " << query << ": " << curl_easy_strerror(result) << endl;
            curl_easy_cleanup(curl);
            return neighbors;
        }

        if (readBuffer.empty()) {
            cerr << "Empty response for: " << query << endl;
            curl_easy_cleanup(curl);
            return neighbors;
        }

        Document d;
        d.Parse(readBuffer.c_str());

        if (d.HasParseError()) {
            cerr << "JSON parse error for " << query << ": " << d.GetParseError() << endl;
            cerr << "Response was: " << readBuffer << endl;
            curl_easy_cleanup(curl);
            return neighbors;
        }

        if (!d.IsObject()) {
            cerr << "Response is not a JSON object for: " << query << endl;
            cerr << "Response was: " << readBuffer << endl;
            curl_easy_cleanup(curl);
            return neighbors;
        }

        if (d.HasMember("neighbors") && d["neighbors"].IsArray()) {
            const Value& a = d["neighbors"];
            for (SizeType i = 0; i < a.Size(); i++) {
                neighbors.push_back(a[i].GetString());
            }
        }

        curl_easy_cleanup(curl);
    }
    return neighbors;
}

void bfs(string query, int depth) {
    queue<pair<string, int>> q;
    set<string> visited;
    q.push({query, 0});
    visited.insert(query);
    
    while(!q.empty()) {
        pair<string, int> current = q.front();
        q.pop();

        string currentName = current.first;
        int currentDepth = current.second;

        cout << "Visiting: " << currentName << " (Depth: " << currentDepth << ")" << endl;

        if (currentDepth >= depth) {
            continue; 
        }

        vector<string> neighbors = jsonParser(currentName);

        for (string neighbor : neighbors) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push({neighbor, currentDepth + 1});
            }
        }
    }
}

int main() {
    string query;
    cout << "Enter an actor's name or a movie name: ";
    getline(cin, query);
    int depth;
    cout << "Enter the distance you want to crawl in the tree: ";
    cin >> depth;
    bfs(query, depth);
    return 0;
}