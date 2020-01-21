#include <FileExplorer.hpp>

FileExplorer::FileExplorer() : fileFrame{0}, filePointer{0}, path{".."}, visible{false} {}

void FileExplorer::getFiles(const int8_t moveSelect)
{
    filePointer += moveSelect;
    filePointer = std::max<int16_t>(std::min<int16_t>(0, filePointer), files.size() - 1);

    if (filePointer > fileFrame + 2)
    {
        fileFrame += 3;
    }
    else if (filePointer < fileFrame)
    {
        fileFrame -= 3;
    }
}

void FileExplorer::select()
{
    path += files.at(filePointer);
    dir = opendir(path.c_str());

    if (dir)
    {
        files.clear();
        while ((ent = readdir(dir)))
        {
            files.emplace_back(ent->d_name);
        }
        closedir(dir);
    }
    else
    {
    }
}