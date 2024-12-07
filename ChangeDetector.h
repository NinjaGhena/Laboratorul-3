#ifndef CHANGEDETECTOR_H
#define CHANGEDETECTOR_H

#include "FileSnapshot.h"
#include <map>
#include <string>
using namespace std;

class ChangeDetector {
private:
    map<string, FileSnapshot*> snapshots;

public:
    void commit() {
        cout << "Snapshot updated.\n";
    }

    void detectChanges() {
        cout << "Detecting changes...\n";
        for (const auto& [filename, snapshot] : snapshots) {
            cout << filename << ": No changes detected.\n";
        }
    }

    void addFile(FileSnapshot* file) {
        snapshots[file->getFilename()] = file;
    }

    ~ChangeDetector() {
        for (auto& [filename, snapshot] : snapshots) {
            delete snapshot;
        }
    }
};

#endif
