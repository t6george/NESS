#include <dirent.h>
#include <unistd.h>

#include <FileExplorer.hpp>

FileExplorer::FileExplorer() : fileFrame{0}, filePointer{0}, path{".."}
{
    // chdir("..");
    getFiles();
}

void FileExplorer::parseDirectory()
{
    if ((dir = opendir(".")))
    {
        while ((ent = readdir(dir)))
        {
            files.emplace_back(ent->d_name);
            // printf("%s\n", ent->d_name);
        }
        closedir(dir);
    }
    else
    {
        perror("Error opening directory\n");
        return 0;
    }
}

void FileExplorer::drawFiles(const int8_t moveSelect)
{
    filePointer += moveSelect;
    filePointer = fmax(fmax(0, filePointer), files.size() - 1);
    if (filePointer > fileFrame + 2)
    {
        fileFrame += 3;
    }
    else if (filePointer < fileFrame)
    {
        fileFrame -= 3;
    }
    
    TTF_Font* Sans = TTF_OpenFont("Sans.ttf", 24); //this opens a font style and sets a size

    SDL_Color White = {255, 255, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, "put your text here", White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture

    SDL_Rect Message_rect; //create a rect
    Message_rect.x = 0;  //controls the rect's x coordinate 
    Message_rect.y = 0; // controls the rect's y coordinte
    Message_rect.w = 100; // controls the width of the rect
    Message_rect.h = 100; // controls the height of the rect

    SDL_RenderCopy(renderer, Message, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture

}

void FileExplorer::select()
{
    path += files.at(filePointer);
    dir = opendir(path);

    if (pDir)
    {
        chdir(path);
    }
    else
    {

    }

    return bExists;
}