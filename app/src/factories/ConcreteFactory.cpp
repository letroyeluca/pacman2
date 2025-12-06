#include "factories/ConcreteFactory.h"
#include "Camera.h"
#include "logic/models/CoinModel.h"
#include "logic/models/PacManModel.h"
#include "logic/models/WallModel.h"
#include "views/CoinView.h"
#include "views/PacManView.h"
#include "views/WallView.h"

ConcreteFactory::ConcreteFactory(Camera &camera) : m_camera(camera) {}

std::unique_ptr<logic::PacManModel>
ConcreteFactory::createPacMan(double x, double y, double width, double height) {
  auto model = std::make_unique<logic::PacManModel>(x, y, width, height);
  auto view = std::make_unique<PacManView>(*model, m_camera);
  m_views.push_back(std::move(view));

  return model;
}

std::unique_ptr<logic::CoinModel>
ConcreteFactory::createCoin(double x, double y, double width, double height) {
  auto model = std::make_unique<logic::CoinModel>(x, y, width, height);
  auto view = std::make_unique<CoinView>(*model, m_camera);
  m_views.push_back(std::move(view));

  return model;
}

std::unique_ptr<logic::WallModel>
ConcreteFactory::createWall(double x, double y, double width, double height) {
  auto model = std::make_unique<logic::WallModel>(x, y, width, height);
  auto view = std::make_unique<WallView>(*model, m_camera);
  m_views.push_back(std::move(view));

  return model;
}
/*
std::unique_ptr<ScoreView> ConcreteFactory::createScoreView(double x, double y)
{ auto model = std::make_unique<logic::Score>(); auto view =
std::make_unique<ScoreView>(*model, x, y); m_views.push_back(std::move(view));
}
 */