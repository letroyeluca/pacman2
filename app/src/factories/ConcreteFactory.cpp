

#include "factories/ConcreteFactory.h"
#include "Camera.h"
#include "logic/models/CoinModel.h"
#include "logic/models/GhostModel.h"
#include "logic/models/PacManModel.h"
#include "logic/models/ScoreModel.h"
#include "logic/models/WallModel.h"
#include "views/CoinView.h"
#include "views/GhostView.h"
#include "views/PacManView.h"
#include "views/ScoreView.h"
#include "views/WallView.h"

ConcreteFactory::ConcreteFactory(Camera& camera) : m_camera(camera) {
    if (!m_sharedTexture.loadFromFile("assets/sprite.png")) {
    }
}

std::shared_ptr<logic::PacManModel> ConcreteFactory::createPacMan(double x, double y, double width, double height) {
    auto model = std::make_shared<logic::PacManModel>(x, y, width, height);
    auto view = std::make_unique<PacManView>(model, m_camera);
    view->setRenderLayer(4);
    m_views.push_back(std::move(view));
    return model;
}

std::shared_ptr<logic::GhostModel> ConcreteFactory::createGhost(double x, double y, double width, double height,
                                                                char type) {
    auto model = std::make_shared<logic::GhostModel>(x, y, width, height, type);
    auto view = std::make_unique<GhostView>(model, m_camera);
    view->setRenderLayer(3);
    m_views.push_back(std::move(view));
    return model;
}

std::shared_ptr<logic::CoinModel> ConcreteFactory::createCoin(double x, double y, double width, double height) {
    auto model = std::make_shared<logic::CoinModel>(x, y, width, height);
    auto view = std::make_unique<CoinView>(model, m_camera, m_sharedTexture);
    view->setRenderLayer(2);
    m_views.push_back(std::move(view));
    return model;
}

std::shared_ptr<logic::WallModel> ConcreteFactory::createWall(double x, double y, double width, double height) {
    auto model = std::make_shared<logic::WallModel>(x, y, width, height);
    auto view = std::make_unique<WallView>(model, m_camera, m_sharedTexture);
    view->setRenderLayer(1);
    m_views.push_back(std::move(view));
    return model;
}

std::shared_ptr<logic::ScoreModel> ConcreteFactory::createScore(double x, double y, double size) {

    auto model = std::make_shared<logic::ScoreModel>(x, y, size);
    auto view = std::make_unique<ScoreView>(model, m_camera);
    view->setRenderLayer(5);
    m_views.push_back(std::move(view));
    return model;
}