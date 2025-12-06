//
// Created by Luca Letroye on 29/11/2025.
//
#include "logic/utils/TxtMapLoader.h"
#include <fstream>
#include <iostream>
#include <algorithm>

namespace logic {
    void TxtMapLoader::loadMap(std::string filename) {
        // 1. Inlezen (Dit gedeelte was al goed)
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

        int cols = maxWidth;
        int rows = mapData.size();
        if (cols == 0 || rows == 0) return;

        // -----------------------------------------------------------
        // 2. NORMALISATIE BEREKENING (Aangepast voor Scorebalk)
        // -----------------------------------------------------------

        // STAP A: Definieer extra ruimte
        int scoreRows = 2; // We willen 2 regels ruimte onderaan
        int totalRows = rows + scoreRows; // Totale hoogte inclusief UI

        double maxLogicDimension = 2.0;

        // STAP B: Gebruik totalRows voor de schaalberekening!
        // Hierdoor worden de tegels iets kleiner zodat er ruimte overblijft.
        int maxGridDimension = std::max(cols, totalRows);

        double calculatedTileSize = maxLogicDimension / maxGridDimension;

        // STAP C: World dimensions baseren op TOTALE grootte
        double worldWidth = cols * calculatedTileSize;
        double worldHeight = totalRows * calculatedTileSize; // Inclusief de lege ruimte

        // Centreren
        double calcStartX = -(worldWidth / 2.0);
        double calcStartY = -(worldHeight / 2.0);

        // -----------------------------------------------------------
        // 3. WORLD UPDATEN & SCORE PLAATSEN
        // -----------------------------------------------------------

        m_world.setGridDimensions(calcStartX, calcStartY, calculatedTileSize);
        m_world.setWorldDimensions(worldWidth, worldHeight);

        double scoreX = 0.0;

        // De absolute bodem is (worldHeight / 2.0).
        // We gaan 1 tegel omhoog om in het midden van de lege balk te komen.
        double scoreY = (worldHeight / 2.0) - (calculatedTileSize * 1.0);

        m_world.createScore(scoreX, scoreY, calculatedTileSize);

        // -----------------------------------------------------------
        // 4. ENTITEITEN PLAATSEN
        // -----------------------------------------------------------
        for (int row = 0; row < rows; ++row) {
            std::string& currentRow = mapData[row];
            for (int col = 0; col < currentRow.length(); ++col) {
                char tile = currentRow[col];

                // Bereken positie: Start + (index * size) + (half size voor center)
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
