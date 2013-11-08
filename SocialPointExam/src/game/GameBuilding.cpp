
#include "game/GameBuilding.hpp"
#include "game/GameBuildingView.hpp"
#include "game/GameMapView.hpp"
#include "game/GameMap.hpp"

GameBuilding::GameBuilding(const std::string& name, const TileSize& size) :
_size(size)
{
    _view = GameBuildingView::create(*this, name);
    setView(_view);
}

GameBuilding::~GameBuilding()
{
}

const GameBuilding::TileSize& GameBuilding::getTileSize()
{
    return _size;
}

GameBuildingView* GameBuilding::getBuildingView()
{
    return _view;
}
