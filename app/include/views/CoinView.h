//
// Created by Luca Letroye on 17/11/2025.
//
#ifndef PACMAN_COINVIEW_H
#define PACMAN_COINVIEW_H
#pragma once
#include "views/EntityView.h"
#include <memory>

namespace logic {
class CoinModel;
}

class CoinView : public EntityView {
public:
    CoinView(std::shared_ptr<logic::CoinModel> model, Camera& camera);
    void draw(sf::RenderWindow& window) override;
    void onNotify(const logic::Subject& subject, logic::Event event) override;
    void onWindowResize() override;

private:
    sf::Texture m_texture;
    bool m_isHidden;
};
#endif // PACMAN_COINVIEW_H