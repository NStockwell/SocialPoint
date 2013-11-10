
#include "game/GameUnit.hpp"
#include "game/GameUnitView.hpp"
#include "game/GameMap.hpp"

USING_NS_CC;

GameUnit::GameUnit(const std::string& name, unsigned short hitPoints, unsigned short power, unsigned short range):
_hitPoints(hitPoints),
_power(power),
_range(range)
{
    _view = GameUnitView::create(*this, name);
    setView(_view);
	_attackingEntity = nullptr;
}

GameUnit::~GameUnit()
{
}

void GameUnit::moveTo(const Tile& tile)
{
    _view->setAnimation(GameUnitView::Animation::Move);
    const CCPoint& from = getView()->getPosition();
    CCPoint to = getMap().getTilePosition(tile);
    
    float angle = ccpToAngle(ccpSub(to, from));
    GameUnitView::Orientation ori = GameUnitView::orientationForAngle(angle);
    _view->setOrientation(ori);
    _view->moveTo(to);
}

void GameUnit::moveToAndAttack(const Tile& tile, const Tile& tileToAttack, GameEntity* attackingEntity)
{
    _view->setAnimation(GameUnitView::Animation::Move);
    const CCPoint& from = getView()->getPosition();
    CCPoint to = getMap().getTilePosition(tile);
	CCPoint toAttack = getMap().getTilePosition(tileToAttack);
    
    float angle = ccpToAngle(ccpSub(to, from));
    GameUnitView::Orientation ori = GameUnitView::orientationForAngle(angle);
    _view->setOrientation(ori);
    _view->moveToAndAttack(to, toAttack);
	_attackingEntity = attackingEntity;
}

bool GameUnit::attackEntity()
{
	if(!_attackingEntity)
		return false;
	if( !_attackingEntity->attacked(_power))
	{
		_attackingEntity = nullptr;
		_view->setAnimation(GameUnitView::Animation::Idle);
		return false;
	}
	return true;
}

void GameUnit::setAttackingEntity(GameEntity* att)
{
	_attackingEntity = att;
}

bool GameUnit::onTouch()
{
    if(GameEntity::onTouch())
    {
        return true;
    }
    getMap().addSelectedUnit(*this);
    return true;
}

void GameUnit::setOrientation(Orientation orientation)
{
    _view->setOrientation(orientation);
}
