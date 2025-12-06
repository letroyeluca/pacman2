//
// Created by Luca Letroye on 29/11/2025.
//
#include "logic/utils/TxtMapLoader.h"
#include <fstream>
#include <iostream>
#include <algorithm>

namespace logic {
#include <algorithm> // Voor std::max

    void TxtMapLoader::loadMap(std::string filename) {
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

        int mapCols = maxWidth;     // De breedte uit de file
        int mapRows = mapData.size(); // De hoogte uit de file

        if (mapCols == 0 || mapRows == 0) return;

        // -----------------------------------------------------------
        // 1. EXTRA RUIMTE RESERVEREN
        // -----------------------------------------------------------
        // We voegen 2 virtuele rijen toe aan de hoogte voor de UI onderaan.
        int scoreRows = 2;
        int totalRows = mapRows + scoreRows;

        // -----------------------------------------------------------
        // 2. NORMALISATIE (Met de nieuwe totale hoogte)
        // -----------------------------------------------------------
        double maxLogicDimension = 2.0;

        // We gebruiken hier 'totalRows' (map + score) om te bepalen hoe groot alles moet zijn.
        // Hierdoor wordt er automatisch uitgezoomd als de map + score te hoog wordt.
        int maxGridDimension = std::max(mapCols, totalRows);

        double calculatedTileSize = maxLogicDimension / maxGridDimension;

        // De totale wereldgrootte bevat nu ook het lege gebied onderaan
        double worldWidth = mapCols * calculatedTileSize;
        double worldHeight = totalRows * calculatedTileSize;

        // Centreren: (0,0) ligt in het midden van de TOTALE wereld (inclusief score gebied).
        double calcStartX = -(worldWidth / 2.0);
        double calcStartY = -(worldHeight / 2.0);

        // -----------------------------------------------------------
        // 3. WORLD UPDATEN
        // -----------------------------------------------------------
        m_world.setGridDimensions(calcStartX, calcStartY, calculatedTileSize);
        m_world.setWorldDimensions(worldWidth, worldHeight);

        // -----------------------------------------------------------
        // 4. ENTITEITEN PLAATSEN
        // -----------------------------------------------------------
        // BELANGRIJK: We loopen alleen over 'mapRows' (de echte data),
        // niet over 'totalRows'. De onderste 2 rijen blijven dus leeg.
        for (int row = 0; row < mapRows; ++row) {
            std::string& currentRow = mapData[row];

            // Zorg dat we niet crashen als een regel korter is dan maxWidth
            // (Al zou je map file eigenlijk rechthoekig moeten zijn)
            int colsInRow = currentRow.length();

            for (int col = 0; col < colsInRow; ++col) {
                char tile = currentRow[col];

                double x = calcStartX + (col * calculatedTileSize) + (calculatedTileSize / 2.0);
                double y = calcStartY + (row * calculatedTileSize) + (calculatedTileSize / 2.0);

                if (tile == '#')
                    m_world.addWall(x, y, calculatedTileSize, calculatedTileSize);
                else if (tile == '*' || tile == '.')
                    m_world.addCoin(x, y, calculatedTileSize, calculatedTileSize);
                else if (tile == 'P')
                    m_world.addPacMan(x, y, calculatedTileSize, calculatedTileSize);
            }
        }
    }

} // namespace logic
