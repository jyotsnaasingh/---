#include <iostream>
#include <string>
#include <vector>


#include "httplib.h"

#include "../core/sandhi_engine.h"
#include "../core/encoder_decoder.h"
#include "../core/storage_manager.h"

using namespace std;

int main() {
    ForwardTrie forwardTrie;
    ReversedTrie reversedTrie;

    const string storageFile = "storage/dictionary.dat";
    cout << "Loading dictionary..." << endl;

    StorageManager::load(forwardTrie, reversedTrie, storageFile);

    cout << "Dictionary loaded." << endl;

    httplib::Server server;

server.Get("/", [](const httplib::Request& req,
                   httplib::Response& res) {
    ifstream file("web/index.html");
    if (!file) {
        res.status = 404;
        res.set_content("index.html not found", "text/plain");
        return;
    }

    string content((istreambuf_iterator<char>(file)),
                    istreambuf_iterator<char>());

    res.set_content(content, "text/html; charset=utf-8");
});

server.Get("/style.css", [](const httplib::Request& req,
                            httplib::Response& res) {
    ifstream file("web/style.css");
    string content((istreambuf_iterator<char>(file)),
                    istreambuf_iterator<char>());

    res.set_content(content, "text/css");
});

server.Get("/script.js", [](const httplib::Request& req,
                            httplib::Response& res) {
    ifstream file("web/script.js");
    string content((istreambuf_iterator<char>(file)),
                    istreambuf_iterator<char>());

    res.set_content(content, "application/javascript");
});


    server.Get("/split", [&](const httplib::Request& req,
                             httplib::Response& res) {

        if (!req.has_param("word")) {
            res.status = 400;
            res.set_content("Missing word parameter.", "text/plain");
            return;
        }

        string input = req.get_param_value("word");

        vector<int> encoded = encodeWord(input);

        vector<vector<int>> result =
            split(encoded, forwardTrie, reversedTrie);

        if (result.empty()) {
            res.set_content("No split found.", "text/plain");
            return;
        }

        string firstWord = decodeWord(result[0]);
        string secondWord = decodeWord(result[1]);

        string output =
            firstWord + " + " + secondWord;

        res.set_content(output, "text/plain; charset=utf-8");
    });

    cout << "Server starting on http://localhost:8080" << endl;

    server.listen("0.0.0.0", 8080);

    return 0;
}