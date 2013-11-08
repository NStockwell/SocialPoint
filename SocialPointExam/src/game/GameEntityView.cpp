

#include "game/GameEntityView.hpp"
#include "game/GameEntity.hpp"
#include <iomanip>

USING_NS_CC;

const int GameEntityView::kTouchPriority = -2;
const float GameEntityView::kContentScale = 0.5f;
const float GameEntityView::kContentHeightOffset = 32.0f;
const float GameEntityView::kViewHeightOffset = 16.0f;
const char* GameEntityView::kSelectedShaderCacheKey = "selected_entity";

GameEntityView::GameEntityView() :
_touched(false), _debugDraw(nullptr), _content(nullptr),
_base(nullptr), _normalShader(nullptr), _selectedShader(nullptr)
{
}

GameEntityView::~GameEntityView()
{
    CC_SAFE_RELEASE(_debugDraw);
    CC_SAFE_RELEASE(_content);
    CC_SAFE_RELEASE(_base);
    CC_SAFE_RELEASE(_normalShader);
    CC_SAFE_RELEASE(_selectedShader);
}

GameEntityView* GameEntityView::create(GameEntity& entity)
{
    GameEntityView* ret = new GameEntityView();
    if(ret && ret->init(entity))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GameEntityView::init(GameEntity& entity)
{
    if(!CCNode::init())
    {
        return false;
    }
    
    _content = CCNode::create();
    _content->ignoreAnchorPointForPosition(true);
    _content->setScale(kContentScale);
    CC_SAFE_RETAIN(_content);
    addChild(_content);
    
    _debugDraw = CCDrawNode::create();
    _debugDrawEnabled = false;
    CC_SAFE_RETAIN(_debugDraw);
    addChild(_debugDraw);
    
    _selectedShader = CCShaderCache::sharedShaderCache()->programForKey(kSelectedShaderCacheKey);
    if(_selectedShader)
    {
        _selectedShader->retain();
    }
    else
    {
        _selectedShader = new CCGLProgram();
        std::string path = CCFileUtils::sharedFileUtils()->fullPathForFilename("data/SelectedShader.fsh");
        const GLchar * fragSource = (GLchar*)CCString::createWithContentsOfFile(path.c_str())->getCString();
        _selectedShader->initWithVertexShaderByteArray(ccPositionTextureColor_vert, fragSource);
        _selectedShader->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
        _selectedShader->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
        _selectedShader->link();
        _selectedShader->updateUniforms();
        _selectedShader->setUniformLocationWith3f(_selectedShader->getUniformLocationForName("u_borderColor"), 0.0f, 0.8f, 0.0f);
        _selectedShader->setUniformLocationWith1f(_selectedShader->getUniformLocationForName("u_borderAlpha"), 1.0f);
        CCShaderCache::sharedShaderCache()->addProgram(_selectedShader, kSelectedShaderCacheKey);
    }
    
    _entity = &entity;
    _touched = false;
    return true;
}

void GameEntityView::start()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kTouchPriority, false);
}

void GameEntityView::stop()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool GameEntityView::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
    CCPoint p = convertTouchToNodeSpace(touch);
    CCRect rect(0.0f, 0.0f, getContentSize().width, getContentSize().height);
    _touched = rect.containsPoint(p);
	return _touched;
}

void GameEntityView::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	if(_touched && !pTouch->getDelta().equals(CCPointZero))
	{
		_touched = false;
	}
}

void GameEntityView::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(_touched)
    {
        _entity->onTouch();
		_touched = false;
    }
}

void GameEntityView::updateDebugDraw()
{
    _debugDraw->clear();
    _debugDraw->removeFromParentAndCleanup(false);
    if(_debugDrawEnabled)
    {
        _debugDraw->ignoreAnchorPointForPosition(true);
        _debugDraw->drawDot(getAnchorPointInPoints(), 1.0f, ccc4f(0.0f, 0.0f, 1.0f, 1.0f));
        const CCSize& size = getContentSize();
        CCPoint verts[4] = {
            CCPoint(0.0f, 0.0f),
            CCPoint(size.width, 0.0f),
            CCPoint(size.width, size.height),
            CCPoint(0.0f, size.height)
        };
        _debugDraw->drawPolygon(verts, 4, ccc4f(0.0f, 0.0f, 0.0f, 0.0f), 0.5f, ccc4f(0.0f, 0.0f, 1.0f, 1.0f));
        addChild(_debugDraw);
    }
}

void GameEntityView::setDebugDraw(bool enabled)
{
    if(_debugDrawEnabled != enabled)
    {
        _debugDrawEnabled = enabled;
        updateDebugDraw();
    }
}

void GameEntityView::setEntityViewBase(cocos2d::CCNode* base, int zorder)
{
    CC_SAFE_RETAIN(base);
    CC_SAFE_RELEASE(_base);
    _base = base;
    setEntityView(base, zorder);
    CCSize size(base->getContentSize().width*kContentScale,
                (base->getContentSize().height-kViewHeightOffset)*kContentScale);
    setContentSize(size);
    setAnchorPoint(CCPoint(base->getAnchorPoint().x, kViewHeightOffset/base->getContentSize().height));
    _content->setPosition(getAnchorPointInPoints());
}

void GameEntityView::setEntityView(cocos2d::CCNode* layer, int zorder)
{
    layer->setAnchorPoint(CCPoint(0.5f, kContentHeightOffset/layer->getContentSize().height));
    _content->addChild(layer, zorder);
}

cocos2d::CCAnimation* GameEntityView::createAnimation(const std::string& prefix, float frameRate)
{
    unsigned i = 1;
    CCAnimation* animation = nullptr;
    while(true)
    {
        std::stringstream name;
        name << prefix << "_" << std::setfill('0') << std::setw(2) << i;
        CCSpriteFrame* spriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name.str().c_str());
        if(!spriteFrame)
        {
            break;
        }
        if(!animation)
        {
            animation = CCAnimation::createWithSpriteFrames(nullptr, frameRate);
        }
        animation->addSpriteFrame(spriteFrame);
        ++i;
    }
    return animation;
}

void GameEntityView::setSelected(bool selected)
{
    if(!_base)
    {
        return;
    }
    if(!_normalShader)
    {
        _normalShader = _base->getShaderProgram();
        CC_SAFE_RETAIN(_normalShader);
    }
    if(selected)
    {
        _base->setShaderProgram(_selectedShader);
        _content->runAction(CCSequence::create(
            CCScaleBy::create(0.2f, 1.2f),
            CCScaleBy::create(0.2f, 1.0f/1.2f),
            nullptr));
    }
    else
    {
        _base->setShaderProgram(_normalShader);
    }
}
