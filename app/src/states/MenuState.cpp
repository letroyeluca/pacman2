//
// Created by Luca Letroye on 27/11/2025.
//

#include "states/MenuState.h"
#include "states/GameState.h"
#include <iostream>

MenuState::MenuState(StateManager& manager, sf::RenderWindow& window) : State(manager,window){
    if (!m_font.loadFromFile("assets/font.ttf")) {std::cerr << "Error loading font!" << std::endl;}

    m_title.setFont(m_font);
    m_title.setString("PACMAN");
    m_title.setCharacterSize(45);
    sf::FloatRect titleBounds = m_title.getLocalBounds();
    m_title.setOrigin(titleBounds.left + titleBounds.width / 2.0f,titleBounds.top + titleBounds.height / 2.0f);
    m_title.setPosition(window.getSize().x / 2.0f, window.getSize().y / 3.0f);

    m_instruction.setFont(m_font);
    m_instruction.setString("Press ENTER to Start");
    m_instruction.setCharacterSize(24);
    sf::FloatRect instBounds = m_instruction.getLocalBounds();
    m_instruction.setOrigin(instBounds.left + instBounds.width / 2.0f,instBounds.top + instBounds.height / 2.0f);
    m_instruction.setPosition(window.getSize().x / 2.0f, (window.getSize().y / 2.0f) + 100.0f);
}

void MenuState::handleInput(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter) {

            m_manager.pushState(std::make_unique<GameState>(m_manager,m_window));
        }
    }
}

void MenuState::update(float dt) {
}

void MenuState::render() {
    m_window.draw(m_title);
    m_window.draw(m_instruction);
}

void MenuState::handleResize(sf::Event::SizeEvent size) {
    m_window.setView(sf::View(sf::FloatRect(0.f, 0.f, size.width, size.height)));
}
