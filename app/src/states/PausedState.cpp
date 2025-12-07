//
// Created by Luca Letroye on 27/11/2025.
//

#include "states/PausedState.h"
#include <iostream>

PausedState::PausedState(StateManager &manager, sf::RenderWindow &window)
        : State(manager, window) {
    if (!m_font.loadFromFile("assets/font.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
    }
    m_title.setFont(m_font);
    m_title.setString("PAUSED");

    // Update de instructie tekst zodat de speler het weet
    m_instruction.setFont(m_font);
    m_instruction.setString("Press ESC to Quit\nPress ANY KEY to Resume"); // <--- AANGEPAST

    setupText();
}

void PausedState::setupText() {
    // ... (deze functie blijft hetzelfde als je had) ...
    m_title.setCharacterSize(m_window.getSize().x / 15);
    sf::FloatRect titleBounds = m_title.getLocalBounds();
    m_title.setOrigin(titleBounds.left + titleBounds.width / 2.0f,
                      titleBounds.top + titleBounds.height / 2.0f);
    m_title.setPosition(m_window.getSize().x / 2.0f, m_window.getSize().y / 3.0f);

    m_instruction.setCharacterSize(m_window.getSize().x / 40);
    sf::FloatRect instBounds = m_instruction.getLocalBounds();
    m_instruction.setOrigin(instBounds.left + instBounds.width / 2.0f,
                            instBounds.top + instBounds.height / 2.0f);
    float y_offset = m_window.getSize().y / 10.0f;
    m_instruction.setPosition(m_window.getSize().x / 2.0f,
                              (m_window.getSize().y / 2.0f) + y_offset);
}

void PausedState::render() {
    m_window.draw(m_title);
    m_window.draw(m_instruction);
}

void PausedState::update(float dt) {}

void PausedState::handleResize(sf::Event::SizeEvent size) {
    m_window.setView(sf::View(sf::FloatRect(0.f, 0.f, size.width, size.height)));
    setupText(); // Tekst opnieuw centreren na resize
}

// --- HIER ZIT DE BELANGRIJKE WIJZIGING ---
void PausedState::handleInput(sf::Event &event) {
    if (event.type == sf::Event::KeyPressed) {

        // Situatie 1: Speler drukt op ESCAPE -> Stoppen met spelen (Terug naar Menu)
        if (event.key.code == sf::Keyboard::Escape) {
            std::cout << "Quitting game, returning to Menu..." << std::endl;

            StateManager& manager = m_manager;
            manager.popState();
            manager.popState();


        }
        else {
            std::cout << "Resuming game..." << std::endl;
            m_manager.popState(); // Verwijder alleen PausedState -> Je landt terug in GameState
        }
    }
}