
#include <iostream>
#include <fstream>
#include <string>

#include "../core/encoder_decoder.h"

using namespace std;

int main() {

    ifstream file("../data/hindi_words.txt");

    if (!file.is_open()) {
        cout << "Could not open hindi_words.txt" << endl;
        return 1;
    }

    string word;

    int total = 0;
    int passed = 0;
    int failed = 0;

    while (getline(file, word)) {

        if (!word.empty() && word.back() == '\r') {
            word.pop_back();
        }

        if (word.empty()) continue;

        total++;

        if (roundTripCheck(word)) {
            passed++;
        }
        else {
            failed++;

            vector<int> encoded = encodeWord(word);
            string decoded = decodeWord(encoded);

            cout << "FAILED: " << word
                 << " -> " << decoded << endl;
        }
    }

    file.close();

    cout << "\n========== CONVERSION CHECK ==========" << endl;
    cout << "Total words: " << total << endl;
    cout << "Passed:      " << passed << endl;
    cout << "Failed:      " << failed << endl;

    return failed == 0 ? 0 : 1;
}
