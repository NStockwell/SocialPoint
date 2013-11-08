#include "gui/MainScene.hpp"
#include "gui/InstructionsScene.hpp"
#include "game/GameScene.hpp"

USING_NS_CC;
USING_NS_CC_EXT;

MainScene::MainScene() :
_text(NULL)
{
}

MainScene::~MainScene()
{
    CC_SAFE_RELEASE(_text);
}

void MainScene::onPlayClicked(CCObject* obj, CCControlEvent ev)
{
    CCDirector::sharedDirector()->pushScene(GameScene::create());
}

void MainScene::onInstructionsClicked(CCObject* obj, CCControlEvent ev)
{
    CCDirector::sharedDirector()->pushScene(InstructionsScene::create());
}

bool MainScene::init()
{
    CCNodeLoaderLibrary *nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    CCBReader ccbReader(nodeLoader);
    CCNode* base = ccbReader.readNodeGraphFromFile("CCB/MainScene.ccbi", this, CCDirector::sharedDirector()->getVisibleSize());
    assert(base);
    base->ignoreAnchorPointForPosition(true);
    addChild(base);
    setContentSize(base->getContentSize());
    setAnchorPoint(base->getAnchorPoint());

    return true;
}

MainScene* MainScene::create()
{
    MainScene* ret = new MainScene();
    if(ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MainScene::onAssignCCBMemberVariable(CCObject* pTarget,const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "text", CCLabelTTF*, _text);
    return false;
}

SEL_MenuHandler MainScene::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler MainScene::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPlayClicked", MainScene::onPlayClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onInstructionsClicked", MainScene::onInstructionsClicked);
    return NULL;
}
