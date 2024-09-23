#pragma once
#include "geometry.h"
class Matrix4
{
	float data[4][4];

public:
	Matrix4();

	void setUnit();
	void multiply(Matrix4& m);
	void setProjectionMatrix(float fNear, float fFar, float fFov, float scrH, float scrW);
	void setOrthographicProjection(float left, float right, float bottom, float top, float near, float far);
	void setScalingMatrixXYZ(float sx, float sy, float sz);
	void setScalingMatrix(float s);
	void setTranslationMatrix(float dx, float dy, float dz);
	void setRotationMatrixX(float angle);
	void setRotationMatrixY(float angle);
	void setRotationMatrixZ(float angle);
	void applyMatrixToVector(Vec3f& v);
};
