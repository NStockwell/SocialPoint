
#ifndef __MainScene_H__
#define __MainScene_H__

#include "MainScene.fwd.hpp"
#include <cocos2d.h>
#include <cocos-ext.h>

/**
 * The initial view
 * @see cocos2d::CCNode
 */
class MainScene : public cocos2d::CCScene,
public cocos2d::extension::CCBMemberVariableAssigner,
public cocos2d::extension::CCBSelectorResolver
{
private:

    /**
     * Main text
     * @see cocos2d::extension::CCBMemberVariableAssigner
     */
    cocos2d::CCLabelTTF* _text;

    /**
     * Button selector called on ccb event
     * @see cocos2d::extension::CCBSelectorResolver
     */
    void onPlayClicked(cocos2d::CCObject* obj, cocos2d::extension::CCControlEvent ev);
    
    /**
     * Button selector called on ccb event
     * @see cocos2d::extension::CCBSelectorResolver
     */
    void onInstructionsClicked(cocos2d::CCObject* obj, cocos2d::extension::CCControlEvent ev);


public:

    /**
     * Constructor
     */
    MainScene();

    /**
     * Destructor
     */
    virtual ~MainScene();

    /**
     * Initialize the view
     */
    bool init();

    /**
     * Create a new autoreleased view
     * @see cocos2d::CCNode::create
     */
    static MainScene* create();

    /**
     * Called by the node loader to assign member variables
     * @see cocos2d::extension::CCBMemberVariableAssigner
     */
    bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

    /**
     * Called by the node loader to assign menu selectors
     * @see cocos2d::extension::CCBMemberVariableAssigner
     */
    cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);

    /**
     * Called by the node loader to assign control selectors
     * @see cocos2d::extension::CCBMemberVariableAssigner
     */
    cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);

};

#endif /* defined(__MainScene_H__) */
