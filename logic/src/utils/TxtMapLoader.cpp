//
// Created by Luca Letroye on 29/11/2025.
//
#include "logic/utils/TxtMapLoader.h"
#include <algorithm>
#include <fstream>
#include <iostream>

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
    if (!line.empty() && line.back() == '\r')
      line.pop_back();
    mapData.push_back(line);
    if (line.length() > maxWidth)
      maxWidth = line.length();
  }

  int cols = maxWidth;
  int rows = mapData.size();
  if (cols == 0 || rows == 0)
    return;

  // -----------------------------------------------------------
  // 2. NORMALISATIE BEREKENING (De Fix)
  // -----------------------------------------------------------

  // De maximale grootte in logic units is 2.0 (van -1 tot 1).
  // We moeten bepalen wat de beperkende factor is: breedte of hoogte?
  double maxLogicDimension = 2.0;

  // Als er meer kolommen zijn dan rijen, bepaalt de breedte de schaal.
  // Als er meer rijen zijn, bepaalt de hoogte de schaal.
  int maxGridDimension = std::max(cols, rows);

  // Bereken de grootte van 1 tegel zodat de grootste zijde precies 2.0 is.
  double calculatedTileSize = maxLogicDimension / maxGridDimension;

  // Nu rekenen we de daadwerkelijke breedte en hoogte van de wereld uit.
  // Eén van deze twee zal exact 2.0 zijn, de andere <= 2.0.
  double worldWidth = cols * calculatedTileSize;
  double worldHeight = rows * calculatedTileSize;

  // We centreren de wereld rond (0,0).
  // Start X is de linkerkant (-de helft van de breedte)
  // Start Y is de bovenkant (-de helft van de hoogte)
  double calcStartX = -(worldWidth / 2.0);
  double calcStartY = -(worldHeight / 2.0);

  // -----------------------------------------------------------
  // 3. WORLD UPDATEN
  // -----------------------------------------------------------

  // Geef de nieuwe dimensies door aan de World
  // (Zorg dat je deze setters hebt toegevoegd in World.h zoals eerder
  // besproken)
  m_world.setGridDimensions(calcStartX, calcStartY, calculatedTileSize);
  m_world.setWorldDimensions(worldWidth, worldHeight);

  // -----------------------------------------------------------
  // 4. ENTITEITEN PLAATSEN
  // -----------------------------------------------------------
  for (int row = 0; row < rows; ++row) {
    std::string &currentRow = mapData[row];
    for (int col = 0; col < currentRow.length(); ++col) {
      char tile = currentRow[col];

      // Bereken positie: Start + (index * size) + (half size voor center)
      double x =
          calcStartX + (col * calculatedTileSize) + (calculatedTileSize / 2.0);
      double y =
          calcStartY + (row * calculatedTileSize) + (calculatedTileSize / 2.0);

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
