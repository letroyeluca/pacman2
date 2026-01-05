#include "factories/ConcreteFactory.h"
#include "Camera.h"
#include "logic/models/AppleModel.h"
#include "logic/models/CoinModel.h"
#include "logic/models/GhostModel.h"
#include "logic/models/PacManModel.h"
#include "logic/models/ScoreModel.h"
#include "logic/models/WallModel.h"
#include "views/AppleView.h"
#include "views/CoinView.h"
#include "views/GhostView.h"
#include "views/LivesView.h"
#include "views/PacManView.h"
#include "views/ScoreView.h"
#include "views/WallView.h"

ConcreteFactory::ConcreteFactory(Camera& camera) : m_camera(camera) {
    if (!m_sharedTexture.loadFromFile("assets/sprite.png")) {
        // Foutafhandeling indien nodig
    }
}

std::shared_ptr<logic::PacManModel> ConcreteFactory::createPacMan(double x, double y, double width, double height) {
    auto model = std::make_shared<logic::PacManModel>(x, y, width, height);

    // FIX 1: Gebruik make_shared i.p.v. make_unique
    auto view = std::make_shared<PacManView>(model, m_camera);
    view->setRenderLayer(4);

    // FIX 2: Init aanroepen (hier gebeurt de attach)
    view->init();
    m_views.push_back(view);

    // LivesView
    auto livesView = std::make_shared<LivesView>(model, m_camera); // <-- make_shared
    livesView->setRenderLayer(100);

    // FIX 3: Vergeet init() niet voor LivesView!
    livesView->init();

    m_views.push_back(livesView);

    return model;
}

std::shared_ptr<logic::GhostModel> ConcreteFactory::createGhost(double x, double y, double width, double height,
                                                                char type) {
    auto model = std::make_shared<logic::GhostModel>(x, y, width, height, type);

    // FIX: make_shared
    auto view = std::make_shared<GhostView>(model, m_camera);
    view->setRenderLayer(3);

    // Roep init aan (ook als GhostView het nu nog niet gebruikt, is het veilig voor de toekomst)
    view->init();

    m_views.push_back(view);
    return model;
}

std::shared_ptr<logic::CoinModel> ConcreteFactory::createCoin(double x, double y, double width, double height) {
    auto model = std::make_shared<logic::CoinModel>(x, y, width, height);

    // FIX: make_shared
    auto view = std::make_shared<CoinView>(model, m_camera, m_sharedTexture);
    view->setRenderLayer(2);
    view->init();

    m_views.push_back(view);
    return model;
}

std::shared_ptr<logic::AppleModel> ConcreteFactory::createApple(double x, double y, double width, double height) {
    auto model = std::make_shared<logic::AppleModel>(x, y, width, height);

    // FIX: make_shared
    auto view = std::make_shared<AppleView>(model, m_camera, m_sharedTexture);
    view->setRenderLayer(2);
    view->init();

    m_views.push_back(view);
    return model;
}

std::shared_ptr<logic::WallModel> ConcreteFactory::createWall(double x, double y, double width, double height) {
    auto model = std::make_shared<logic::WallModel>(x, y, width, height);

    // FIX: make_shared
    auto view = std::make_shared<WallView>(model, m_camera, m_sharedTexture);
    view->setRenderLayer(1);
    view->init();

    m_views.push_back(view);
    return model;
}

std::shared_ptr<logic::ScoreModel> ConcreteFactory::createScore(double x, double y, double size) {
    auto model = std::make_shared<logic::ScoreModel>(x, y, size);

    // FIX: make_shared (DIT was je specifieke crash bij ScoreView)
    auto view = std::make_shared<ScoreView>(model, m_camera);
    view->setRenderLayer(5);

    // Dit crashte omdat 'view' een unique_ptr was. Nu is het shared_ptr, dus veilig.
    view->init();

    m_views.push_back(view);
    return model;
}