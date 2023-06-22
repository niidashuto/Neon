#include"vector3.h"
#include<cmath>



Vector3::Vector3()
{
	x = y = z = 0;
}

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

float Vector3::length() const
{
	return sqrt(x * x + y * y + z * z);
}

Vector3& Vector3::normalize()
{
	float len = length();
	if (len != 0) {
		return*this /= len;
	}
	return *this;
}

float Vector3::dot(const Vector3& v) const
{
	return (this->x * v.x) + (this->y * v.y) + (this->z * v.z);
}

Vector3 Vector3::cross(const Vector3& v) const
{
	return Vector3((this->y * v.z) - (this->z * v.y), (this->z * v.x) - (this->x * v.z), (this->x * v.y) - (this->y * v.x));
}

Vector3 Vector3::operator+() const
{
	return *this;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3& Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3& Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

//Vector2　クラスに属さない関数群
//二項演算子
const Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp += v2;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp -= v2;
}

const Vector3 operator*(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp *= s;
}

const Vector3 operator*(float s, const Vector3& v)
{
	return v * s;
}

const Vector3 operator/(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp /= s;
}
const Vector3 lerp(const Vector3& start, const Vector3& end, const float t)
{
	//float y=t;
	//return start*(1.0f-t)+end*y;

	return start * (1.0f - t) + end * t;
}
