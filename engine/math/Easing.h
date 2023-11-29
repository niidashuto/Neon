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
public:
	void Standby() { startCount = std::chrono::steady_clock::now(); }

	float easeIn(float time, float startpos, float differencepos, float totalTime);

	// ���
	const float Lerp(const float start, const float end, const float time);
	const Vector3 Lerp3D(const Vector3& start, const Vector3& end, const float time);

public:
	// ���Ԍv��
	std::chrono::steady_clock::time_point startCount;	//�J�n����
	std::chrono::steady_clock::time_point nowCount;		//���ݎ���
	std::chrono::microseconds elapsedCount;

	float t;					//����
	float b;					//�J�n�ʒu
	float c;					//�J�n�ʒu-�I���ʒu�̍�
	float d;					//���v����
};