#include "TextFile.h"
#include "ImageFile.h"
#include "CodeFile.h"
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <filesystem>

using namespace std;

class ChangeDetector {
public:
    map<string, FileSnapshot*> files;
    // Stores file contents at the time of the last commit
    map<string, vector<string>> commitSnapshots;

    void addFile(FileSnapshot* file) {
        files[file->getFilename()] = file;
    }

    vector<string> readFileLines(const string& filename) {
        vector<string> lines;
        if (std::filesystem::exists(filename)) {
            ifstream file(filename);
            string line;
            while (std::getline(file, line)) {
                lines.push_back(line);
            }
        }
        return lines;
    }

    void commit() {
        commitSnapshots.clear();
        for (auto& [filename, file] : files) {
            file->updateBaseline();
            // Store current file contents
            commitSnapshots[filename] = readFileLines(filename);
        }
        cout << "Snapshot committed. All files marked as unchanged.\n";
    }

    void info(const string& filename) const {
        if (files.count(filename)) {
            files.at(filename)->getInfo();
        } else {
            cout << "File not found.\n";
        }
    }

    void showDifferences(const vector<string>& oldLines, const vector<string>& newLines) {
        // Simple diff: lines added, lines removed
        size_t oldSize = oldLines.size();
        size_t newSize = newLines.size();

        // Check for added lines
        if (newSize > oldSize) {
            cout << "New lines added:\n";
            for (size_t i = oldSize; i < newSize; ++i) {
                cout << "+ " << newLines[i] << "\n";
            }
        }

        // Check for removed lines
        if (oldSize > newSize) {
            cout << "Lines removed:\n";
            for (size_t i = newSize; i < oldSize; ++i) {
                cout << "- " << oldLines[i] << "\n";
            }
        }

        // Check modified lines
        size_t common = std::min(oldSize, newSize);
        for (size_t i = 0; i < common; ++i) {
            if (oldLines[i] != newLines[i]) {
                cout << "Line " << (i + 1) << " changed from:\n" 
                     << oldLines[i] << "\n"
                     << "to:\n"
                     << newLines[i] << "\n";
            }
        }
    }

    void status() {
        cout << "--- File Status ---\n";
        for (auto& [filename, file] : files) {
            bool changed = file->checkIfChanged();
            cout << filename << ": " << (changed ? "Changed" : "Unchanged") << "\n";
            if (changed) {
                // Show what changed if the file exists now
                if (std::filesystem::exists(filename)) {
                    vector<string> currentLines = readFileLines(filename);
                    const auto& oldLines = commitSnapshots[filename];
                    showDifferences(oldLines, currentLines);
                } else {
                    cout << filename << " was deleted.\n";
                }
            }
        }
    }

    ~ChangeDetector() {
        for (auto& [filename, file] : files) {
            delete file;
        }
    }
};

int main() {
    ChangeDetector detector;

    // Make sure these files exist in the current directory
    detector.addFile(new TextFile("example.txt", 10, 100, 500));
    detector.addFile(new ImageFile("image.png", 1920, 1080));
    detector.addFile(new CodeFile("script.py", 300, 5, 25));

    int choice;
    do {
        cout << "\n--- Document Change Detection System ---\n";
        cout << "1. Commit\n";
        cout << "2. File Info\n";
        cout << "3. Status\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            detector.commit();
            break;
        case 2: {
            string filename;
            cout << "Enter filename: ";
            cin >> filename;
            detector.info(filename);
            break;
        }
        case 3:
            detector.status();
            break;
        case 4:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 4);

    return 0;
}
