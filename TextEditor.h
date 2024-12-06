#ifndef ENGINE_TEXTEDITOR_H
#define ENGINE_TEXTEDITOR_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

class TextEditor {
private:
    std::string filename;

private:
    std::string readFromFile() const {
        std::ifstream file(filename);

        if (!file.is_open()) {
            throw std::runtime_error("Unable to open the file for reading");
        }

        std::ostringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    void writeToFile(const std::string& text) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open the file for writing");
        }
        file << text;
    }

    std::vector<std::string> splitSentences(const std::string& text) const {
        std::regex sentenceRegex(R"(([^.!?\n]*[.!?\n]))");
        std::sregex_iterator begin(text.begin(), text.end(), sentenceRegex), end;
        std::vector<std::string> sentences;
        for (auto it = begin; it != end; ++it) {
            sentences.push_back(it->str());
        }
        return sentences;
    }

public:
    explicit TextEditor(const std::string& file) : filename(file) {}

    void replaceWord(const std::string oldWord, const std::string newWord) { // accept by value at the moment, there are problems with types in arguments if we accept by reference
        std::string text = readFromFile();
        std::regex wordRegex("\\b" + oldWord + "\\b");
        text = std::regex_replace(text, wordRegex, newWord);
        writeToFile(text);
    }

    std::vector<size_t> findPhrase(const std::string phrase) {
        std::string text = readFromFile();
        std::vector<size_t> positions;
        size_t pos = text.find(phrase);
        while (pos != std::string::npos) {
            positions.push_back(pos);
            pos = text.find(phrase, pos + 1);
        }
        return positions;
    }

    void toUpperWords() {
        std::string text = readFromFile();
        bool upperNext = true;
        for (char& c : text) {
            if (std::isspace(c)) {
                upperNext = true;
            } else if (upperNext && std::isalpha(c)) {
                c = std::toupper(c);
                upperNext = false;
            }
        }
        writeToFile(text);
    }

    void toUpperSentences() {
        std::string text = readFromFile();
        bool upperNext = true;
        for (char& c : text) {
            if (upperNext && std::isalpha(c)) {
                c = std::toupper(c);
                upperNext = false;
            }
            if (c == '.' || c == '!' || c == '?' || c == '\n') {
                upperNext = true;
            }
        }
        writeToFile(text);
    }

    void addSentenceNumbering() {
        std::string text = readFromFile();
        std::vector<std::string> sentences = splitSentences(text);
        std::ostringstream numberedText;
        for (size_t i = 0; i < sentences.size(); ++i) {
            numberedText << (i + 1) << ". " << sentences[i];
        }
        writeToFile(numberedText.str());
    }

    std::string getText() const {
        return readFromFile();
    }
};


#endif //ENGINE_TEXTEDITOR_H
