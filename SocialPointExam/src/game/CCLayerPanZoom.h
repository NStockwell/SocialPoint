
#ifndef __CCLayerPanZoom_H__
#define __CCLayerPanZoom_H__

#include <cocos2d.h>

#define NUM_INERTIA_SAMPLES 5

namespace cocos2d { namespace extension { 
    
    /**
     * This class implements the pan and zoom of the map
     */
    class CCLayerPanZoom : public cocos2d::CCLayer
    {
    protected:
        static const float _kDragInertiaDuration;
        
        // Double tap handling
        struct DoubleTapData
        {
            cocos2d::cc_timeval _lastTap;
            int _touchId;
        };
        
        DoubleTapData _doubleTapData;
        double _doubleTapInterval; // Double tap interval in seconds
        bool _doubleTapEnabled;
        
        // Min and Max scale
        float _minScale;
        float _maxScale;
        
        // Panning and Zooming
        bool _panningOrZooming;
        bool _panZoomEnabled;
        bool _dragging;
        
        // Inertia
        bool _dragInertia;
        cocos2d::CCPoint _dragInertiaDir;
        cocos2d::CCPoint _avgInertiaDir[NUM_INERTIA_SAMPLES];
        unsigned _avgIdx;
        cocos2d::cc_timeval _lastTouchMoved;
        float _dragInertiaCurrentTime;
        
        // Bounds
        cocos2d::CCRect _panBoundsRect;
        
        CCLayerPanZoom();
        void move(cocos2d::CCPoint &moveTo);
        void scale(const float &newScale, const cocos2d::CCPoint &scaleCenter);
        void setDoubleTap(bool enabled, double doubleTapInterval);
        virtual void onDoubleTap();
        
    public:
        virtual ~CCLayerPanZoom();
        
        CREATE_FUNC(CCLayerPanZoom);
        virtual bool init();
        
        void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
        void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
        void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
        
        virtual void update(float dt);
        
        void onEnter();
        void onExit();
        
        void enablePanZoom();
        void disablePanZoom();
        
        void setMaxScale(float maxScale);
        float getMaxScale();
        void setMinScale(float minScale);
        float getMinScale();
        
        void setPanBoundsRect(const cocos2d::CCRect& rect);
        cocos2d::CCRect getPanBoundRect();
        
        void setScale(float scale);
        void setPosition(const cocos2d::CCPoint& position);
        
        /**
         * Changes the scale factor on X axis of this node
         * The deafult value is 1.0 if you haven't changed it before
         * @param fScaleX   The scale factor on X axis.
         */
        virtual void setScaleX(float fScaleX);
        /**
         * Changes the scale factor on Y axis of this node
         * The Default value is 1.0 if you haven't changed it before.
         * @param fScaleY   The scale factor on Y axis.
         */
        virtual void setScaleY(float fScaleY);
        
    private:
        double timeValToSeconds(const cocos2d::cc_timeval& rTime) const;
    };
    
}}

#endif //_CCLAYERPANZOOM_H_