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
	// ���Ԍv��
	std::chrono::steady_clock::time_point startCount;	//�J�n����
	std::chrono::steady_clock::time_point nowCount;		//���ݎ���
	std::chrono::microseconds elapsedCount;

	float t;					//����
	float b;					//�J�n�ʒu
	float c;					//�J�n�ʒu-�I���ʒu�̍�
	float d;					//���v����
};