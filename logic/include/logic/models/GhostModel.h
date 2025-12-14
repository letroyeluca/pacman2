#ifndef PACMAN_GHOSTMODEL_H
#define PACMAN_GHOSTMODEL_H

#include "logic/models/EntityModel.h"
#include <memory>

namespace logic {

// Forward Declaration (zodat we Strategy hier kunnen noemen)
class GhostStrategy;
class World;
enum class Direction;

class GhostModel : public EntityModel {
public:
    GhostModel(double x, double y, double width, double height, char type);
    ~GhostModel() override; // Destructor nu niet meer default in .h (vanwege unique_ptr)

    void update(float dt) override;
    void accept(Visitor& visitor) override;

    // Movement
    void queueDirection(Direction dir);
    void commitDirection();
    void stop();
    void setPosition(double x, double y);

    // --- AI FUNCTIE ---
    // Laat het spook nadenken via zijn strategie
    void think(World& world);

    Direction getDirection() const { return m_direction; }
    Direction getNextDirection() const { return m_nextDirection; }
    float getSpeed() const { return m_speed; }
    char getType() const { return m_type; }

    bool isLocked() const { return m_locked; }
    void setLocked(bool locked) { m_locked = locked; }

private:
    Direction m_direction;
    Direction m_nextDirection;
    float m_speed;
    char m_type;
    bool m_locked = false;

    // Het brein
    std::unique_ptr<GhostStrategy> m_strategy;
};

} // namespace logic

#endif // PACMAN_GHOSTMODEL_H