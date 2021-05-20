#include <DxLib.h>
#include<array>//stlを配列にしたもの
#include <iostream>
using namespace std;

#define SCENE_SIZE_X 640
#define SCENE_SIZE_Y 480

bool SysInit(void)
{
	// システム処理
	SetWindowText("2016025_永冨　心"); // ゲームウィンドウのタイトル

	SetGraphMode(SCENE_SIZE_X, SCENE_SIZE_Y, 32);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)
	{
		return false;
	}
	return true;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{

	if (!SysInit())
	{
		return -1;
	}
	
	int pos_x = 100;
	int pos_y = 100;
	int size = 50;

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		ClsDrawScreen();
		DrawBox(pos_x, pos_y, pos_x + size, pos_y + size, 0xffffff, true);
		ScreenFlip();
	}

	//delete shape1;
	return 0;
}