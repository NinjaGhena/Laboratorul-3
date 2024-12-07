#ifndef CODEFILE_H
#define CODEFILE_H

#include "FileSnapshot.h"

class CodeFile : public FileSnapshot {
private:
    int lineCount;
    int classCount;
    int methodCount;

public:
    CodeFile(const std::string& name, int lines, int classes, int methods)
        : FileSnapshot(name, "py"), lineCount(lines), classCount(classes), methodCount(methods) {}

    void getInfo() const override {
        FileSnapshot::getInfo();
        std::cout << "Lines: " << lineCount << ", Classes: " << classCount << ", Methods: " << methodCount << "\n";
    }
};

#endif
