
#include "game/GameMap.hpp"
#include "game/GameMapView.hpp"
#include "game/GameEntity.hpp"
#include "game/GameEntityView.hpp"
#include "game/GameUnit.hpp"
#include "game/GameBuilding.hpp"
#include <cocos2d.h>

USING_NS_CC;

GameMap::GameMap() :
_ignoreNextTouch(false), _view(GameMapView::create(*this)), _debug(false)
{
    CC_SAFE_RETAIN(_view);
    _view->start();
	_buildingList.clear();

}

GameMap::~GameMap()
{
    if(_view)
    {
        _view->stop();
    }
    CC_SAFE_RELEASE(_view);
    for(EntitySet::iterator itr = _entities.begin(); itr != _entities.end(); ++itr)
    {
        delete *itr;
    }
	_buildingList.clear();
}

cocos2d::CCNode* GameMap::getView()
{
    return _view;
}

cocos2d::CCPoint GameMap::getTilePosition(const Tile& tile) const
{
    return _view->getTilePosition(tile);
}

bool GameMap::getPositionTile(const cocos2d::CCPoint& point, Tile& tile) const
{
    return _view->getPositionTile(point, tile);
}

void GameMap::setEntityTiles(const Tile& tile, GameEntity* entity)
{
	TileMap::const_iterator itr=_tiles.begin();
    while(itr!=_tiles.end())
    {
        if(itr->second == entity)
        {
            itr = _tiles.erase(itr);
        }
		else
		{
			++itr;
		}
    }
    _entities.insert(entity);
    for(unsigned x=0; x<entity->getTileSize().width; x++)
    {
        for(unsigned y=0; y<entity->getTileSize().height; y++)
        {
            Tile t(tile.x-x, tile.y-y);
            TileMap::iterator itr = _tiles.find(t);
            _tiles.insert(itr, TileMap::value_type(t, entity));
        }
    }
    entity->onTile(tile);
}

bool GameMap::getFreeTile(const Tile& tile, Tile& free) const
{
    for(int i=0; i<GameMapView::kTileAmount; ++i)
    {
        for(int x=-i; x<=i; ++x)
        {
            for(int y=-i; y<=i; ++y)
            {
                free.x = tile.x + x;
                free.y = tile.y + y;
                TileMap::const_iterator itr = _tiles.find(free);
                if(itr == _tiles.end())
                {
                    return true;
                }
            }
        }
    }
    return false;
}


void GameMap::putEntity(const Tile& tile, GameEntity* entity)
{
    _view->addTileChild(tile, entity->getView());
    entity->setMap(*this);
    setEntityTiles(tile, entity);
}

void GameMap::watchForDestruction(GameEntity* entity)
{
	cocos2d::CCNotificationCenter::sharedNotificationCenter()->addObserver((cocos2d::CCObject*)this, callfuncO_selector(GameMap::onBuildingDestroyed), "DESTROYED", NULL);
	_buildingList.insert((GameBuildingDestroyable*)entity);
    
}

bool GameMap::onTouch(const Tile& tile)
{
    if(_ignoreNextTouch)
    {
        _ignoreNextTouch = false;
        return false;
    }
    _ignoreNextTouch = false;
    Tile free(0,0);
    for (UnitSet::iterator itr = _selectedUnits.begin(); itr != _selectedUnits.end(); ++itr)
    {
        if(getFreeTile(tile, free))
        {
            (*itr)->setSelected(false);
			
			if(_selectedBuilding)
			{
				(*itr)->moveToAndAttack(free, tile, _selectedBuilding);	
			}else
				(*itr)->moveTo(free);
            setEntityTiles(free, *itr);
        }

    }
    _selectedUnits.clear();
	_selectedBuilding=nullptr;
    return true;
}

bool GameMap::onEntityTouched(GameEntity& entity)
{
    if(_ignoreNextTouch)
    {
        return true;
    }
    _ignoreNextTouch = true;
    return false;
}

void GameMap::addSelectedUnit(GameUnit& unit)
{
    UnitSet::iterator itr = _selectedUnits.find(&unit);
    if(itr == _selectedUnits.end())
    {
        _selectedUnits.insert(&unit);
        unit.setSelected(true);
    }
    else
    {
        _selectedUnits.erase(itr);
        unit.setSelected(false);
    }
	//prefer selection of units than selection of buildings
	_selectedBuilding = nullptr;
}

void GameMap::selectBuilding(GameBuilding& building)
{
	_selectedBuilding = &building;
}

void GameMap::onBuildingDestroyed(cocos2d::CCObject* obj)
{
	//trying to remove when there is nothing there
	if(_buildingList.size() == 0)
		return;

	BuildingSet::const_iterator iter = _buildingList.find((GameBuildingDestroyable*)obj);
	if(iter != _buildingList.end())
	{
		_buildingList.erase(iter);
		if(_buildingList.size() == 0)
			cocos2d::CCDirector::sharedDirector()->popScene();
	}
}

void GameMap::toggleDebug()
{
    _debug = !_debug;
    for(EntitySet::iterator itr = _entities.begin(); itr != _entities.end(); ++itr)
    {
        (*itr)->getView()->setDebugDraw(_debug);
    }
    _view->setDebugDraw(_debug);
}

bool GameMap::isTileOccupied(const Tile& tile) const
{
    TileMap::const_iterator itr = _tiles.find(tile);
    return itr != _tiles.end();
}

GameEntity& GameMap::getTileEntity(const Tile& tile)
{
    TileMap::iterator itr = _tiles.find(tile);
    assert(itr != _tiles.end());
    return *(itr->second);
}

int GameMap::getZOrderForTile(const Tile &tile)
{
    return tile.x+tile.y;
}

GameMapTile::GameMapTile(unsigned x_, unsigned y_) :
x(x_), y(y_)
{
}

bool GameMapTile::operator<(const GameMapTile& tile) const
{
    if(x == tile.x)
    {
        return y < tile.y;
    }
    else
    {
        return x < tile.x;
    }
}

GameMapTileSize::GameMapTileSize(unsigned width_, unsigned height_) :
width(width_), height(height_)
{
}

bool GameMapTileSize::operator<(const GameMapTileSize& size) const
{
	if(width == size.width)
	{
		return height < size.height;
	}
	else
	{
		return width < size.width;
	}
}
