#pragma once

#include <string>


class Map {
    private:
        std::string textureID;
        int mapScale;
        int tileSize;
        int scaledSize;
        int sizeX;
        int sizeY;

    public:
        Map(std::string texID, int mScale, int tSize, int mSizeX, int mSizeY);
        ~Map();

        void LoadMap(std::string path);
        void AddTile(int srcX, int srcY, int xpos, int ypos);

        int getScaledSize() {
            return scaledSize;
        }

        int getSizeX() {
            return sizeX;
        }

        int getSizeY() {
            return sizeY;
        }
};
