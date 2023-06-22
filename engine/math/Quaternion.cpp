#include "Quaternion.h"
#include <cmath>

float Norm(const Quaternion& q)
{
	Vector3 i = q.GetImaginary();
	return sqrt(i.dot(i) + q.w * q.w);
}

Quaternion Normalize(const Quaternion& q)
{
	Quaternion ans = q;
	ans /= Norm(q);
	return ans;
}

Quaternion Inverse(const Quaternion& q)
{
	Quaternion ans = Conjugate(q);
	ans /= Norm(q) * Norm(q);
	return ans;
}

void Quaternion::operator*=(const Quaternion& q)
{
	Vector3 iq1 = this->GetImaginary();
	Vector3 iq2 = q.GetImaginary();

	Quaternion ans =
	{
		this->w* q.w - iq1.dot(iq2),
		iq1.cross(iq2) + q.w * iq1 + this->w * iq2
	};
	*this = ans;
}

Quaternion operator*(const Quaternion& q1, const Quaternion& q2)
{
	Quaternion ans = q1;
	ans *= q2;
	return ans;
}

Quaternion Identity() { return Quaternion(1, 0, 0, 0); }

Quaternion Conjugate(const Quaternion& q) { return Quaternion(q.w, -q.GetImaginary()); }

Quaternion MakeAxisAngle(const Vector3& axis, float angle)
{
	return Quaternion(cosf(angle / 2.0f), axis * sinf(angle / 2.0f));
}

Vector3 RotateVector(const Vector3& v, const Quaternion& q)
{
	Quaternion r = { 0,v };
	return Quaternion(q * r * Conjugate(q)).GetImaginary();
}

Matrix4 Quaternion::MakeRotateMatrix() const
{
	Matrix4 mat =
	{
		w * w + x * x - y * y - z * z, 2 * (x * y + w * z), 2 * (x * z - w * y), 0,
		2 * (x * y - w * z), w * w - x * x + y * y - z * z, 2 * (y * z - w * x), 0,
		2 * (x * z + w * y), 2 * (y * z + w * x), w * w - x * x - y * y + z * z, 0,
		0,0,0,1
	};

	return mat;
}

Matrix4 MakeRotateMatrix(const Quaternion& q)
{
	return q.MakeRotateMatrix();
}

