#include <fstream>

#include "Map.hpp"
#include "Game.hpp"


Map::Map() {    }
Map::~Map() {    }


void Map::LoadMap(std::string path, int sizeX, int sizeY) {
    char tile;
    std::fstream mapFile(path);

    int srcX, srcY;

    for (int y = 0; y < sizeY; y++)
        for (int x = 0; x < sizeX; x++) {
            mapFile.get(tile);
            srcY = atoi(&tile) * 32;
            mapFile.get(tile);
            srcX = atoi(&tile) * 32;
            Game::AddTile(srcX, srcY, x * 64, y * 64);
            mapFile.ignore();
        }
    
    mapFile.close();
}
