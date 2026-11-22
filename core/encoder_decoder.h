
#ifndef ENCODER_DECODER_H
#define ENCODER_DECODER_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;


inline unordered_map<int, int> unicodeToIndex = {

    // Independent vowels
    {0x0905, 0},  // अ
    {0x0906, 1},  // आ
    {0x0907, 2},  // इ
    {0x0908, 3},  // ई
    {0x0909, 4},  // उ
    {0x090A, 5},  // ऊ
    {0x090B, 6},  // ऋ
    {0x0960, 7},  // ॠ
    {0x090C, 8},  // ऌ
    {0x0961, 9},  // ॡ
    {0x090F, 10}, // ए
    {0x0910, 11}, // ऐ
    {0x0913, 12}, // ओ
    {0x0914, 13}, // औ

    // Dependent vowels / Matras
    {0x093E, 1},  // ा → आ
    {0x093F, 2},  // ि → इ
    {0x0940, 3},  // ी → ई
    {0x0941, 4},  // ु → उ
    {0x0942, 5},  // ू → ऊ
    {0x0943, 6},  // ृ → ऋ
    {0x0944, 7},  // ॄ → ॠ
    {0x0962, 8},  // ॢ → ऌ
    {0x0963, 9},  // ॣ → ॡ
    {0x0947, 10}, // े → ए
    {0x0948, 11}, // ै → ऐ
    {0x094B, 12}, // ो → ओ
    {0x094C, 13}, // ौ → औ

    // Consonants
    {0x0915, 14}, // क
    {0x0916, 15}, // ख
    {0x0917, 16}, // ग
    {0x0918, 17}, // घ
    {0x0919, 18}, // ङ

    {0x091A, 19}, // च
    {0x091B, 20}, // छ
    {0x091C, 21}, // ज
    {0x091D, 22}, // झ
    {0x091E, 23}, // ञ

    {0x091F, 24}, // ट
    {0x0920, 25}, // ठ
    {0x0921, 26}, // ड
    {0x0922, 27}, // ढ
    {0x0923, 28}, // ण

    {0x0924, 29}, // त
    {0x0925, 30}, // थ
    {0x0926, 31}, // द
    {0x0927, 32}, // ध
    {0x0928, 33}, // न

    {0x092A, 34}, // प
    {0x092B, 35}, // फ
    {0x092C, 36}, // ब
    {0x092D, 37}, // भ
    {0x092E, 38}, // म

    {0x092F, 39}, // य
    {0x0930, 40}, // र
    {0x0932, 41}, // ल
    {0x0935, 42}, // व

    {0x0936, 43}, // श
    {0x0937, 44}, // ष
    {0x0938, 45}, // स
    {0x0939, 46}, // ह
    {0x0933, 47}  // ळ
};


inline unordered_map<int, pair<int,int>> vowelRev = {
    {0,{0x0905,0x0000}}, {1,{0x0906,0x093E}},
    {2,{0x0907,0x093F}}, {3,{0x0908,0x0940}},
    {4,{0x0909,0x0941}}, {5,{0x090A,0x0942}},
    {6,{0x090B,0x0943}}, {7,{0x0960,0x0944}},
    {8,{0x090C,0x0962}}, {9,{0x0961,0x0963}},
    {10,{0x090F,0x0947}}, {11,{0x0910,0x0948}},
    {12,{0x0913,0x094B}}, {13,{0x0914,0x094C}}
};


inline unordered_map<int,int> consonantRev = {
    {14,0x0915},{15,0x0916},{16,0x0917},{17,0x0918},{18,0x0919},
    {19,0x091A},{20,0x091B},{21,0x091C},{22,0x091D},{23,0x091E},
    {24,0x091F},{25,0x0920},{26,0x0921},{27,0x0922},{28,0x0923},
    {29,0x0924},{30,0x0925},{31,0x0926},{32,0x0927},{33,0x0928},
    {34,0x092A},{35,0x092B},{36,0x092C},{37,0x092D},{38,0x092E},
    {39,0x092F},{40,0x0930},{41,0x0932},{42,0x0935},{43,0x0936},
    {44,0x0937},{45,0x0938},{46,0x0939},{47,0x0933}
};


inline bool isConsonant(int u) {
    return
        (u >= 0x0915 && u <= 0x0939) ||
        (u >= 0x0958 && u <= 0x095F);
}


inline bool isIndependentVowel(int u) {
    return
        (u >= 0x0905 && u <= 0x0914) ||
        u == 0x0960 ||
        u == 0x0961;
}


inline bool isDependentVowel(int u) {
    return u >= 0x093E && u <= 0x094C;
}


inline bool isHalant(int u) {
    return u == 0x094D;
}


inline bool isVowelRev(int x) {
    return x >= 0 && x <= 13;
}


inline bool isConsonantRev(int x) {
    return x >= 14 && x <= 50;
}


// UTF-8 string to Unicode codepoints
inline vector<int> toUnicode(string text) {

    vector<int> unicode;

    for (size_t i = 0; i < text.length();) {

        unsigned char c = text[i];
        int codePoint;

        if ((c & 0x80) == 0) {
            codePoint = c;
            i++;
        }
        else if ((c & 0xE0) == 0xC0) {
            codePoint = ((c & 0x1F) << 6) |
                        (text[i + 1] & 0x3F);
            i += 2;
        }
        else if ((c & 0xF0) == 0xE0) {
            codePoint = ((c & 0x0F) << 12) |
                        ((text[i + 1] & 0x3F) << 6) |
                        (text[i + 2] & 0x3F);
            i += 3;
        }
        else {
            codePoint = ((c & 0x07) << 18) |
                        ((text[i + 1] & 0x3F) << 12) |
                        ((text[i + 2] & 0x3F) << 6) |
                        (text[i + 3] & 0x3F);
            i += 4;
        }

        unicode.push_back(codePoint);
    }

    return unicode;
}


// Unicode codepoints to UTF-8 string
inline string unicodeToString(vector<int>& v) {

    string s;

    for (int u : v) {

        if (u <= 0x7F)
            s += char(u);

        else if (u <= 0x7FF) {
            s += char(0xC0 | (u >> 6));
            s += char(0x80 | (u & 0x3F));
        }

        else {
            s += char(0xE0 | (u >> 12));
            s += char(0x80 | ((u >> 6) & 0x3F));
            s += char(0x80 | (u & 0x3F));
        }
    }

    return s;
}


// ENCODER
// Hindi string -> Unicode -> integer representation
//
// This contains the exact conversion loop from your main().
inline vector<int> encodeWord(string text) {

    vector<int> unicode = toUnicode(text);

    vector<int> varn_vichheda;

    for (int i = 0; i < unicode.size(); i++) {

        if (isHalant(unicode[i])) continue;

        varn_vichheda.push_back(unicodeToIndex[unicode[i]]);

        // last
        if (i == unicode.size() - 1 && isConsonant(unicode[i])) {
            varn_vichheda.push_back(0);
            continue;
        }

        // non-last
        if (isConsonant(unicode[i]) && isConsonant(unicode[i + 1])) {
            // add a
            varn_vichheda.push_back(0);
        }
    }

    return varn_vichheda;
}


// DECODER
// Integer representation -> Unicode codepoints
//
// This contains the exact backtounicode loop from your main().
inline vector<int> decodeToUnicode(vector<int>& varn_vichheda) {

    vector<int> backtounicode;

    for (int i = 0; i < varn_vichheda.size(); i++) {

        if (isConsonantRev(varn_vichheda[i])) {
            backtounicode.push_back(consonantRev[varn_vichheda[i]]);
        }

        if (isVowelRev(varn_vichheda[i])) {
            backtounicode.push_back(vowelRev[varn_vichheda[i]].first);
        }

        // last
        if (i == varn_vichheda.size() - 1) {
            if (isConsonantRev(varn_vichheda[i])) {
            backtounicode.push_back(0x094D);
        }
        continue;
        }

        // non-last
        if (
            isConsonantRev(varn_vichheda[i]) &&
            isConsonantRev(varn_vichheda[i + 1])
        ) {
            backtounicode.push_back(0x094D);
        }
        else if (
            isConsonantRev(varn_vichheda[i]) &&
            isVowelRev(varn_vichheda[i + 1])
        ) {
            if (varn_vichheda[i + 1] != 0) {
                backtounicode.push_back(
                    vowelRev[varn_vichheda[i + 1]].second
                );
            }

            i++;
        }
    }

    return backtounicode;
}


// Complete decoder: integer representation -> Hindi string
inline string decodeWord(vector<int>& varn_vichheda) {

    vector<int> backtounicode = decodeToUnicode(varn_vichheda);

    string deva = unicodeToString(backtounicode);

    return deva;
}


// Same equality check performed in your original main().
inline bool roundTripCheck(string text) {

    vector<int> unicode = toUnicode(text);

    vector<int> varn_vichheda = encodeWord(text);

    vector<int> backtounicode = decodeToUnicode(varn_vichheda);

    return unicode == backtounicode;
}

#endif
