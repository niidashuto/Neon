#include "Easing.h"
#include <cmath>

Easing::Easing()
{
	startCount = std::chrono::steady_clock::now();
	nowCount = std::chrono::steady_clock::now();

	this->start = (float)WinApp::GetInstance()->window_width;
	this->end = 0.0f;
	this->maxTime = 1.0f;

	t = 0.0f;
	b = start;
	c = end - start;
	d = maxTime;
}

Easing::Easing(float start, float end, float maxTime)
{
	startCount = std::chrono::steady_clock::now();
	nowCount = std::chrono::steady_clock::now();

	this->start = start;
	this->end = end;
	this->maxTime = maxTime;

	t = 0.0f;
	b = start;
	c = end - start;
	d = maxTime;
}

float Easing::easeIn(float time, float startpos, float differencepos, float totalTime)
{
	//現在時間を取得する
	nowCount = std::chrono::steady_clock::now();
	//前回記録からの経過時間を取得する
	elapsedCount = std::chrono::duration_cast<std::chrono::microseconds>
		(nowCount - startCount);

	t = std::chrono::duration_cast<std::chrono::microseconds>
		(elapsedCount).count() / 1'000'000.0f;//マイクロ秒を秒に単位変換

	float x = min(time / totalTime, 1.0f);

	float v = x * x * x;

	float ret = differencepos * v + startpos;

	num_Y = ret;

	return num_Y;
}