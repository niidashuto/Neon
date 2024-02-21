#include "Input.h"
#include <DirectXMath.h>

namespace MyMath {
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	const float PI = 3.141592654f;

	//ランダム生成
	int RandomMTInt(int min, int max);
	float RandomMTFloat(float min, float max);
}

