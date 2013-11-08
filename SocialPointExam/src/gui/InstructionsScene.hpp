
#ifndef __InstructionsScene_H__
#define __InstructionsScene_H__

#include "gui/InstructionsScene.fwd.hpp"
#include <cocos2d.h>
#include <cocos-ext.h>
#include <functional>

/**
 * The scene that shows the instructions
 * @see cocos2d::CCScene
 */
class InstructionsScene : public cocos2d::CCScene,
public cocos2d::extension::CCBMemberVariableAssigner,
public cocos2d::extension::CCBSelectorResolver
{

private:

    /**
     * The text in the ccb
     * @see cocos2d::extension::CCBMemberVariableAssigner
     */
    cocos2d::CCLabelTTF* _text;
    
    /**
     * The link button in the ccb
     * @see cocos2d::extension::CCBMemberVariableAssigner
     */
    cocos2d::extension::CCControlButton* _link;
    std::string _linkUrl;

    /**
     * The scroll container in the ccb
     * @see cocos2d::extension::CCBMemberVariableAssigner
     */
    cocos2d::CCNode* _scrollContainer;

    /**
     * Exit selector called on ccb event
     * @see cocos2d::extension::CCBSelectorResolver
     */
    void onCloseClicked(cocos2d::CCObject* obj, cocos2d::extension::CCControlEvent ev);

    /**
     * Link selector called on ccb event
     * @see cocos2d::extension::CCBSelectorResolver
     */
    void onLinkClicked(cocos2d::CCObject* obj, cocos2d::extension::CCControlEvent ev);

public:

    /**
     * Constructor
     */
    InstructionsScene();

    /**
     * Destructor
     */
    virtual ~InstructionsScene();

    /**
     * Initialize the view
     */
    bool init();

    /**
     * Create a new autoreleased view
     * @see cocos2d::CCNode::create
     */
    static InstructionsScene* create();
    
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

#endif /* defined(__InstructionsScene_H__) */
