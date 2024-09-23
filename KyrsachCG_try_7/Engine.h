#pragma once
#include <windows.h>
#include <vector>
#include <limits>
#include <algorithm>
#include <numeric>
#include "Model.h"
#include "Matrix4.h"

class Engine
{
	//Model* model = NULL;
	std::vector<Model*> models;
	Matrix4 matrix;
	Matrix4 supMatrix;
	Matrix4 matProj;
	Matrix4 matProjOrt;

	int width_;
	int height_;
	const int depth = 255;
	const int MAX_DEPTH = 8;
	const int eps = 2;
	unsigned int activeModel_ = 0;
	HDC doubleBuffer;
	std::vector<geo::Polygon> screenSpaceCoordinates;
	std::ofstream logFile;

public:

	Engine();
	Engine(int width, int height);
	~Engine();

	void draw(HDC hdc);
	void drawWithShadow(HDC hdc);
	void rotX(float z);
	void rotY(float z);
	void rotZ(float z);
	void move(float x, float y, float z);
	void scale(float s);
	void updProjMatrix(float fNear, float fFar, float fFov, float scrH, float scrW);
	void drawLine(float _x1, float _y1, float _x2, float _y2, HDC hdc, COLORREF color);
	void drawWarnockAlg(HDC hdc);
	void setScreenSpaceCoordinate(size_t index, geo::Polygon value);
	Vec3f centerM();
	void drawFilledTriangle(HDC hdc, Vec3i t0, Vec3i t1, Vec3i t2, float intansity);
	void setActive(int numM);
	void setWidthHeigth(int width, int heigth);
	void initializeDoubleBuffer(HDC hdc, int width, int height);
	void divide(std::vector<geo::Polygon>& polygons, rect window, int depth = 0);
	int countPolyInWindow(const std::vector<geo::Polygon>& polygons, const std::vector<Vec3i>& win, std::vector<geo::Polygon>& inPolygons);
	bool isPointInWindow(Vec3i p, std::vector<Vec3i> w);
	bool isPointInTriangle(Vec3i p, std::vector<Vec3i> t);
	bool isPointInPolygon(const std::vector<Vec3i>& poly, const Vec3i& point);
	bool isIntersection(const std::vector<Vec3i> line_p, const std::vector<Vec3i> line_w, int* x, int* y, int* z);
	bool pointInPoly(Vec3i p, std::vector<Vec3i> poly);
	Vec3i getPolyCentroid(const std::vector<Vec3i> poly);
	double sign(Vec3i p1, Vec3i p2, Vec3i p3);
	void drawSection(const rect& w, int xc, int yc);
	void drawPolygon(const std::vector<Vec3i>& poly, float intensity, COLORREF boarderColor = RGB(0, 255, 0));
	void fillPolygon(const std::vector<Vec3i>& poly, float intensity);
	std::vector<Vec3i> getPolygonIntersection(const std::vector<Vec3i>& p, const std::vector<Vec3i>& w, float intens);
	int orientation(const Vec3i& p, const Vec3i& q, const Vec3i& r);
	bool onSegment(const Vec3i& p, const Vec3i& q, const Vec3i& r);
	bool doIntersect(const Vec3i& p1, const Vec3i& q1, const Vec3i& p2, const Vec3i& q2);
	Vec3f calculateShadowCoordinate(const Vec3f& vsh, const Vec3f& light_pos);
	void resizeDoubleBuffer(int width, int height);
	void drawDoubleBuffer(HDC hdc);
	static bool compareZ(const geo::Polygon& a, const geo::Polygon& b);
	float interpolateZ(Vec3i z1, Vec3i z2, int x);
	void logAndWriteZ(const std::vector<Vec3i>& points);
	HDC getDoubleBuffer() const { return doubleBuffer; }
};