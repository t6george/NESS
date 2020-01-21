#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include <dirent.h>
#include <unistd.h>


class FileExplorer
{
    public:
    std::vector<std::string> files;
    int16_t fileFrame;
    int16_t filePointer;
    std::string path;
    bool visible;

    DIR *dir;
    struct dirent *ent;

    FileExplorer();
    ~FileExplorer() = default;
    void getFiles(const int8_t moveSelect);
    void select();
};

