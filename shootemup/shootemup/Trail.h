#pragma once
#include "Geometry.h"
#include <list>
struct HomingShot;
class Trail
{
private:
	HomingShot& owner_;
	std::list<Position2> history_;//åªç›ÇÃç¿ïW
public:
	Trail(HomingShot& owner);
	void Updata();
	void Draw();
	void Clear();
};

