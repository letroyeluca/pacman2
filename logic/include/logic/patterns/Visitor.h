//
// Created by Luca Letroye on 28/11/2025.
//

#ifndef PACMAN_VISITOR_H
#define PACMAN_VISITOR_H

namespace logic {
class PacManModel;
class CoinModel;
class WallModel;

class Visitor {
public:
  virtual ~Visitor() = default;
  virtual void visit(PacManModel &pacman) = 0;
  virtual void visit(CoinModel &coin) = 0;
  virtual void visit(WallModel &wall) = 0;
};
} // namespace logic

#endif // PACMAN_VISITOR_H
