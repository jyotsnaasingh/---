#ifndef SANDHI_ENGINE_H
#define SANDHI_ENGINE_H
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
using namespace std;

struct Node {
    // public:
    Node* links[51];
    Node* parent = nullptr;
    bool flag = false;
    unordered_set<int> wordsOf1;

    Node() {
        for (int i = 0; i < 51; i++) {
            links[i] = nullptr;
        }
    }

    bool containsKey(int ch) {
        return links[ch] != nullptr;
    }

    Node* get(int ch) {
        return links[ch];
    }

    void put(int ch, Node* node) {
        links[ch] = node;
    }

    void setParent(Node* node) {
        parent = node;
    }

    bool isEnd() {
        return flag == true;
    }

    void setEnd() {
        flag = true;
    }

    Node* returnParent(Node* node) {
        return node->parent;
    }
};

class ForwardTrie {
    Node* root;

public:
    ForwardTrie() {
        root = new Node();
    }

    void insert(vector<int> word) {
        Node* node = root;

        for (int i = 0; i < word.size(); i++) {
            if (!node->containsKey(word[i])) {
                node->put(word[i], new Node());
            }

            if (i == word.size() - 1) {
                node->wordsOf1.insert(word[i]);
            }

            node = node->get(word[i]);
        }

        node->setEnd();
    }

    bool isWord(vector<int> word) {
        Node* node = root;

        for (int i = 0; i < word.size(); i++) {
            if (!node->containsKey(word[i])) {
                return false;
            }

            node = node->get(word[i]);
        }

        return node->isEnd();
    }

    Node* nextLetter(vector<int> word) {
        Node* node = root;

        for (int i = 0; i < word.size(); i++) {
            if (!node->containsKey(word[i]))
                return nullptr;

            node = node->get(word[i]);
        }

        return node;
    }

    Node* searchfrommid(Node* node, int ch) {
        if (!node->containsKey(ch))
            return nullptr;

        return node->get(ch);
    }
    Node* getRoot() const {
    return root;
}
void setRoot(Node* node) {
    root = node;
}
};

class ReversedTrie {
    Node* root;

public:
    ReversedTrie() {
        root = new Node();
    }

    void insert(vector<int> word) {
        Node* node = root, *parent;

        for (int i = word.size() - 1; i >= 0; i--) {
            parent = node;

            if (!node->containsKey(word[i])) {
                node->put(word[i], new Node());
            }

            if (i == 0) {
                node->wordsOf1.insert(word[i]);
            }

            node = node->get(word[i]);
            node->setParent(parent);
        }

        node->setEnd();
    }

    bool isWord(vector<int> word) {
        Node* node = root;

        for (int i = word.size() - 1; i >= 0; i--) {
            if (!node->containsKey(word[i])) {
                return false;
            }

            node = node->get(word[i]);
        }

        return node->isEnd();
    }

    Node* nextLetter(vector<int> word) {
        Node* node = root;

        for (int i = word.size() - 1; i >= 0; i--) {
            if (!node->containsKey(word[i]))
                return nullptr;

            node = node->get(word[i]);
        }

        return node;
    }

    Node* searchfrommid(Node* node, int ch) {
        if (!node->containsKey(ch))
            return nullptr;

        return node->get(ch);
    }
     Node* getRoot() const {
    return root;
}
void setRoot(Node* node) {
    root = node;
}
};

unordered_map<int, int> minus_maps = {
    // vowels to -1 (yana sandhi)
    // sparsha vyanjana(3,4), swara and antah stha vyanjana to -2(3 to 1)
    // n or m to -3 (5 to 1)
    // च्, ज्, झ्, ट्, ड्, या ल् to -4
};

unordered_map<int, vector<vector<int>>> mp = {

    // अकः सवर्णे दीर्घः
    {1,  {{0,0},{0,1},{1,0},{1,1}}},
    {3,  {{2,2},{2,3},{3,2},{3,3}}},
    {5,  {{4,4},{4,5},{5,4},{5,5}}},
    {7,  {{6,6},{6,7},{7,6},{7,7}}},
    {9,  {{8,8},{8,9},{9,8},{9,9}}},

    // आद्गुणः
    {10, {{0,2},{0,3},{1,2},{1,3}}},
    {12, {{0,4},{0,5},{1,4},{1,5}}},

    // वृद्धिरेचि
    {11, {{0,10},{0,11},{1,10},{1,11}}},
    {13, {{0,12},{0,13},{1,12},{1,13}}},

    // इको यणचि
    {39, {{2,-1},{3,-1}}},
    {42, {{4,-1},{5,-1}}},
    {40, {{6,-1},{7,-1}}},
    {41, {{8,-1},{9,-1}}},
    {}
};

vector<vector<int>> split(vector<int> sandhi, ForwardTrie fwd, ReversedTrie rvs) {

    vector<int> sub1 = {sandhi[0]},
                sub2(sandhi.begin() + 2, sandhi.end());

    Node* forward_last, *backward_last;

    forward_last = fwd.nextLetter(sub1);
    backward_last = rvs.nextLetter(sub2);

    unordered_set<int> starts, ends;
    vector<vector<int>> combos, ans;

    bool splitfound = false;

    reverse(sub2.begin(), sub2.end());
    sub1 = {};
    sub2.push_back(sandhi[1]);

    for (int i = 1; i < sandhi.size() - 1; i++) {

        int curr = sandhi[i];

        sub1.push_back(sandhi[i - 1]);
        sub2.pop_back();

        starts = forward_last->wordsOf1;
        ends = backward_last->wordsOf1;
// if (curr == 11) {
//     cout << "\nAt वृद्धि boundary:\n";

//     cout << "STARTS: ";
//     for (int x : starts) cout << x << " ";

//     cout << "\nENDS: ";
//     for (int x : ends) cout << x << " ";

//     cout << endl;
// }
   
if (mp.find(curr) != mp.end()) {

            combos = mp[curr];

            for (int j = 0; j < combos.size(); j++) {

                if (starts.find(combos[j][0]) != starts.end() &&
                    (combos[j][1] == -1 ||
                     ends.find(combos[j][1]) != ends.end())) {

                    sub1.push_back(combos[j][0]);

                    if (combos[j][1] != -1) {
                        sub2.push_back(combos[j][1]);
                    }

                    reverse(sub2.begin(), sub2.end());
                    splitfound = true;
                    break;
                }
            }
        }

        if (!splitfound && i != sandhi.size() - 2 && curr == 0) {

            if (sandhi[i + 1] == 39) {
                if (starts.find(10) != starts.end()) {
                    splitfound = true;
                    sub1.push_back(10);
                    sub2.pop_back();
                    reverse(sub2.begin(), sub2.end());
                }
            }
            else if (sandhi[i + 1] == 42) {
                if (starts.find(12) != starts.end()) {
                    splitfound = true;
                    sub1.push_back(12);
                    sub2.pop_back();
                    reverse(sub2.begin(), sub2.end());
                }
            }

        }

        else if (!splitfound && i != sandhi.size() - 2 && curr == 1) {

            if (sandhi[i + 1] == 39) {
                if (starts.find(11) != starts.end()) {
                    splitfound = true;
                    sub1.push_back(11);
                    sub2.pop_back();
                    reverse(sub2.begin(), sub2.end());

                }
            }
            else if (sandhi[i + 1] == 42) {
                if (starts.find(13) != starts.end()) {
                    splitfound = true;
                    sub1.push_back(13);
                    sub2.pop_back();
                    reverse(sub2.begin(), sub2.end());
                }
            }

        }

        if (splitfound) {
            ans = {sub1, sub2};
            break;
        }

        forward_last = fwd.searchfrommid(forward_last, sandhi[i]);
        backward_last = backward_last->parent;
    }

    return ans;
}
#endif