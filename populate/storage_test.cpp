
#include <iostream>

#include "../core/storage_manager.h"

using namespace std;

int main() {

    ForwardTrie fwd;
    ReversedTrie rvs;

    // Sample word for testing
    vector<int> word = {14, 0, 33, 0};

    // Insert into both tries
    fwd.insert(word);
    rvs.insert(word);

    // Save dictionary
    try {
        StorageManager::save(fwd, rvs);
        cout << "Dictionary saved successfully.\n";
    }
    catch (const exception& e) {
        cout << "Save error: " << e.what() << endl;
        return 1;
    }

    // Create fresh tries
    ForwardTrie newFwd;
    ReversedTrie newRvs;

    // Load dictionary
    try {
        StorageManager::load(newFwd, newRvs);
        cout << "Dictionary loaded successfully.\n";
    }
    catch (const exception& e) {
        cout << "Load error: " << e.what() << endl;
        return 1;
    }

    // Verify the word exists in both tries
    cout << "Forward trie: "
         << (newFwd.isWord(word) ? "PASS" : "FAIL") << endl;

    cout << "Reversed trie: "
         << (newRvs.isWord(word) ? "PASS" : "FAIL") << endl;

    return 0;
}
