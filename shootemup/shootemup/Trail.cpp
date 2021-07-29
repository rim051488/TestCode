#include "Trail.h"
#include"HomingShot.h"
#include <DxLib.h>

constexpr size_t history_limit = 16;

Trail::Trail(HomingShot& owner):owner_(owner)
{

}

void Trail::SetHandle(int handle)
{
	handle_ = handle;
}

void Trail::Updata()
{
	history_.push_front(owner_.pos);
	if (history_.size() > history_limit) {
		history_.pop_back();
	}
}

void Trail::Draw()
{
	auto lastpos = owner_.pos;
	float thickness = 5.0f;
	float idv=1.0f/static_cast<float>(history_.size());
	for (const auto& pos : history_)
	{
		DrawLineAA(lastpos.x, lastpos.y, pos.x, pos.y,
			0xffffff, thickness);
		auto v = pos - lastpos;
		v.Normalized();
		v = Vector2(v.y, v.x);

		//thickness *= 0.9f;
		lastpos = pos;
	}
}

void Trail::Clear()
{
	history_.clear();
}
