#pragma once

#include <string>


class Map {
    private:
        const char* mapFilePath;
        int mapScale;
        int tileSize;
        int scaledSize;

    public:
        Map(const char* mFilePath, int mScale, int tSize);
        ~Map();

        void LoadMap(std::string path, int sizeX, int sizeY);
        void AddTile(int srcX, int srcY, int xpos, int ypos);
};
