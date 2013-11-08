#include "main.h"
#include "AppDelegate.hpp"
#include "service/ServiceLocator.hpp"
#include "platform/win/UrlOpener.hpp"
#include <cocos2d.h>

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	// set windows url opener
	ServiceLocator::get().setUrlOpener(new UrlOpener());

    // create the application instance
    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setViewName("SocialPointExam");
    eglView->setFrameSize(2048, 1536);
    // The resolution of ipad3 is very large. In general, PC's resolution is smaller than it.
    // So we need to invoke 'setFrameZoomFactor'(only valid on desktop(win32, mac, linux)) to make the window smaller.
    eglView->setFrameZoomFactor(0.4f);

    return CCApplication::sharedApplication()->run();
}
