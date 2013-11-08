#ifndef __GameUnit_FWD__
#define __GameUnit_FWD__

class GameUnit;

struct GameUnitOrientation
{
	enum Type {
		North,
		East,
		West,
		South,
		SouthEast,
		NorthEast,
		SouthWest,
		NorthWest
	};
};

#endif /* defined(__GameUnit_FWD__) */