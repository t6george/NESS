#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <SDL2/SDL.h>

class FileExplorer
{
    public:
    std::vector<std::string> files;
    int16_t fileFrame;
    int16_t filePointer;
    std::string path;

    DIR *dir;
    struct dirent *ent;

    FileExplorer();
    ~FileExplorer() = default;
    void parseCurrentDirectory();
    void drawFiles(const int8_t moveSelect);
    void select();
};

