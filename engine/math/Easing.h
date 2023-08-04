#pragma once
#include "WinApp.h"
#include <chrono>

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
public:
	void Standby() { startCount = std::chrono::steady_clock::now(); }

	float easeIn(float time, float startpos, float differencepos, float totalTime);

public:
	// 時間計測
	std::chrono::steady_clock::time_point startCount;	//開始時間
	std::chrono::steady_clock::time_point nowCount;		//現在時間
	std::chrono::microseconds elapsedCount;

	float t;					//時間
	float b;					//開始位置
	float c;					//開始位置-終了位置の差
	float d;					//合計時間
};