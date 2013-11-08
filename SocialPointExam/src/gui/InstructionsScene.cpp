
#include "gui/InstructionsScene.hpp"
#include "service/ServiceLocator.hpp"
#include "service/IUrlOpener.hpp"

#include <fstream>

USING_NS_CC;
USING_NS_CC_EXT;

InstructionsScene::InstructionsScene() :
_text(nullptr),
_link(nullptr),
_scrollContainer(nullptr)
{
}

InstructionsScene::~InstructionsScene()
{
    CC_SAFE_RELEASE(_text);
    CC_SAFE_RELEASE(_link);
    CC_SAFE_RELEASE(_scrollContainer);
}

void InstructionsScene::onCloseClicked(CCObject* obj, CCControlEvent ev)
{
    CCDirector::sharedDirector()->popScene();
}

void InstructionsScene::onLinkClicked(CCObject* obj, CCControlEvent ev)
{
    ServiceLocator::get().getUrlOpener().open(_linkUrl);
}

bool InstructionsScene::init()
{
    CCNodeLoaderLibrary *nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    CCBReader ccbReader(nodeLoader);
    CCNode* base = ccbReader.readNodeGraphFromFile("CCB/InstructionsScene.ccbi", this, CCDirector::sharedDirector()->getVisibleSize());
    base->ignoreAnchorPointForPosition(true);
    addChild(base);
    setContentSize(base->getContentSize());
    setAnchorPoint(base->getAnchorPoint());
    
    CCBReader ccbReader2(nodeLoader);
    CCNode* scrollContent = ccbReader2.readNodeGraphFromFile("CCB/InstructionsScroll.ccbi", this, CCSize(_scrollContainer->getContentSize().width, 700));
    CCScrollView* scroll = CCScrollView::create(_scrollContainer->getContentSize(), scrollContent);
    scroll->setDirection(kCCScrollViewDirectionVertical);
    scroll->setContentOffset(CCPoint(0.0f, -scrollContent->getContentSize().height+_scrollContainer->getContentSize().height));
    _scrollContainer->addChild(scroll);
    
    std::string path = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/instructions.txt");
    std::ifstream in(path.c_str(), std::ios::in | std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize((unsigned)in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        _text->setString(contents.c_str());
    }
    
    _linkUrl = "http://www.cocos2d-x.org/wiki";
    
    _link->setTitleForState(CCString::create(_linkUrl), CCControlStateNormal);
    _link->setTitleForState(CCString::create(_linkUrl), CCControlStateSelected);
    _link->setTitleForState(CCString::create(_linkUrl), CCControlStateHighlighted);
    _link->setTitleForState(CCString::create(_linkUrl), CCControlStateDisabled);

    return true;
}

InstructionsScene* InstructionsScene::create()
{
    InstructionsScene* ret = new InstructionsScene();
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

bool InstructionsScene::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "scrollContainer", CCNode*, _scrollContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "text", CCLabelTTF*, _text);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "link", CCControlButton*, _link);
    return false;
}

SEL_MenuHandler InstructionsScene::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler InstructionsScene::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseClicked", InstructionsScene::onCloseClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onLinkClicked", InstructionsScene::onLinkClicked);
    return NULL;
}
