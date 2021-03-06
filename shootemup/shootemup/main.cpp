#include<DxLib.h>
#include<cmath>
#include<memory>
#include"HomingShot.h"

///当たり判定関数
///@param posA Aの座標
///@param radiusA Aの半径
///@param posB Bの座標
///@param radiusB Bの半径
bool IsHit(const Position2& posA, float radiusA, const Position2& posB,  float radiusB) {
	//当たり判定を実装してください
	return false;
}

using namespace std;
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	SetMainWindowText("2016025_永冨心");
	if (DxLib_Init() != 0) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	//背景用
	int bgH[4];
	LoadDivGraph("img/bganim.png", 4, 4, 1, 256, 192, bgH);

	int skyH = LoadGraph("img/sky.png");
	int sky2H = LoadGraph("img/sky2.png");

	auto bulletH=LoadGraph("img/bullet.png");
	int playerH[10];
	LoadDivGraph("img/player.png", 10, 5, 2, 16, 24,playerH );

	int enemyH[2];
	LoadDivGraph("img/enemy.png", 2, 2, 1, 32, 32, enemyH);

	int arrowH = LoadGraph("img/arrow.png");

	//弾の半径
	float bulletRadius = 5.0f;

	//自機の半径
	float playerRadius = 10.0f;

	//適当に256個くらい作っとく
	Bullet bullets[256];

	HomingShot homingShots[16] = {};//プレイヤーのホーミング弾
	for (auto& shot : homingShots)
	{
		shot.trail.SetHandle(arrowH);
	}

	Position2 enemypos(320,25);//敵座標
	Position2 playerpos(320, 400);//自機座標

	unsigned int frame = 0;//フレーム管理用

	char keystate[256] = {};
	char lastkeystate[256] = {};
	bool isDebugMode = false;
	bool isRightHoming = false;
	int skyy = 0;
	int skyy2 = 0;
	int bgidx = 0;
	constexpr float homing_shot_speed = 10.0f;
	while (ProcessMessage() == 0) {
		ClearDrawScreen();

		GetHitKeyStateAll(keystate);

		isDebugMode = keystate[KEY_INPUT_P];

		//背景
		DrawExtendGraph(0, 0, 640, 480, bgH[bgidx / 8], false);
		bgidx = (bgidx + 1) % 32;

		//SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		skyy = (skyy + 1) % 480;
		skyy2 = (skyy2 + 2) % 480;
		DrawExtendGraph(0, skyy, 640, skyy + 480, skyH, true);
		DrawExtendGraph(0, skyy - 480, 640, skyy, skyH, true);
		DrawExtendGraph(0, skyy2, 640, skyy2 + 480, sky2H, true);
		DrawExtendGraph(0, skyy2 - 480, 640, skyy2, sky2H, true);


		//プレイヤー
		if (keystate[KEY_INPUT_RIGHT]) {
			playerpos.x = min(640,playerpos.x+4);
		}
		else if (keystate[KEY_INPUT_LEFT]) {
			playerpos.x = max(0,playerpos.x-4);
		}
		if (keystate[KEY_INPUT_UP]) {
			playerpos.y = max(0,playerpos.y-4);
		}else if (keystate[KEY_INPUT_DOWN]) {
			playerpos.y = min(480,playerpos.y+4);
		}
		if (keystate[KEY_INPUT_Z]&&!lastkeystate[KEY_INPUT_Z]) {
			//ホーミング弾発射
			DrawString(100, 100, "発射", 0x000000);
			int count = 0;
			for (auto& hs : homingShots) {
				if (!hs.isActive)
				{
					hs.isActive = true;
					hs.pos = playerpos;
					hs.vel = { count % 2 == 0 ? homing_shot_speed : -homing_shot_speed,4.0f };
					hs.vel.Normalized();
					//hs.vel *= homing_shot_speed;
					hs.trail.Clear();
					isRightHoming = !isRightHoming;
					if (++count > 1)
					{
						break;
					}
				}
			}
		}

		//プレイヤーのホーミング弾描画
		for (auto& hshot : homingShots) {
			if (!hshot.isActive)continue;
			if (frame % 2 == 0)
			{
				hshot.trail.Updata();
			}
			hshot.pos += hshot.vel;
			hshot.trail.Draw();
			//for (int i = 1; i < 5; ++i) {
			//	auto tailPos = hshot.pos - hshot.vel * static_cast<float>(i);
			//	auto thickness = static_cast<float>(6 - i);d
			//	DrawLineAA(hshot.pos.x, hshot.pos.y, tailPos.x, tailPos.y,
			//		0xff0000, thickness*4.0f);
			//}

			//hshot.vel = (hshot.vel +((enemypos - playerpos).Normalized()).Normalized() * homing_shot_speed);

			//hshot.vel = (hshot.vel + (enemypos - hshot.pos).Normalized()).Normalized() * homing_shot_speed;

			////敵へのベクトル、および今の速度ベクトルを正規化
			auto nVelocity = hshot.vel.Normalized();
			auto nToEnemy = (enemypos - hshot.pos).Normalized();
			auto dot = Dot(nVelocity, nToEnemy);//cosθが出る
			auto angle = acos(dot);//角度が出る(0を中心とした線対称
			angle = std::fminf(angle, DX_PI_F / 24.0f);
			float sign = Cross(nVelocity, nToEnemy) > 0.0f ? 1.0f : -1.0f;
			angle = atan2(hshot.vel.y, hshot.vel.x) + sign * angle;
			hshot.vel = Vector2(cos(angle), sin(angle)) * homing_shot_speed;

			DrawCircleAA(hshot.pos.x, hshot.pos.y,
				8.0f, 16, 0xff0000);

			//敵に当たった
			if ((enemypos - hshot.pos).SQMagnitude() < 900.0f) {
				hshot.isActive = false;
			}
			//弾を殺す
			if (hshot.pos.x + 16 < 0 || hshot.pos.x - 16 > 640 ||
				hshot.pos.y + 24 < 0 || hshot.pos.y - 24 > 480) {
				hshot.isActive = false;
			}
		}

		DrawCircleAA(enemypos.x, enemypos.y, 30.0f, 16, 0x00ff00, false, 3.0f);

		int pidx = (frame/4 % 2)*5+3;
		DrawRotaGraph(playerpos.x, playerpos.y, 2.0f, 0.0f, playerH[pidx], true);
		if (isDebugMode) {
			//自機の本体(当たり判定)
			DrawCircle(playerpos.x, playerpos.y, playerRadius, 0xffaaaa, false, 3);
		}

		//パフォーマンス計測
		//ドローコール
		auto dCallCnt = GetDrawCallCount();
		auto fps = GetFPS();
		//デバック表示
		DrawFormatString(10, 50, 0x000000, "DrawCall=%d", dCallCnt);
		DrawFormatString(10, 100, 0x000000, "fps=%f", fps);

		//弾発射
		if (frame % 12 == 0) {
			for (auto& b : bullets) {
				if (!b.isActive) {
					//b.pos = enemypos;
					//b.vel = Vector2(0, 5);
					//b.isActive = true;
					break;
				}
			}
		}

		//弾の更新および表示
		for (auto& b : bullets) {
			if (!b.isActive) {
				continue;
			}

			//弾の現在座標に弾の現在速度を加算してください
			
			float angle = 0.0f;
			//弾の角度をatan2で計算してください。angleに値を入れるんだよオゥ
			
			DrawRotaGraph(b.pos.x, b.pos.y,1.0f,angle, bulletH, true);
			
			if (isDebugMode) {
				//弾の本体(当たり判定)
				DrawCircle(b.pos.x, b.pos.y, bulletRadius, 0x0000ff, false, 3);
			}
			//弾を殺す
			if (b.pos.x + 16 < 0 || b.pos.x - 16 > 640 ||
				b.pos.y + 24 < 0 || b.pos.y - 24 > 480) {
				b.isActive = false;
			}

			//あたり！
			//↓のIsHitは実装を書いてません。自分で書いてください。
			if (IsHit(b.pos, bulletRadius, playerpos, playerRadius)) {
				//当たった反応を書いてください。
			}
		}

		//敵の表示
		enemypos.x = abs((int)((frame+320) % 1280) - 640);
		int eidx = (frame / 4 % 2);
		DrawRotaGraph(enemypos.x, enemypos.y, 2.0f, 0.0f, enemyH[eidx],true);

		if (isDebugMode) {
			//敵の本体(当たり判定)
			DrawCircle(enemypos.x, enemypos.y, 5, 0xffffff, false, 3);
		}
		++frame;
		ScreenFlip();
		std::copy(begin(keystate), end(keystate), begin(lastkeystate));
	}

	DxLib_End();

	return 0;
}