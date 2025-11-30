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
        double m_width = 2.0;
        double m_tileSize = m_width / totalCols;
        double m_height = totalRows * m_tileSize;
        double m_startX = -1.0;
        double m_startY = -m_height / 2.0;

        for (int row = 0; row < mapHeight; ++row) {
            std::string& currentRow = mapData[row];
            for (int col = 0; col < currentRow.length(); ++col) {
                char tile = currentRow[col];

                double x = m_startX + (col * m_tileSize) + (m_tileSize / 2.0);
                double y = m_startY + (row * m_tileSize) + (m_tileSize / 2.0);

                if (tile == '#') m_world.addWall(x, y , m_tileSize,m_tileSize);
                else if (tile == '*' || tile == '.') m_world.addCoin(x, y , m_tileSize,m_tileSize);
                else if (tile == 'P') m_world.addPacMan(x, y , m_tileSize,m_tileSize);
            }
        }
    }

}
