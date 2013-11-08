
#include "game/GameUnit.hpp"
#include "game/GameUnitView.hpp"
#include "game/GameMap.hpp"

USING_NS_CC;

GameUnit::GameUnit(const std::string& name)
{
    _view = GameUnitView::create(*this, name);
    setView(_view);
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
