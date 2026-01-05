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
#include <iostream>

ConcreteFactory::ConcreteFactory(Camera& camera) : m_camera(camera) {
    if (!m_sharedTexture.loadFromFile("assets/sprite.png")) {
        std::cerr << "failed to load spritemap" << std::endl;
    }
}

std::shared_ptr<logic::PacManModel> ConcreteFactory::createPacMan(double x, double y, double width, double height) {
    //maak de model en view van pacman aan
    auto model = std::make_shared<logic::PacManModel>(x, y, width, height);
    auto view = std::make_shared<PacManView>(model, m_camera);

    //zet de juiste z layer zodat de pacman over de munten etc gaat
    view->setRenderLayer(4);

    //attach de observer na de constructie pas
    view->init();
    m_views.push_back(view);

    //maak de view voor de hoeveelheid lives
    auto livesView = std::make_shared<LivesView>(model, m_camera); // <-- make_shared

    //zet deze helemaal van boven
    livesView->setRenderLayer(100);

    // de oberserver attatchen na de constructie pas
    livesView->init();
    m_views.push_back(livesView);
    return model;
}

std::shared_ptr<logic::GhostModel> ConcreteFactory::createGhost(double x, double y, double width, double height,char type) {
    //maak de model en view aan
    auto model = std::make_shared<logic::GhostModel>(x, y, width, height, type);
    auto view = std::make_shared<GhostView>(model, m_camera);

    // zet de juiste z layer voor de juiste volgorden van wie boven wie komt
    view->setRenderLayer(3);

    //attatch de observer na de constructor
    view->init();

    m_views.push_back(view);
    return model;
}

std::shared_ptr<logic::CoinModel> ConcreteFactory::createCoin(double x, double y, double width, double height) {
    //maak de model en view ineens
    auto model = std::make_shared<logic::CoinModel>(x, y, width, height);
    auto view = std::make_shared<CoinView>(model, m_camera, m_sharedTexture);

    //juiste layer zetten zodat dit onder pacman komt
    view->setRenderLayer(2);

    //attatch de observer na de constructor
    view->init();

    m_views.push_back(view);
    return model;
}

std::shared_ptr<logic::AppleModel> ConcreteFactory::createApple(double x, double y, double width, double height) {
    //maak de model en de view aan van deze entity
    auto model = std::make_shared<logic::AppleModel>(x, y, width, height);
    auto view = std::make_shared<AppleView>(model, m_camera, m_sharedTexture);

    //zet de juiste render layer zodat dit boven de juiste entities komt te staan
    view->setRenderLayer(2);

    //attatch de observer na de constructor
    view->init();

    m_views.push_back(view);
    return model;
}

std::shared_ptr<logic::WallModel> ConcreteFactory::createWall(double x, double y, double width, double height) {
    //maak de model en de view aan van deze entity
    auto model = std::make_shared<logic::WallModel>(x, y, width, height);
    auto view = std::make_shared<WallView>(model, m_camera, m_sharedTexture);

    //zet de juiste render layer zodat dit boven de juiste entities komt te staan
    view->setRenderLayer(1);

    //attatch de observer na de constructor
    view->init();

    m_views.push_back(view);
    return model;
}

std::shared_ptr<logic::ScoreModel> ConcreteFactory::createScore(double x, double y, double size) {
    //maak de model en de view aan van deze entity
    auto model = std::make_shared<logic::ScoreModel>(x, y, size);
    auto view = std::make_shared<ScoreView>(model, m_camera);

    //zet de juiste render layer zodat dit boven de juiste entities komt te staan
    view->setRenderLayer(5);

    //attatch de observer na de constructor
    view->init();

    m_views.push_back(view);
    return model;
}