
#include <iostream>
#include <string>
#include <vector>

#include "../core/sandhi_engine.h"
#include "../core/encoder_decoder.h"
#include "../core/storage_manager.h"

using namespace std;

int main() {
    ForwardTrie forwardTrie;
    ReversedTrie reversedTrie;

    const string storageFile = "../storage/dictionary.dat";

    cout << "Loading dictionary..." << endl;

    StorageManager::load(forwardTrie, reversedTrie, storageFile);

    cout << "Dictionary loaded." << endl;

    string input;

    while (true) {
        cout << "\nEnter word (or type exit): ";
        cin >> input;

        if (input == "exit") {
            break;
        }

        vector<int> encoded = encodeWord(input);

        vector<vector<int>> result =
            split(encoded, forwardTrie, reversedTrie);

        cout << "\n========== RESULT ==========" << endl;

        if (result.empty()) {
            cout << "No split found." << endl;
        }
        else {
            string firstWord = decodeWord(result[0]);
            string secondWord = decodeWord(result[1]);

            cout << "First word  : " << firstWord << endl;
            cout << "Second word : " << secondWord << endl;

            cout << "\nSandhi Vichchheda: "
                 << firstWord << " + " << secondWord << endl;
        }

        cout << "============================" << endl;
    }

    return 0;
}
