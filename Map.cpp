#include <fstream>

#include "Map.hpp"
#include "Game.hpp"


Map::Map() {
    
}


Map::~Map() {
    
}


void Map::LoadMap(std::string path, int sizeX, int sizeY) {
    char tile;
    std::fstream mapFile(path);

    for (int y = 0; y < sizeY; y++)
        for (int x = 0; x < sizeX; x++) {
            mapFile.get(tile);
            Game::AddTile(atoi(&tile), x * 32, y * 32);
            mapFile.ignore();
        }
    
    mapFile.close();
}
