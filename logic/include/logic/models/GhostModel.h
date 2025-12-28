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
    void setSpeed(double newSpeed) { m_speed = newSpeed; }
    char getType() const { return m_type; }

    bool isLocked() const { return m_locked; }
    void setLocked(bool locked) { m_locked = locked; }
    void reset();
    void setSpawnDelay(float delay);

    void frighten(float duration);
    bool isFrightened() const { return m_isFrightened; }

    void die();                              // Mark for removal
    bool isDead() const { return m_isDead; } // Check status
    double getStartX() const { return m_startX; }
    double getStartY() const { return m_startY; }

private:
    Direction m_direction;
    Direction m_nextDirection;
    float m_speed;
    char m_type;
    bool m_locked = false;
    double m_startX;
    double m_startY;
    float m_spawnDelay = 0.0f; // Hoe lang moet hij wachten?
    float m_spawnTimer = 0.0f; // Huidige teller
    bool m_isActive = false;   // Mag hij al bewegen?
    bool m_isFrightened = false;
    float m_frightenedTimer = 0.0f;
    bool m_isDead = false;
    bool m_hasExitedHouse;
    bool m_hasWarnedEnding = false;

    // Het brein
    std::unique_ptr<GhostStrategy> m_strategy;
    std::unique_ptr<GhostStrategy> m_normalStrategy;
    std::unique_ptr<GhostStrategy> m_frightenedStrategy;
};

} // namespace logic

#endif // PACMAN_GHOSTMODEL_H