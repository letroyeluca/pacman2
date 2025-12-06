//
// Created by Luca Letroye on 4/11/2025.
//

#include "states/GameState.h"
#include "factories/ConcreteFactory.h"
#include "logic/models/PacManModel.h"
#include "states/PausedState.h"
#include <iostream>

// --- Constructor ---
GameState::GameState(StateManager &manager, sf::RenderWindow &window)
    : State(manager, window)
// m_score wordt automatisch geinitialiseerd op 0
{
  std::cout << "GameState initialiseren..." << std::endl;

  // 1. Maak de ScoreView aan (Visuals)

  // 2. Setup Camera & Factory
  m_camera =
      std::make_unique<Camera>(m_window.getSize().x, m_window.getSize().y);
  m_factory = std::make_unique<ConcreteFactory>(*m_camera);

  m_world = std::make_unique<logic::World>(m_factory.get());

  m_camera->setWorldDimensions(m_world->getWidth(), m_world->getHeight());
  m_views = m_factory->getCreatedViews();

  // Eenmalige resize triggeren om alles goed te zetten
  for (auto &view : m_views) {
    view->onWindowResize();
  }

  std::cout << "GameState setup compleet. Aantal views: " << m_views.size()
            << std::endl;
}

// --- Destructor (MOET bestaan voor unique_ptr) ---
GameState::~GameState() {}

// --- Handle Resize ---
void GameState::handleResize(sf::Event::SizeEvent size) {
  // 1. Update de SFML View
  m_manager.pushState(std::make_unique<PausedState>(m_manager, m_window));
  m_window.setView(sf::View(sf::FloatRect(0.f, 0.f, size.width, size.height)));

  // 2. Camera herberekenen
  m_camera->onWindowResize(size.width, size.height);

  // 3. Alle sprites updaten
  for (auto &view : m_views) {
    view->onWindowResize();
  }
}

// --- Handle Input ---
void GameState::handleInput(sf::Event &event) {
  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Escape) {
      m_manager.pushState(std::make_unique<PausedState>(m_manager, m_window));
    }

    auto pacman = m_world->getPacMan();
    if (pacman) {
      if (event.key.code == sf::Keyboard::Up)
        pacman->queueDirection(logic::Direction::UP);
      else if (event.key.code == sf::Keyboard::Down)
        pacman->queueDirection(logic::Direction::DOWN);
      else if (event.key.code == sf::Keyboard::Left)
        pacman->queueDirection(logic::Direction::LEFT);
      else if (event.key.code == sf::Keyboard::Right)
        pacman->queueDirection(logic::Direction::RIGHT);
    }
  }
}

// --- Update ---
void GameState::update(float dt) {
  m_world->update(dt);

  // 2. Animaties updaten
  for (auto &view : m_views) {
    view->updateAnimation(dt);
  }
}

// --- Render ---
void GameState::render() {
  m_window.clear(sf::Color::Black);
  for (const auto &view : m_views) {
    view->draw(m_window);
  }
}