
#include "game/GameBuildingDestroyable.hpp"
#include "game/GameBuilding.hpp"

GameBuildingDestroyable::GameBuildingDestroyable(const std::string& name, int hitPoints, const TileSize& size):GameBuilding(name,size)
{
	_hitPoints = hitPoints;
	_destroyed = false;
}

GameBuildingDestroyable::~GameBuildingDestroyable()
{
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
		_hitPoints = 0;
		_destroyed = true;
	}
	return true;
}