
#ifndef __GameBuildingView_FWD__
#define __GameBuildingView_FWD__

class GameBuildingView;

struct GameBuildingViewLayer
{
	enum Type
	{
		Base,
		Orientation,
		Hit,
		Overlay,
		End
	};
};

#endif /* defined(__GameBuildingView_FWD__) */