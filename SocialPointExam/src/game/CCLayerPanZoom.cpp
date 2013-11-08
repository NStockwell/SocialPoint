#include "CCLayerPanZoom.h"

namespace cocos2d { namespace extension { 
    
    const float CCLayerPanZoom::_kDragInertiaDuration = 0.5f;
    
    CCLayerPanZoom::CCLayerPanZoom()
    {
        // Init DoubleTap timer
        CCTime::gettimeofdayCocos2d(&_doubleTapData._lastTap, NULL);
        _doubleTapData._touchId = -1;
    }
    
    CCLayerPanZoom::~CCLayerPanZoom()
    {
        
    }
    
    bool CCLayerPanZoom::init()
    {
        if(!CCLayer::init())
        {
            return false;
        }
        
        _avgIdx = 0;
        
        _panningOrZooming = false;
        _panZoomEnabled = true;
        _dragging = false;
        _dragInertiaCurrentTime = 0;
        
        _minScale = 0.5f;
        _maxScale = 2.0f;
        
        setAnchorPoint(ccp(0.0f, 0.0f));
        
        setTouchEnabled(true);
        
        setDoubleTap(true, 0.3);
        
        return true;
    }
    
    void CCLayerPanZoom::enablePanZoom()
    {
        _panZoomEnabled = true;
    }
    
    void CCLayerPanZoom::disablePanZoom()
    {
        _panZoomEnabled = false;
    }
    
    void CCLayerPanZoom::setMaxScale(float maxScale)
    {
        _maxScale = maxScale;
    }
    
    void CCLayerPanZoom::setMinScale(float minScale)
    {
        _minScale = minScale;
    }
    
    float CCLayerPanZoom::getMaxScale()
    {
        return _maxScale;
    }
    
    float CCLayerPanZoom::getMinScale()
    {
        return _minScale;
    }
    
    void CCLayerPanZoom::setPanBoundsRect(const cocos2d::CCRect& rect)
    {
        _panBoundsRect = rect;
        
        setScale(_minScale);
        setPosition(getPosition());
    }
    
    cocos2d::CCRect CCLayerPanZoom::getPanBoundRect()
    {
        return _panBoundsRect;
    }
    
    void CCLayerPanZoom::update(float dt)
    {
        if(_dragInertia)
        {
            _dragInertiaCurrentTime += dt;
            float amount = clampf(_dragInertiaCurrentTime / _kDragInertiaDuration, 0, 1);
            
            CCPoint currentDragInertia = ccpLerp( _dragInertiaDir, ccp(0,0), amount );
            
            move(currentDragInertia);
            
            if(_dragInertiaCurrentTime > _kDragInertiaDuration)
            {
                _dragInertia = false;
                _dragInertiaCurrentTime = 0;
            }
        }
    }
    
    void CCLayerPanZoom::onEnter()
    {
        CCLayer::onEnter();
        scheduleUpdate();
    }
    
    void CCLayerPanZoom::onExit()
    {
        CCLayer::onExit();
        unscheduleUpdate();
    }
    
    void CCLayerPanZoom::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
    {
        _dragInertiaDir = ccp(0,0);
        _panningOrZooming = false;
    }
    
    void CCLayerPanZoom::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
    {
        _panningOrZooming = true;
        
        if(_panZoomEnabled)
        {
            if(pTouches->count() == 1 && !_dragging)  // Drag the map
            {
                CCTouch* touch = (CCTouch*)(*pTouches->begin());
                CCPoint touchLocation = touch->getLocationInView();
                CCPoint prevLocation = touch->getPreviousLocationInView();
                touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
                prevLocation = CCDirector::sharedDirector()->convertToGL( prevLocation );
                
                CCPoint diff = ccpSub(touchLocation, prevLocation);
                
                move(diff);
                
                //add the diff into the avgInertia
                _avgInertiaDir[_avgIdx] = diff;
                
                ++_avgIdx;
                if(_avgIdx >= NUM_INERTIA_SAMPLES)
                    _avgIdx = 0;
                
                CCTime::gettimeofdayCocos2d(&_lastTouchMoved, NULL);
            }
            else if(pTouches->count() == 2 && _panZoomEnabled) // Pinch zoom in/out
            {
                // Get two touchess to handle the zoom
                CCSetIterator iter = pTouches->begin();
                CCTouch* touchOne = (CCTouch*)(*iter);
                ++iter;
                CCTouch* touchTwo = (CCTouch*)(*iter);
                
                // Get the touches and previous touches locations
                CCPoint touchLocationOne = touchOne->getLocation();
                CCPoint touchLocationTwo = touchTwo->getLocation();
                
                CCPoint previousLocationOne = touchOne->getPreviousLocation();
                CCPoint previousLocationTwo = touchTwo->getPreviousLocation();
                
                // Position the camera to the middle of the pinch
                // Get the middle position of the pinch
                CCPoint pinchCenter = ccpMidpoint(touchLocationOne, touchLocationTwo);
                
                // Calculate new scale
                float newScale = getScale() * ccpDistance(touchLocationOne, touchLocationTwo) / ccpDistance(previousLocationOne, previousLocationTwo);
                
                if(newScale < _minScale) newScale = _minScale;
                else if(newScale > _maxScale) newScale = _maxScale;
                
                // Call the scale method to scale by the distanceDelta from pinchCenter
                scale(newScale, pinchCenter);
                
                // You can move using two fingers (like in Google Maps)
                touchLocationOne = CCDirector::sharedDirector()->convertToGL(touchLocationOne);
                touchLocationTwo = CCDirector::sharedDirector()->convertToGL(touchLocationTwo);
                previousLocationOne = CCDirector::sharedDirector()->convertToGL(previousLocationOne);
                previousLocationTwo = CCDirector::sharedDirector()->convertToGL(previousLocationTwo);
                
                CCPoint touchLocation = ccpMidpoint(touchLocationOne, touchLocationTwo);
                CCPoint prevtouchLocation = ccpMidpoint(previousLocationOne, previousLocationTwo);
                
                CCPoint diff = ccpSub(touchLocation, prevtouchLocation);
                diff.y = -diff.y;
                move(diff);
            }
        }
    }
    
    void CCLayerPanZoom::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
    {
        _dragging = false;
        
        if(pTouches->count() !=  1)
        {
            return;
        }
        
        if(_panZoomEnabled || _doubleTapEnabled)
        {
            cc_timeval now;
            CCTime::gettimeofdayCocos2d(&now, NULL);
            double timeNow = timeValToSeconds(now);
            
            if(_panZoomEnabled)  // Add some inertia when we release the finger
            {
                double time = timeValToSeconds(_lastTouchMoved);
                
                //if the time between the last TouchMoved event plus 0.1 seconds is higher than the actual time we want drag inertia
                if((time + 0.1) > timeNow)
                {
                    _dragInertia = true;
                    _dragInertiaDir = ccp(0,0);
                    
                    for(unsigned i = 0; i < NUM_INERTIA_SAMPLES; ++i)
                    {
                        _dragInertiaDir = ccpAdd(_dragInertiaDir, _avgInertiaDir[i]);
                        _avgInertiaDir[i] = ccp(0,0);
                    }
                    
                    _dragInertiaDir = ccpMult(_dragInertiaDir, 1.0f / NUM_INERTIA_SAMPLES);
                    
                    _avgIdx = 0;
                }
            }
            
            if(_doubleTapEnabled)
            {
                double lastTapTime = timeValToSeconds(_doubleTapData._lastTap);
                CCTouch* pTouch = static_cast<CCTouch*>(pTouches->anyObject());
                
                if(timeNow - lastTapTime <= _doubleTapInterval && pTouch->getID() == _doubleTapData._touchId)
                {
                    onDoubleTap();
                }
                else
                {
                    _doubleTapData._touchId = pTouch->getID();
                    _doubleTapData._lastTap = now;
                }
            }
        }
    }
    
    void CCLayerPanZoom::move(cocos2d::CCPoint &moveTo)
    {
        CCPoint newPosition = ccpAdd(getPosition(), moveTo);
        setPosition(newPosition);
    }
    
    void CCLayerPanZoom::scale(const float &newScale, const CCPoint &scaleCenter)
    {
        // If you are doing a pinch zoom, this should be the center of your pinch
        CCPoint pinchCenter1 = convertToNodeSpace(scaleCenter);
        
        // Set the scale
        setScale(newScale);
        
        // Get the new center point
        CCPoint pinchCenter2 = convertToNodeSpace(scaleCenter);
        
        // Then calculate the delta
        CCPoint centerPointDelta  = ccpSub(pinchCenter2,pinchCenter1);
        centerPointDelta = ccpMult(centerPointDelta, newScale);
        
        // Now adjust your layer by the delta
        move(centerPointDelta);
    }
    
    void CCLayerPanZoom::setScale(float scale)
    {
        CCLayer::setScale(MIN(MAX(scale, _minScale), _maxScale));
    }
    
    void CCLayerPanZoom::setScaleX(float fScaleX)
    {
        fScaleX = (MIN(MAX(fScaleX, _minScale), _maxScale));
        
        CCPoint winOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
        CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint screenCenter = ccp(winOrigin.x + winSize.width * 0.5f, winOrigin.y + winSize.height * 0.5f);
        
        CCPoint centerBefore = convertToNodeSpace(screenCenter);
        
        CCNode::setScaleX(fScaleX);
        
        CCPoint centerAfter = convertToNodeSpace(screenCenter);
        
        CCPoint centerPointDelta  = ccpSub(centerAfter,centerBefore);
        centerPointDelta = ccpMult(centerPointDelta, fScaleX);
        
        move(centerPointDelta);
    }
    
    void CCLayerPanZoom::setScaleY(float fScaleY)
    {
        fScaleY = (MIN(MAX(fScaleY, _minScale), _maxScale));
        
        CCPoint winOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
        CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint screenCenter = ccp(winOrigin.x + winSize.width * 0.5f, winOrigin.y + winSize.height * 0.5f);
        
        CCPoint centerBefore = convertToNodeSpace(screenCenter);
        
        CCNode::setScaleY(fScaleY);
        
        CCPoint centerAfter = convertToNodeSpace(screenCenter);
        
        CCPoint centerPointDelta  = ccpSub(centerAfter,centerBefore);
        centerPointDelta = ccpMult(centerPointDelta, fScaleY);
        
        move(centerPointDelta);
    }
    
    void CCLayerPanZoom::setPosition(const cocos2d::CCPoint& position)
    {
        CCNode::setPosition(position);
        
        if(!_panBoundsRect.equals(CCRectZero))
        {
            CCSize size = CCDirector::sharedDirector()->getWinSize();
            
            if(_panBoundsRect.size.width * getScaleX() < size.width)
            {
                CCNode::setPosition(ccp((_panBoundsRect.origin.x + _panBoundsRect.size.width) * getScaleX() * 0.5f, (_panBoundsRect.origin.y + _panBoundsRect.size.height) * getScaleY() * 0.5f));
            }
            else
            {
                bool changed = false;
                CCPoint newPosition = getPosition();
                float value = -_panBoundsRect.origin.x * getScaleX();
                
                if(newPosition.x > value)
                {
                    newPosition.x = value;
                    changed = true;
                }
                
                value = ((-_panBoundsRect.origin.x - _panBoundsRect.size.width) * getScaleX()) + size.width;
                
                if(newPosition.x < value)
                {
                    newPosition.x = value;
                    changed = true;
                }
                
                value = -_panBoundsRect.origin.y * getScaleY();
                
                if(newPosition.y > value)
                {
                    newPosition.y = value;
                    changed = true;
                }
                
                value = ((-_panBoundsRect.origin.y - _panBoundsRect.size.height) * getScaleY()) + size.height;
                
                if(newPosition.y < value)
                {
                    newPosition.y = value;
                    changed = true;
                }
                
                if(changed)
                {
                    CCNode::setPosition(newPosition);
                }
            }
        }
    }
    
    void CCLayerPanZoom::setDoubleTap(bool enabled, double doubleTapInterval)
    {
        _doubleTapEnabled = enabled;
        _doubleTapInterval = doubleTapInterval;
    }
    
    void CCLayerPanZoom::onDoubleTap()
    {
        // Empty
    }
    
    double CCLayerPanZoom::timeValToSeconds(const cocos2d::cc_timeval& rTime) const
    {
        return (double)rTime.tv_sec + rTime.tv_usec * 0.000001;
    }
    
}}
