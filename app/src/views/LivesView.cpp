#include "views/LivesView.h"
#include "AssetManager.h"
#include <iostream>

LivesView::LivesView(std::shared_ptr<logic::PacManModel> model, Camera& camera)
    : EntityView(model, camera), // 2. PASS TO BASE: shared_ptr passed to EntityView (OK!)
      m_model(model)             // 3. STORE LOCALLY: shared_ptr converts to weak_ptr (OK!)
{
    // Laad de texture (gebruik dezelfde als je PacMan, of een specifiek icoon)
    if (!m_texture.loadFromFile("assets/sprite.png")) {
        std::cerr << "ERROR: Could not load pacman texture for UI" << std::endl;
    }

    auto& assets = Asset::AssetManager::getInstance();
    sf::FloatRect spriteBounds = m_sprite.getLocalBounds();
    m_sprite.setTexture(m_texture);
    m_sprite.setTextureRect(assets.getAnimationFrames(17, 1, 1)[0]); // Row 0
    m_sprite.setOrigin(25.0f, 25.0f);
    m_sprite.scale(0.8f, 0.8f);
}

void LivesView::updateAnimation(float dt) {
    // UI is meestal statisch, maar je zou de icoontjes kunnen laten happen
}

void LivesView::onWindowResize() {
    // UI hoeft vaak niet herrekend te worden als we DefaultView gebruiken,
    // maar je kunt hier posities resetten als je wilt schalen met het scherm.
}

void LivesView::draw(sf::RenderWindow& window) {
    if (auto model = m_model.lock()) {
        int lives = model->getLives();

        // 1. BEWAAR HUIDIGE VIEW (De game camera)
        sf::View oldView = window.getView();

        // 2. SCHAKEL NAAR UI VIEW (Scherm coordinaten, linksboven is 0,0)
        window.setView(window.getDefaultView());

        // 3. TEKEN DE LEVENS
        // We tekenen 'lives - 1' omdat de huidige PacMan in het spel is.
        // Maar in de arcade game tonen ze vaak het TOTAAL aantal levens of de RESERVES.
        // Laten we hier het totaal aantal levens tekenen (huidige + reserves).
        for (int i = 0; i < lives; ++i) {
            m_sprite.setPosition(m_startX + (i * m_spacing), m_startY);
            window.draw(m_sprite);
        }

        // 4. HERSTEL DE GAME VIEW
        window.setView(oldView);
    }
}