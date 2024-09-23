#include "Matrix4.h"

#include "Matrix4.h"
#include <cmath>

Matrix4::Matrix4()
{
	setUnit();
}

void Matrix4::setUnit() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			data[i][j] = 0.0f;
		}
	}
	for (int i = 0; i < 4; i++) {
		data[i][i] = 1.0f;
	}
}

void Matrix4::multiply(Matrix4& m) {
	Matrix4 result;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.data[i][j] = 0;
			for (int k = 0; k < 4; k++) {
				result.data[i][j] += data[i][k] * m.data[k][j];
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			data[i][j] = result.data[i][j];
		}
	}
}

void Matrix4::setProjectionMatrix(float fNear, float fFar, float fFov, float scrH, float scrW)
{
	float fAspectRatio = scrH / scrW;
	float fFlovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);
	data[0][0] = fAspectRatio * fFlovRad;
	data[1][1] = fFlovRad;
	data[2][2] = fFar / (fFar - fNear);
	data[3][2] = (-fFar * fNear) / (fFar - fNear);
	data[2][3] = 1.0f;
	data[3][3] = 0.0f;
}

void Matrix4::setOrthographicProjection(float left, float right, float bottom, float top, float near, float far)
{
	data[0][0] = 2.0f / (right - left);
	data[1][1] = 2.0f / (top - bottom);
	data[2][2] = -2.0f / (far - near);
	data[3][0] = -(right + left) / (right - left);
	data[3][1] = -(top + bottom) / (top - bottom);
	data[3][2] = -(far + near) / (far - near);
	data[3][3] = 1.0f;
}

void Matrix4::setScalingMatrixXYZ(float sx, float sy, float sz)
{
	setUnit();
	data[0][0] = sx;
	data[1][1] = sy;
	data[2][2] = sz;
}

void Matrix4::setScalingMatrix(float s)
{
	setScalingMatrixXYZ(s, s, s);
}

void Matrix4::setTranslationMatrix(float dx, float dy, float dz) {
	setUnit();
	data[3][0] = dx;
	data[3][1] = dy;
	data[3][2] = dz;
}

void Matrix4::setRotationMatrixX(float angle) {
	float cosAngle = cos(angle);
	float sinAngle = sin(angle);
	setUnit();
	data[1][1] = cosAngle;
	data[1][2] = sinAngle;
	data[2][1] = -sinAngle;
	data[2][2] = cosAngle;
}

void Matrix4::setRotationMatrixY(float angle)
{
	float cosAngle = cos(angle);
	float sinAngle = sin(angle);
	setUnit();
	data[0][0] = cosAngle;
	data[0][2] = -sinAngle;
	data[2][0] = sinAngle;
	data[2][2] = cosAngle;
}

void Matrix4::setRotationMatrixZ(float angle)
{
	float cosAngle = cos(angle);
	float sinAngle = sin(angle);
	setUnit();
	data[0][0] = cosAngle;
	data[0][1] = sinAngle;
	data[1][0] = -sinAngle;
	data[1][1] = cosAngle;
}

void Matrix4::applyMatrixToVector(Vec3f& v)
{
	float _x, _y, _z, w;
	_x = v.x;
	_y = v.y;
	_z = v.z;
	v.x = _x * data[0][0] + _y * data[1][0] + _z * data[2][0] + data[3][0];
	v.y = _x * data[0][1] + _y * data[1][1] + _z * data[2][1] + data[3][1];
	v.z = _x * data[0][2] + _y * data[1][2] + _z * data[2][2] + data[3][2];
	w = _x * data[0][3] + _y * data[1][3] + _z * data[2][3] + data[3][3];

	if (w != 0.0f) {
		v.x = v.x / w;
		v.y = v.y / w;
	}
}
