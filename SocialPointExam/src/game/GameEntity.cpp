
#include "game/GameEntity.hpp"
#include "game/GameEntityView.hpp"
#include "game/GameMap.hpp"
#include <cocos2d.h>

USING_NS_CC;

GameEntity::GameEntity() :
_view(nullptr)
{
}

GameEntity::~GameEntity()
{
    CC_SAFE_RELEASE(_view);
}

void GameEntity::setView(GameEntityView* view)
{
    if(_view)
    {
        _view->stop();
    }
    CC_SAFE_RELEASE(_view);
    _view = view;
    CC_SAFE_RETAIN(_view);
    if(_view)
    {
        _view->start();
    }
}

GameEntityView* GameEntity::getView()
{
    return _view;
}

GameMap& GameEntity::getMap()
{
    return *_map;
}

const GameMap& GameEntity::getMap() const
{
    return *_map;
}

void GameEntity::setMap(GameMap& map)
{
    if(_map != &map)
    {
        _map = &map;
    }
}

bool GameEntity::onTouch()
{
    return _map->onEntityTouched(*this);
}

void GameEntity::onTile(const Tile& tile)
{
    setZOrderForTile(tile);
}

void GameEntity::setZOrderForTile(const Tile &tile)
{
    Tile ftile(tile.x-getTileSize().width, tile.y);
    int z = _map->getZOrderForTile(ftile);
    if(_view->getParent())
    {
        _view->getParent()->reorderChild(_view, z);
    }
    else
    {
        _view->setZOrder(z);
    }
}

const GameEntity::TileSize& GameEntity::getTileSize()
{
    static TileSize size(1, 1);
    return size;
}

void GameEntity::setSelected(bool selected)
{
    _view->setSelected(selected);
}
