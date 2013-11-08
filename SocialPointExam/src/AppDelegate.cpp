#include "AppDelegate.hpp"
#include "AppMacros.hpp"
#include "gui/MainScene.hpp"

#include <vector>
#include <string>


USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate() :
_rootController(nullptr)
{
}

AppDelegate::~AppDelegate() 
{
}

/**
 * This is the application entrance point
 */
bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
    
    CCSize frameSize = pEGLView->getFrameSize();
    
    ResolutionPolicy resolutionPolicy;
    
    float designRatio = designResolutionSize.width / designResolutionSize.height;
    float screenRatio = frameSize.width / frameSize.height;
    
    if(designRatio > screenRatio)
    {
        resolutionPolicy = kResolutionFixedWidth;
    }
    else
    {
        resolutionPolicy = kResolutionFixedHeight;
    }
    
    // Set the design resolution
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, resolutionPolicy);

	std::string resourcesRoot;

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    resourcesRoot = "..\\resources\\";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_LINUX
    resourcesRoot = "../../resources/";
#endif

    vector<string> searchPaths;
	searchPaths.push_back(resourcesRoot);

    // In this demo, we select resource according to the frame's height.
    // If the resource size is different from design resolution size, you need to set contentScaleFactor.
    // We use the ratio of resource's height to the height of design resolution,
    // this can make sure that the resource's height could fit for the height of design resolution.

    // if the frame's height is larger than the height of small resource size, select medium resource.
    if (frameSize.height > smallResource.size.height)
    {
        searchPaths.push_back(resourcesRoot+mediumResource.directory);
        
        pDirector->setContentScaleFactor(MIN(mediumResource.size.height/designResolutionSize.height, mediumResource.size.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium resource size, select small resource.
	else
    {
        searchPaths.push_back(resourcesRoot+smallResource.directory);

        pDirector->setContentScaleFactor(MIN(smallResource.size.height/designResolutionSize.height, smallResource.size.width/designResolutionSize.width));
    }
    
    // set searching path
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);

    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // start the game!
    CCDirector::sharedDirector()->pushScene(MainScene::create());

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
