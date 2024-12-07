#ifndef IMAGEFILE_H
#define IMAGEFILE_H

#include "FileSnapshot.h"

class ImageFile : public FileSnapshot {
private:
    int width, height;

public:
    ImageFile(const std::string& name, int w, int h)
        : FileSnapshot(name, "png"), width(w), height(h) {}

    void getInfo() const override {
        FileSnapshot::getInfo();
        std::cout << "Dimensions: " << width << "x" << height << "\n";
    }
};

#endif
