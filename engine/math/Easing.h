#pragma once
#include "WinApp.h"
#include <chrono>
#include "Vector3.h"

class Easing
{
public:
	float start;
	float end;
	float maxTime;
public:
	Easing();
	Easing(float start, float end, float maxTime);
	float num_Y = 0.0f;
	float num_X;
public:
	void Standby(bool reverse) { startCount = std::chrono::steady_clock::now(); }

	float easeIn(float time, float startpos, float differencepos, float totalTime);

	float ease_out_quint();

	// 補間
	const float Lerp(const float start, const float end, const float time);
	const Vector3 Lerp3D(const Vector3& start, const Vector3& end, const float time);

public:
	// 時間計測
	std::chrono::steady_clock::time_point startCount;	//開始時間
	std::chrono::steady_clock::time_point nowCount;		//現在時間
	std::chrono::microseconds elapsedCount;

	float t;					//時間
	float b;					//開始位置
	float c;					//開始位置-終了位置の差
	float d;					//合計時間

private:
	float timeNow;					//時間
	float startpos;					//開始位置
	float differencepos;			//開始位置-終了位置の差
	float totaltime;
};