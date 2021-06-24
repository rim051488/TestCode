#pragma once

// ベクトル構造体(2D)
struct Vector2
{
	float x, y;

	Vector2() :x(0.0f), y(0.0f) {};
	Vector2(float inx, float iny);

	//ベクトルの大きさを返す
	float Magnitude()const;		// 大きさ
	
	//ベクトルの大きさの２乗を返す
	float SQMagnitude()const;

	//ベクトルを正規化する
	void Normalize();

	//ベクトルを正規化したベクトルを返す
	Vector2 Normalized()const;

	// 90度回転
	void Rotate90();
	Vector2 Rotated90()const;
	static const Vector2 Zero() {
		return { 0.0f,0.0f };
	}

	//代入
	void operator=(const Vector2& rval);
	void operator+=(const Vector2& rval);
	void operator-=(const Vector2& rval);
	void operator*=(const float scale);
	void operator/=(const float div);
	bool operator==(const Vector2& rval) const;
};

Vector2 operator+(const Vector2& lval, const Vector2& rval);
Vector2 operator-(const Vector2& lval, const Vector2& rval);
Vector2 operator*(const Vector2& lval, const float scale);
Vector2 operator/(const Vector2& lval, const float div);

// 座標構造体=ベクトルと一緒(2D)
using Position2 = Vector2;

// 矩形構造体
struct Rect
{
	Position2 center;//矩形の中心
	float w, h;//幅の高さの半分
	float Left() const;//左
	float Top() const;//上
	float Right() const;//右
	float Bottom() const;//下
	float Width() const;//幅
	float Height() const;//高さ

};

// 円の構造体
struct Circle {
	Position2 center;//中心点
	float radius;//半径
};
