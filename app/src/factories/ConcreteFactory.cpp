// factories/ConcreteFactory.cpp

#include "factories/ConcreteFactory.h"
#include "Camera.h"
#include "logic/models/CoinModel.h"
#include "logic/models/PacManModel.h"
#include "logic/models/ScoreModel.h"
#include "logic/models/WallModel.h"
#include "views/CoinView.h"
#include "views/PacManView.h"
#include "views/ScoreView.h"
#include "views/WallView.h"

ConcreteFactory::ConcreteFactory(Camera &camera) : m_camera(camera) {}

std::shared_ptr<logic::PacManModel>
ConcreteFactory::createPacMan(double x, double y, double width, double height) {
  // 1. Maak Model als SHARED
  auto model = std::make_shared<logic::PacManModel>(x, y, width, height);

  // 2. Maak View als UNIQUE (voor in de vector), geef model pointer mee
  auto view = std::make_unique<PacManView>(model, m_camera);

  m_views.push_back(std::move(view));
  return model;
}

std::shared_ptr<logic::CoinModel>
ConcreteFactory::createCoin(double x, double y, double width, double height) {
  // 1. Maak Model als SHARED
  auto model = std::make_shared<logic::CoinModel>(x, y, width, height);

  // 2. Maak View als UNIQUE
  // Let op: Geen '*' meer voor model, want de View verwacht nu een shared_ptr!
  auto view = std::make_unique<CoinView>(model, m_camera);

  m_views.push_back(std::move(view));
  return model;
}

std::shared_ptr<logic::WallModel>
ConcreteFactory::createWall(double x, double y, double width, double height) {
  // 1. Maak Model als SHARED
  auto model = std::make_shared<logic::WallModel>(x, y, width, height);

  // 2. Maak View als UNIQUE
  auto view = std::make_unique<WallView>(model, m_camera);

  m_views.push_back(std::move(view));
  return model;
}

// logic/models/ScoreModel.h moet constructor ScoreModel(x, y, size) hebben!
std::shared_ptr<logic::ScoreModel>
ConcreteFactory::createScore(double x, double y, double size) {
  // We geven de size mee als width/height aan het model
  auto model = std::make_shared<logic::ScoreModel>(x, y, size);
  auto view = std::make_unique<ScoreView>(model, m_camera);
  m_views.push_back(std::move(view));
  return model;
}