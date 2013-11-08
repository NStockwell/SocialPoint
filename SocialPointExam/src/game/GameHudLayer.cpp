
#include "game/GameHudLayer.hpp"
#include "game/GameMap.hpp"
#include <cocos-ext.h>

USING_NS_CC;
USING_NS_CC_EXT;

GameHudLayer::GameHudLayer()
{
}

GameHudLayer::~GameHudLayer()
{
}

void GameHudLayer::exitSelector(CCObject* obj, CCControlEvent ev)
{
    CCDirector::sharedDirector()->popScene();
}

void GameHudLayer::debugSelector(CCObject* obj, CCControlEvent ev)
{
    _map->toggleDebug();
}

const float kBorder = 10.0f;

bool GameHudLayer::init(GameMap& map)
{
    _map = &map;
    setContentSize(CCDirector::sharedDirector()->getVisibleSize());
    
    CCControlButton* close = CCControlButton::create();
    close->setAnchorPoint(CCPoint(1.0f, 1.0f));
    CCScale9Sprite* closeBg = CCScale9Sprite::create();
    closeBg->initWithFile("bt-close.png");
    closeBg->setCapInsets(CCRect(0.0f, 0.0f, 0.0f, 0.0f));
    close->setPreferredSize(closeBg->getContentSize());
    close->setContentSize(closeBg->getContentSize());
    close->setZoomOnTouchDown(false);
    close->setBackgroundSpriteForState(closeBg, CCControlStateNormal);
    close->setPosition(CCPoint(getContentSize().width-kBorder, getContentSize().height-kBorder));
    close->addTargetWithActionForControlEvents(this, cccontrol_selector(GameHudLayer::exitSelector), CCControlEventTouchUpInside);
    addChild(close);
    
    CCControlButton* debug = CCControlButton::create();
    debug->setAnchorPoint(CCPoint(1.0f, 0.0f));
    debug->setTitleForState(CCString::create("SHOW DEBUG"), CCControlStateNormal);
    debug->setZoomOnTouchDown(false);
    debug->setPosition(CCPoint(getContentSize().width-kBorder, kBorder));
    debug->addTargetWithActionForControlEvents(this, cccontrol_selector(GameHudLayer::debugSelector), CCControlEventTouchUpInside);
    addChild(debug);
    
    return true;
}

GameHudLayer* GameHudLayer::create(GameMap& map)
{
    GameHudLayer* ret = new GameHudLayer();
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
