#include <DxLib.h>
#include<array>//stlを配列にしたもの
#include <iostream>
#include <math.h>
#include "Geometry.h"
using namespace std;

#define SCENE_SIZE_X 640
#define SCENE_SIZE_Y 480


Position2 GetMousePosition2()
{
	int mx, my;
	GetMousePoint(&mx, &my);
	return  { static_cast<float>(mx),static_cast<float>(my) };
}

float DegreeToRadian(float deg)
{
	return deg * DX_PI / 180.0f;
}

bool Isreverse = false;
int lasMouseInput = 0;
int frameForAngle = 0;


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// システム処理
	SetWindowText(L"2016025_永冨　心"); // ゲームウィンドウのタイトル

	SetGraphMode(SCENE_SIZE_X, SCENE_SIZE_Y, 32);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)
	{
		return false;
	}

	Circle cA = { {100,100},50 };
	Circle cB = { {400,300},50 };
	char keystate[256];
	int ground = LoadGraph(L"./Image/ground.png", false);
	int bgAssetH = LoadGraph(L"./Image/Assets.png", false);


	unsigned int frameNo = 0;

	int lastMouseInput = 0;
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		auto currentMouseInput = GetMouseInput();
		if ((currentMouseInput & MOUSE_INPUT_LEFT) &&
			!(lastMouseInput & MOUSE_INPUT_LEFT))
		{
			Isreverse = !Isreverse;
		}
		lastMouseInput = currentMouseInput;

		int arrW, arrH;
		GetGraphSize(arrowH,&arrW,&arrH);
		constexpr size_t block_size = 16;
		constexpr size_t Width = 1000;
		auto count = Width / block_size;
		constexpr int base_y = 240;
		constexpr float sin_amp = block_size*2;
		int x = 0;
		int y = 240;
		Position2 groundPos(x, y);
		Position2 currentPos(x, y);
		//Vector2 lastDelta90Vec = Vector2::Zero(); // 直前のベクトル
		Vector2 lastDelta90Vectors[2] = { Vector2::Zero() ,Vector2::Zero() }; // 直前のベクトル
		for (int i = 0; i < count; ++i)
		{
			int nextX = i * block_size;
			int nextY = sin_amp * sin((float)(0.5f * nextX) / 180.0f * DX_PI);

			auto deltaVec = Vector2(block_size, nextY).Normalized() * block_size;
			auto nextPos = currentPos + deltaVec;

			auto middleVec0 = deltaVec;
			auto middleVecR = deltaVec.Rotated90();
			if (!(lastDelta90Vectors[0] == Vector2::Zero())) {
				middleVecR = (middleVecR + lastDelta90Vectors[0]).Normalized() * block_size;
			}
			auto middleVecL = lastDelta90Vectors[0];
			lastDelta90Vectors[0] = deltaVec.Rotated90();
			if (!(lastDelta90Vectors[1] == Vector2::Zero())) {
				middleVecL = (middleVecL + lastDelta90Vectors[1]).Normalized() * block_size;
			}
			lastDelta90Vectors[1] = lastDelta90Vectors[0];
			lastDelta90Vectors[0] = deltaVec.Rotated90();

			auto middlePosL = currentPos + middleVecL;
			auto middlePosR = nextPos + middleVecR;

			DrawRectModiGraph(currentPos.x, currentPos.y, // 左上
				nextPos.x, nextPos.y,//右上
				middlePosR.x, middlePosR.y,//右下(右上から真下に落としている)
				middlePosL.x, middlePosL.y,//左下(左上から真下に落としている)
				48, 0,//元画像の左上
				16, 16,//元画像の切り抜きの幅、切り抜き高さ
				ground,
				true
			);
			//四角形表示
			//DrawLineAA(x, y,//始点
			//	nextX, nextY,//終点
			//	0xffffff, 2.0f);
			//DrawLineAA(x, y,//始点
			//	nextX, nextY + block_size,//終点
			//	0xffffff, 2.0f);
			//DrawLineAA(nextX, nextY + block_size,//始点
			//	 x, y + block_size,//終点
			//	0xffffff, 2.0f);
			//DrawLineAA(x, y + block_size,//始点
			//	nextX, nextY,//終点
			//	0xffffff, 2.0f);

			x = nextX;
			y = nextY;
		}

		// 現在のマウス座標を取得
		auto mpos = GetMousePosition2();
		Vector2 dir = mpos- cA.center;
		//基準（0。）右ベクトルで、マウスの向きを角度で返す
		float angle = atan2(dir.y, dir.x);
		auto imgIdx = frameNo / frames_per_pict;
		int flipX = 16;
		if (Isreverse)
		{
			int w, h;
			GetGraphSize(graphH[imgIdx], &w, &h);
		}
		DrawRotaGraph2(cA.center.x, cA.center.y,	//画像の表示位置
			16,35, //上の画像表示位置に、画像自体のこの座標があうように表示
			4.0f, angle, graphH[frameNo/frames_per_pict],true, Isreverse);

	}

	//delete shape1;
	return 0;
}