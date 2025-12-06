//
// Created by Luca Letroye on 29/11/2025.
//
#include <iostream>
#include "logic/utils/txtMapLoader.h"
#include <fstream>

namespace logic{
    void txtMapLoader::loadMap(std::string filename){
        std::string path = "assets/" + filename;
        std::ifstream file(path);

        if (!file.is_open()) {
            std::cerr << "FATAL: Kon map niet vinden op: " << filename << std::endl;
            return;

        }

        std::vector<std::string> mapData;
        std::string line;
        int maxWidth = 0;
        while (std::getline(file, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            mapData.push_back(line);
            if (line.length() > maxWidth) maxWidth = line.length();
        }
        int mapWidth = maxWidth;
        int mapHeight = mapData.size();
        if (mapWidth == 0 || mapHeight == 0) return;

        int totalRows = mapHeight + 2;
        int totalCols = mapWidth;
        double worldWidth = 2.0;
        double calculatedTileSize = worldWidth / totalCols;

        double totalWorldHeight = totalRows * calculatedTileSize;
        double calcStartX = -1.0;
        double calcStartY = -(totalWorldHeight / 2.0);

        m_world.setGridDimensions(calcStartX, calcStartY, calculatedTileSize);
        for (int row = 0; row < mapHeight; ++row) {
            std::string& currentRow = mapData[row];
            for (int col = 0; col < currentRow.length(); ++col) {
                char tile = currentRow[col];

                double x = calcStartX + (col * calculatedTileSize) + (calculatedTileSize / 2.0);
                double y = calcStartY + (row * calculatedTileSize) + (calculatedTileSize / 2.0);

                if (tile == '#') m_world.addWall(x, y, calculatedTileSize, calculatedTileSize);
                else if (tile == '*' || tile == '.') m_world.addCoin(x, y, calculatedTileSize, calculatedTileSize);
                else if (tile == 'P') m_world.addPacMan(x, y, calculatedTileSize, calculatedTileSize);
            }
        }
    }

}
