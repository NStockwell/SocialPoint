
#include "game/GameMap.hpp"
#include "game/GameMapView.hpp"
#include "game/CCLayerPanZoom.h"
#include <cocos-ext.h>

USING_NS_CC;
USING_NS_CC_EXT;

const cocos2d::CCPoint GameMapView::kTileOffset(495.0f, 933.0f);
const cocos2d::CCSize GameMapView::kTileSize(9.75f, 7.4f);
const unsigned GameMapView::kTileAmount = 40;
const int GameMapView::kTouchPriority = -1;

#pragma mark - initialization

GameMapView::GameMapView() :
_debugDraw(nullptr),
_debugDrawEnabled(false),
_touchTile(0,0),
_touched(false)
{
}

GameMapView::~GameMapView()
{
    CC_SAFE_RELEASE(_debugDraw);
}

bool GameMapView::init(GameMap& map)
{
    _map = &map;
    setContentSize(CCDirector::sharedDirector()->getVisibleSize());
    CCLayerPanZoom* panZoom = CCLayerPanZoom::create();
    panZoom->ignoreAnchorPointForPosition(true);
    panZoom->setContentSize(getContentSize());
    
    _background = CCSprite::create("bg-isle.png");
    panZoom->addChild(_background);
    panZoom->setPanBoundsRect(CCRect(
        _background->getContentSize().width/-2.0f,
        _background->getContentSize().height/-2.0f,
        _background->getContentSize().width,
        _background->getContentSize().height));
    panZoom->setMinScale(1.0f);
    panZoom->setScale(1.0f);
    addChild(panZoom);

	_touched = false;
    _debugDraw = CCDrawNode::create();
    CC_SAFE_RETAIN(_debugDraw);
    
    return true;
}

void GameMapView::updateDebugDraw()
{
    _debugDraw->clear();
    _debugDraw->removeFromParentAndCleanup(false);
    if(_debugDrawEnabled)
    {
		
        ccColor4F drawColor = ccc4f(1.0f, 0.0f, 0.0f, 1.0f);
        for(unsigned i = 0; i<kTileAmount; ++i)
        {
            for(unsigned j = 0; j<kTileAmount; ++j)
            {
                auto p = getTilePosition(Tile(i, j));
                _debugDraw->drawDot(p, 1.0f, drawColor);
            }
        }
        _background->addChild(_debugDraw);
    }
}

void GameMapView::setDebugDraw(bool enabled)
{
    if(_debugDrawEnabled != enabled)
    {
        _debugDrawEnabled = enabled;
        updateDebugDraw();
    }
}

void GameMapView::start()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kTouchPriority, false);
}

void GameMapView::stop()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

GameMapView* GameMapView::create(GameMap& map)
{
    GameMapView* ret = new GameMapView();
    if(ret && ret->init(map))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

#pragma mark - Tiles

cocos2d::CCPoint GameMapView::getTilePosition(const Tile& tile) const
{
    int x = (int)tile.x-tile.y;
    int y = -((int)tile.x+tile.y);
    
    return ccpAdd(kTileOffset, ccp(kTileSize.width*x, kTileSize.height*y));
}

bool GameMapView::getPositionTile(const CCPoint &p, Tile& tile) const
{
    for(unsigned i = 0; i<kTileAmount; ++i)
    {
        tile.x = i;
        for(unsigned j = 0; j<kTileAmount; ++j)
        {
            tile.y = j;
            CCPoint tp = getTilePosition(tile);
            if(ccpDistance(p, tp) < MIN(kTileSize.width, kTileSize.height))
            {
                return true;
            }
        }
    }
    return false;
}

void GameMapView::addTileChild(const Tile& tile, cocos2d::CCNode* child)
{
    child->setPosition(getTilePosition(tile));
    child->removeFromParentAndCleanup(false);
    _background->addChild(child);
}

bool GameMapView::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
    _touched = getPositionTile(_background->convertTouchToNodeSpace(touch), _touchTile);
	return _touched;
}

void GameMapView::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	if(_touched && !pTouch->getDelta().equals(CCPointZero))
	{
		_touched = false;
	}
}

void GameMapView::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(_touched)
    {
        _map->onTouch(_touchTile);
		_touched = false;
    }
}

