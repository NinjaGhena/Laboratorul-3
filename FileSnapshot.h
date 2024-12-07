#ifndef FILESNAPSHOT_H
#define FILESNAPSHOT_H

#include <iostream>
#include <string>
#include <chrono>
#include <filesystem>
#include <ctime>

class FileSnapshot {
protected:
    std::string filename;
    std::string extension;
    std::filesystem::file_time_type baselineWriteTime;
    bool changed;

    std::time_t to_time_t(std::filesystem::file_time_type ftime) const {
        auto since_epoch = ftime.time_since_epoch();
        auto sec = std::chrono::duration_cast<std::chrono::seconds>(since_epoch);
        std::time_t cftime = static_cast<std::time_t>(sec.count());
        return cftime;
    }

public:
    FileSnapshot(const std::string& name, const std::string& ext)
        : filename(name), extension(ext), changed(false) 
    {
        if (std::filesystem::exists(filename)) {
            baselineWriteTime = std::filesystem::last_write_time(filename);
        } else {
            baselineWriteTime = std::filesystem::file_time_type::min();
        }
    }

    virtual ~FileSnapshot() = default;

    virtual void getInfo() const {
        std::cout << "Filename: " << filename << "\n";
        std::cout << "Extension: " << extension << "\n";
        
        if (std::filesystem::exists(filename)) {
            auto ftime = std::filesystem::last_write_time(filename);
            std::time_t cftime = to_time_t(ftime);
            std::cout << "Last Modified: " << std::asctime(std::localtime(&cftime));
        } else {
            std::cout << "File does not currently exist on the filesystem.\n";
        }
    }

    void updateBaseline() {
        if (std::filesystem::exists(filename)) {
            baselineWriteTime = std::filesystem::last_write_time(filename);
        } else {
            baselineWriteTime = std::filesystem::file_time_type::min();
        }
    }

    bool checkIfChanged() {
        if (std::filesystem::exists(filename)) {
            auto currentWriteTime = std::filesystem::last_write_time(filename);
            changed = (currentWriteTime != baselineWriteTime);
        } else {
            changed = true; // If file was deleted, consider it changed
        }
        return changed;
    }

    bool isChanged() const {
        return changed;
    }

    std::string getFilename() const {
        return filename;
    }
};

#endif
