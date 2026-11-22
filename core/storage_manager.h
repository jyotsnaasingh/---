
#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <fstream>
#include <stdexcept>
#include <unordered_set>
#include <cstdint>

#include "sandhi_engine.h"

class StorageManager {
private:

    static void writeNode(std::ofstream& out, Node* node) {
        bool exists = (node != nullptr);
        out.write(reinterpret_cast<const char*>(&exists), sizeof(exists));

        if (!exists) return;

        out.write(reinterpret_cast<const char*>(&node->flag), sizeof(node->flag));

        uint64_t wordCount = node->wordsOf1.size();
        out.write(reinterpret_cast<const char*>(&wordCount), sizeof(wordCount));

        for (int word : node->wordsOf1) {
            out.write(reinterpret_cast<const char*>(&word), sizeof(word));
        }

        for (int i = 0; i < 51; i++) {
            writeNode(out, node->links[i]);
        }
    }

    static Node* readNode(std::ifstream& in, Node* parent = nullptr) {
        bool exists;
        in.read(reinterpret_cast<char*>(&exists), sizeof(exists));

        if (!in || !exists) return nullptr;

        Node* node = new Node();

        in.read(reinterpret_cast<char*>(&node->flag), sizeof(node->flag));

        uint64_t wordCount;
        in.read(reinterpret_cast<char*>(&wordCount), sizeof(wordCount));

        for (uint64_t i = 0; i < wordCount; i++) {
            int word;
            in.read(reinterpret_cast<char*>(&word), sizeof(word));
            node->wordsOf1.insert(word);
        }

        node->parent = parent;

        for (int i = 0; i < 51; i++) {
            node->links[i] = readNode(in, node);
        }

        if (!in) {
            delete node;
            throw std::runtime_error("Error while reading dictionary file.");
        }

        return node;
    }

public:

    static void save(ForwardTrie& forward, ReversedTrie& reversed,
                     const std::string& filename = "../storage/dictionary.dat") {

        std::ofstream out(filename, std::ios::binary);

        if (!out) {
            throw std::runtime_error("Could not open dictionary file for writing.");
        }

        const char magic[8] = {'S', 'A', 'N', 'D', 'H', 'I', '1', '\0'};
        out.write(magic, sizeof(magic));

        writeNode(out, forward.getRoot());
        writeNode(out, reversed.getRoot());

        if (!out) {
            throw std::runtime_error("Error while saving dictionary.");
        }
    }

    static void load(ForwardTrie& forward, ReversedTrie& reversed,
                     const std::string& filename = "../storage/dictionary.dat") {

        std::ifstream in(filename, std::ios::binary);

        if (!in) {
            throw std::runtime_error("Could not open dictionary file for reading.");
        }

        char magic[8];
        const char expected[8] = {'S', 'A', 'N', 'D', 'H', 'I', '1', '\0'};

        in.read(magic, sizeof(magic));

        if (!in || !std::equal(magic, magic + 8, expected)) {
            throw std::runtime_error("Invalid dictionary file format.");
        }

        Node* forwardRoot = readNode(in);
        Node* reversedRoot = readNode(in);

        if (forwardRoot == nullptr || reversedRoot == nullptr) {
            throw std::runtime_error("Dictionary file is incomplete or corrupted.");
        }

        // Root nodes do not have parents.
        forwardRoot->parent = nullptr;
        reversedRoot->parent = nullptr;

        forward.setRoot(forwardRoot);
        reversed.setRoot(reversedRoot);
    }
};

#endif
