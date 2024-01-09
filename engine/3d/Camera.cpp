#include "Camera.h"
#include "WinApp.h"
#include "MyMath.h"


using namespace DirectX;
Camera::Camera()
{
	// ビュー行列の生成
	UpdateViewMatrix();

	// 透視投影による射影行列の生成
	UpdateProjectionMatrix();

	matViewProjection_ = matView_ * matProjection_;

}

Camera::~Camera()
{
}

void Camera::Update()
{
	// ビュー行列の生成
	UpdateViewMatrix();

	// 透視投影による射影行列の生成
	UpdateProjectionMatrix();

	matViewProjection_ = matView_ * matProjection_;
}

void Camera::UpdateViewMatrix()
{
	// ビュー行列の更新
	matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
	//視点座標
	XMVECTOR eyePosition = XMLoadFloat3(&eye_);
	//注視点座標
	XMVECTOR targetPosition = XMLoadFloat3(&target_);
	//(仮の)上方向
	XMVECTOR upVector = XMLoadFloat3(&up_);

	//カメラZ軸(視線方向)
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
	//0ベクトルだと向きが定まらないので除外
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));

	//ベクトルを正規化
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	//カメラのX軸(右方向)
	XMVECTOR cameraAxisX;
	//X軸は上方向→Z軸の外積で決まる
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	//ベクトルを正規化
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	//カメラのY軸(上方向)
	XMVECTOR cameraAxisY;
	//Y軸はZ軸→X軸の外積で決まる
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	//カメラ回転行列
	XMMATRIX matCameraRot;
	//カメラ座標系→ワールド座標系の変換行列
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	//転置により逆行列(逆回転)を計算
	matView_ = XMMatrixTranspose(matCameraRot);

	//視点座標に-1を掛けた座標
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	//カメラの位置からワールド視点へのベクトル(カメラ座標系)
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);	//X成分
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);	//Y成分
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);	//Z成分
	//一つのベクトルにまとめる
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

	//ビュー行列に平行移動成分を設定
	matView_.r[3] = translation;

#pragma region 全方向ビルボード行列の計算
	//ビルボード行列
	matBillboard_.r[0] = cameraAxisX;
	matBillboard_.r[1] = cameraAxisY;
	matBillboard_.r[2] = cameraAxisZ;
	matBillboard_.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
#pragma endregion

#pragma region Y軸回りビルボード行列の計算
	//カメラX,Y,Z軸
	XMVECTOR yBillCameraAxisX, yBillCameraAxisY, yBillCameraAxisZ;

	//X軸は共通
	yBillCameraAxisX = cameraAxisX;
	//Y軸はワールド座標系のY軸
	yBillCameraAxisY = XMVector3Normalize(upVector);
	//Z軸はX軸→Y軸の外積で決まる
	yBillCameraAxisZ = XMVector3Cross(yBillCameraAxisX, yBillCameraAxisY);

	//Y軸回りビルボード行列
	matBillboardY_.r[0] = yBillCameraAxisX;
	matBillboardY_.r[1] = yBillCameraAxisY;
	matBillboardY_.r[2] = yBillCameraAxisZ;
	matBillboardY_.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

#pragma endregion

}
void Camera::UpdateProjectionMatrix()
{
	matProjection_ = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),
		(float)WinApp::window_width / WinApp::window_height,
		0.1f, 1000.0f
	);
}
void Camera::CameraMoveVector(const XMFLOAT3& move)
{
	XMFLOAT3 eye_moved = eye_;
	XMFLOAT3 target_moved = target_;

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	target_moved.x += move.x;
	target_moved.y += move.y;
	target_moved.z += move.z;

	SetEye(eye_moved);
	SetTarget(target_moved);
}

void Camera::CameraMoveVectorEye(const XMFLOAT3& move)
{
	XMFLOAT3 eye_moved = eye_;

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	SetEye(eye_moved);
}

void Camera::CameraMoveVectorTarget(const XMFLOAT3& move)
{
	XMFLOAT3 target_moved = target_;

	target_moved.x += move.x;
	target_moved.y += move.y;
	target_moved.z += move.z;

	SetTarget(target_moved);
}

void Camera::Reset()
{
	////追従対象がいれば
	//if (player_)
	//{
	//	//追従座標・角度の初期化
	//	interTarget_ = player_->GetWorldPosition();
	//	
	//}

	//XMFLOAT3 offset=
}

XMFLOAT3 Camera::ShakeEye(XMFLOAT3 eye, int count, XMFLOAT3 min, XMFLOAT3 max)
{
	const XMFLOAT3 nowEye = eye;
	//プランA
	// 視点座標
	for (int i = 0; i < count; i++)
	{
		//if (i > count - 1)eye_ = { 0.0f, 6.0f, -115.0f };
		eye_ = { MyMath::RandomMTFloat(min.x,max.x), MyMath::RandomMTFloat(min.y,max.y), MyMath::RandomMTFloat(min.z,max.z) };
	}

	/*
	//プランB
	// 視点座標
	eye_ = { 0.0f, 81.0f, -115.0f };
	// 注視点座標
	target_ = { 0.0f,-35.0f,0.0f };
	*/

	// ビュー行列の生成
	UpdateViewMatrix();

	// 透視投影による射影行列の生成
	UpdateProjectionMatrix();

	matViewProjection_ = matView_ * matProjection_;


	return nowEye;
}

XMFLOAT3 Camera::ShakeTarget(XMFLOAT3 target, int count, XMFLOAT3 min, XMFLOAT3 max)
{
	const XMFLOAT3 nowTarget = target;
	//プランA
	// 視点座標
	for (int i = 0; i < count; i++)
	{
		//if (i > count - 1)target_ = { 0.0f, 5.0f, 0.0f };
		target_ = { MyMath::RandomMTFloat(min.x,max.x), MyMath::RandomMTFloat(min.y,max.y), MyMath::RandomMTFloat(min.z,max.z) };
	}

	UpdateViewMatrix();

	// 透視投影による射影行列の生成
	UpdateProjectionMatrix();

	matViewProjection_ = matView_ * matProjection_;

	return nowTarget;
}





const XMFLOAT3 Camera::lerp(const XMFLOAT3& start, const XMFLOAT3& end, const float t)
{
	XMFLOAT3 result;

	result.x = start.x * (1.0f - t) + end.x * t;
	result.y = start.y * (1.0f - t) + end.y * t;
	result.z = start.z * (1.0f - t) + end.z * t;

	return result;
}

const float Camera::Flerp(const float& start, const float& end, const float t)
{
	float result;
	result = start * (1.0f - t) + end * t;

	return result;
}

void Camera::SetEye(const XMFLOAT3& eye)
{
	this->eye_ = eye;
}

void Camera::SetTarget(const XMFLOAT3& target) {
	this->target_ = target;
}

void Camera::SetUp(const XMFLOAT3& up)
{
	this->up_ = up;
}