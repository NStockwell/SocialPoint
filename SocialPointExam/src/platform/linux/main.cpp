#include "AppDelegate.hpp"
#include "service/ServiceLocator.hpp"
#include "platform/linux/UrlOpener.h"
#include <cocos2d.h>
#include <cstdlib>
#include <cstdio>
#include <string>

USING_NS_CC;

int main(int argc, char **argv)
{
    AppDelegate app;

    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setFrameSize(800, 480);

    ServiceLocator::get().setUrlOpener(new UrlOpener());

    return CCApplication::sharedApplication()->run();
}
