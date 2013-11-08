
#ifndef __GameBuilding_FWD__
#define __GameBuilding_FWD__

class GameBuilding;

struct GameBuildingOrientation
{
	enum Type {
		North,
		East,
		West,
		South,
		SouthEast,
		NorthEast,
		SouthWest,
		NorthWest,
		End
	};
};

#endif /* defined(__GameBuilding_FWD__) */