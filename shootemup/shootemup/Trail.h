#pragma once
#include "Geometry.h"
#include <list>
struct HomingShot;
class Trail
{
private:
	HomingShot& owner_;
	std::list<Position2> history_;//現在の座標
	int handle_;
public:
	Trail(HomingShot& owner);
	void SetHandle(int handle);
	void Updata();
	void Draw();
	void Clear();
};

