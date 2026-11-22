
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../core/sandhi_engine.h"
#include "../core/encoder_decoder.h"
#include "../core/storage_manager.h"

using namespace std;

int main() {

    // Create both dictionaries
    ForwardTrie forwardTrie;
    ReversedTrie reversedTrie;

    const string inputFile = "../data/hindi_words.txt";
    const string storageFile = "../storage/dictionary.dat";

    // Load existing dictionaries if a storage file already exists.
    // This prevents previously populated entries from being lost.
    ifstream existingStorage(storageFile, ios::binary);

    if (existingStorage.good()) {
        existingStorage.close();

        StorageManager::load(
            forwardTrie,
            reversedTrie,
            storageFile
        );

        cout << "Existing dictionaries loaded.\n";
    }
    else {
        cout << "Starting with empty dictionaries.\n";
    }

    // Open Hindi word dataset
    ifstream input(inputFile);

    if (!input.is_open()) {
        cerr << "ERROR: Could not open " << inputFile << '\n';
        return 1;
    }

    string hindiWord;
    int totalWords = 0;
    int insertedWords = 0;
    int skippedWords = 0;

    cout << "\nStarting dictionary population...\n\n";

    while (getline(input, hindiWord)) {

        if (hindiWord.empty()) {
            continue;
        }

        totalWords++;

        // Convert Hindi string into the existing integer representation
        vector<int> wordVector = encodeWord(hindiWord);

        if (wordVector.empty()) {
            cout << "SKIPPED: " << hindiWord << '\n';
            skippedWords++;
            continue;
        }

        // Insert into both dictionaries
   

forwardTrie.insert(wordVector);
reversedTrie.insert(wordVector);



        insertedWords++;

        if (insertedWords % 100 == 0) {
            cout << insertedWords << " words inserted...\n";
        }
    }

    input.close();

    // Persist both dictionaries
  

StorageManager::save(
    forwardTrie,
    reversedTrie,
    storageFile
);


    cout << "\n========== POPULATION COMPLETE ==========\n";
    cout << "Total words read: " << totalWords << '\n';
    cout << "Successfully inserted: " << insertedWords << '\n';
    cout << "Skipped: " << skippedWords << '\n';
    cout << "Storage file: " << storageFile << '\n';

    return 0;
}
