#ifndef TEXTFILE_H
#define TEXTFILE_H

#include "FileSnapshot.h"

class TextFile : public FileSnapshot {
private:
    int lineCount;
    int wordCount;
    int charCount;

public:
    TextFile(const std::string& name, int lines, int words, int chars)
        : FileSnapshot(name, "txt"), lineCount(lines), wordCount(words), charCount(chars) {}

    void getInfo() const override {
        FileSnapshot::getInfo();
        std::cout << "Lines: " << lineCount << ", Words: " << wordCount << ", Characters: " << charCount << "\n";
    }
};

#endif
