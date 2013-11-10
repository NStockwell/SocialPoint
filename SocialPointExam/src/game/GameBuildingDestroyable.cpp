
#include "game/GameBuildingDestroyable.hpp"
#include "game/GameBuilding.hpp"
#include "game/GameBuildingView.hpp"

GameBuildingDestroyable::GameBuildingDestroyable(const std::string& name, int hitPoints, const TileSize& size):GameBuilding(name,size)
{
	_hitPoints = hitPoints;
	_destroyed = false;
}

GameBuildingDestroyable::~GameBuildingDestroyable()
{
}


bool GameBuildingDestroyable::onTouch()
{
	if(_destroyed)
		return true;
    //if(GameEntity::onTouch())
    {
      //  return true;
    }
    getMap().selectBuilding(*this);
    return true;
}


bool GameBuildingDestroyable::isDestroyed()
{
	return _destroyed;
}

	
bool GameBuildingDestroyable::attacked(int damage)
{
	if(_destroyed)
		return false;

	_hitPoints -= damage;
	if(_hitPoints <=0)
	{
		GameBuildingView *gbv = getBuildingView();
		gbv->setVisible(false);
		_hitPoints = 0;
		_destroyed = true;
		//setView(nullptr);
	}
	return true;
}