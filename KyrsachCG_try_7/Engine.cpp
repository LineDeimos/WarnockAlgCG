#include "Engine.h"
#include <algorithm>

Engine::Engine() {
	updProjMatrix(0.1f, 1000.0f, 90.0f, 400.0f, 600.0f);
	models.push_back(new Model("obj/Prism_in.obj"));
	models.push_back(new Model("obj/Prism_in.obj"));
	move(0, 0, -8);
}

Engine::Engine(int width, int height) : width_(width), height_(height)
{
	matProjOrt.setOrthographicProjection(-100, 100, -100, 100, -100, 200);
	updProjMatrix(0.1f, 1000.0f, 90.0f, height_, width_);
	//models.push_back(new Model("obj/african_head.obj"));
	//models.push_back(new Model("obj/Triangle.obj"));
	models.push_back(new Model("obj/Prism_in.obj"));
	//models.push_back(new Model("obj/just_anvil.obj"));
	//models.push_back(new Model("obj/just_anvil.obj"));
	
	move(0, 0, -13);
	//rotX(-100.0f);
	//rotY(50.0f);
	//rotY(-75.0f);
	scale(0.5f);

	/*models.push_back(new Model("obj/Plane_2.obj"));
	move(0, -5, -15);
	rotX(250.0f);
	scale(6.0f);*/
	logFile.open("log.txt");

	if (!logFile.is_open()) {
		std::cerr << "Ошибка открытия файла для записи: " << "log.txt" << std::endl;
		return;
	}
}

Engine::~Engine()
{
	DeleteDC(doubleBuffer);
	logFile.close();
}

void Engine::drawWithShadow(HDC hdc)
{
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB(0, 255, 0));
	Vec3f light_pos(-50, 100, 100);
	Vec3f light_dir(0, 0, -1);
	for (int k = 0; k < models.size(); k++) {
		for (int i = 0; i < models[k]->nfaces(); i++) {
			std::vector<int> face = models[k]->face(i);
			Vec3f world_coords[3];
			Triangle<Vec3i> tris;
			Triangle<Vec3i> trisShd;
			for (int j = 0; j < 3; j++) {
				Vec3f v0 = models[k]->vert(face[j]);
				Vec3f vsh = models[k]->vert(face[j]);
				Vec3f tmp = models[k]->vert(face[j]);

				tmp = calculateShadowCoordinate(v0, light_pos);

				world_coords[j] = v0;
				matProj.applyMatrixToVector(v0);
				matProj.applyMatrixToVector(tmp);
				int xsh = (tmp.x + 1.) * width_ / 2.;
				int ysh = ((tmp.y + 1.) * height_ / 2.);
				int x0 = (v0.x + 1.) * width_ / 2.;
				int y0 = ((v0.y + 1.) * height_ / 2.);
				tris.vertex[j] = Vec3i(x0, y0, v0.z);
				trisShd.vertex[j] = Vec3i(xsh, ysh, tmp.z);
			}
			drawFilledTriangle(hdc, trisShd.vertex[0], trisShd.vertex[1], trisShd.vertex[2], 0.1);
			Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
			n.normalize();
			float intensity = n * light_dir;
			if (intensity > 0.1) {
				drawFilledTriangle(hdc, tris.vertex[0], tris.vertex[1], tris.vertex[2], intensity);
			}
		}
	}
}

void Engine::draw(HDC hdc)
{
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB(0, 255, 0));
	Vec3f light_dir(0, 0, -1);
	for (int k = 0; k < models.size(); k++) {
		for (int i = 0; i < models[k]->nfaces(); i++) {
			std::vector<int> face = models[k]->face(i);
			Vec3f world_coords[3];
			Triangle<Vec3i> tris;
			for (int j = 0; j < 3; j++) {
				Vec3f v0 = models[k]->vert(face[j]);
				Vec3f v1 = models[k]->vert(face[(j + 1) % 3]);
				world_coords[j] = v0;
				matProj.applyMatrixToVector(v0);
				matProj.applyMatrixToVector(v1);
				int x0 = (v0.x + 1.) * width_ / 2.;
				int y0 = ((v0.y + 1.) * height_ / 2.);
				tris.vertex[j] = Vec3i(x0, y0, v0.z);
			}
			MoveToEx(hdc, tris.vertex[0].x, tris.vertex[0].y, NULL);
			LineTo(hdc, tris.vertex[1].x, tris.vertex[1].y);
			LineTo(hdc, tris.vertex[2].x, tris.vertex[2].y);
			LineTo(hdc, tris.vertex[0].x, tris.vertex[0].y);
			/*Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
			n.normalize();
			float intensity = n * light_dir;
			if (intensity > 0) {
				drawFilledTriangle(hdc, tris.vertex[0], tris.vertex[1], tris.vertex[2], intensity);
			}*/
		}
	}
}

void Engine::rotX(float z)
{
	Vec3f c = centerM();
	matrix.setTranslationMatrix(-c.x, -c.y, -c.z);
	supMatrix.setTranslationMatrix(c.x, c.y, c.z);
	for (int i = 0; i < models[activeModel_]->nverts(); i++) {
		Vec3f v = models[activeModel_]->vert(i);
		matrix.applyMatrixToVector(v);
		models[activeModel_]->setVert(i, v);
	}
	matrix.setRotationMatrixX(z);
	matrix.multiply(supMatrix);
	for (int i = 0; i < models[activeModel_]->nverts(); i++) {
		Vec3f v = models[activeModel_]->vert(i);
		matrix.applyMatrixToVector(v);
		models[activeModel_]->setVert(i, v);
	}
	return;
}

void Engine::rotY(float z)
{
	Vec3f c = centerM();
	matrix.setTranslationMatrix(-c.x, -c.y, -c.z);
	supMatrix.setTranslationMatrix(c.x, c.y, c.z);
	for (int i = 0; i < models[activeModel_]->nverts(); i++) {
		Vec3f v = models[activeModel_]->vert(i);
		matrix.applyMatrixToVector(v);
		models[activeModel_]->setVert(i, v);
	}
	matrix.setRotationMatrixY(z);
	matrix.multiply(supMatrix);
	for (int i = 0; i < models[activeModel_]->nverts(); i++) {
		Vec3f v = models[activeModel_]->vert(i);
		matrix.applyMatrixToVector(v);
		models[activeModel_]->setVert(i, v);
	}
	return;
}

void Engine::rotZ(float z)
{
	Vec3f c = centerM();
	matrix.setTranslationMatrix(-c.x, -c.y, -c.z);
	supMatrix.setTranslationMatrix(c.x, c.y, c.z);
	for (int i = 0; i < models[activeModel_]->nverts(); i++) {
		Vec3f v = models[activeModel_]->vert(i);
		matrix.applyMatrixToVector(v);
		models[activeModel_]->setVert(i, v);
	}
	matrix.setRotationMatrixZ(z);
	matrix.multiply(supMatrix);
	for (int i = 0; i < models[activeModel_]->nverts(); i++) {
		Vec3f v = models[activeModel_]->vert(i);
		matrix.applyMatrixToVector(v);
		models[activeModel_]->setVert(i, v);
	}
	return;
}

void Engine::move(float x, float y, float z)
{
	matrix.setTranslationMatrix(x, y, z);
	for (int i = 0; i < models[activeModel_]->nverts(); i++) {
		Vec3f v = models[activeModel_]->vert(i);
		matrix.applyMatrixToVector(v);
		models[activeModel_]->setVert(i, v);
	}
	return;
}

void Engine::scale(float s)
{
	Vec3f c = centerM();
	matrix.setTranslationMatrix(-c.x, -c.y, -c.z);
	supMatrix.setTranslationMatrix(c.x, c.y, c.z);
	for (int i = 0; i < models[activeModel_]->nverts(); i++) {
		Vec3f v = models[activeModel_]->vert(i);
		matrix.applyMatrixToVector(v);
		models[activeModel_]->setVert(i, v);
	}
	matrix.setScalingMatrix(s);
	matrix.multiply(supMatrix);
	for (int i = 0; i < models[activeModel_]->nverts(); i++) {
		Vec3f v = models[activeModel_]->vert(i);
		matrix.applyMatrixToVector(v);
		models[activeModel_]->setVert(i, v);
	}
	return;
}

void Engine::updProjMatrix(float fNear, float fFar, float fFov, float scrH, float scrW)
{
	matProj.setProjectionMatrix(fNear, fFar, fFov, scrH, scrW);
}

void Engine::drawLine(float _x1, float _y1, float _x2, float _y2, HDC hdc, COLORREF color)
{
	int x1 = round(_x1);
	int x2 = round(_x2);
	int y1 = round(_y1);
	int y2 = round(_y2);
	int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
	int dy = abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2;
	int e2 = err;
	for (;;)
	{
		SetPixel(hdc, x1, y1, color);
		if (x1 == x2 && y1 == y2) break;
		e2 = err;
		if (e2 > -dx)
			err -= dy, x1 += sx;
		if (e2 < dy)
			err += dx, y1 += sy;
	}
}

Vec3f Engine::centerM()
{
	Vec3f tmp;
	for (int i = 0; i < models[activeModel_]->nverts(); i++)
		tmp = tmp + models[activeModel_]->vert(i);
	tmp.x = tmp.x / models[activeModel_]->nverts();
	tmp.y = tmp.y / models[activeModel_]->nverts();
	tmp.z = tmp.z / models[activeModel_]->nverts();
	return tmp;
}

void Engine::drawWarnockAlg(HDC hdc)
{
	Vec3f light_dir(0, 0, -1);
	Vec3f light_pos(-50, 100, 100);
	for (int k = 0; k < models.size(); k++) {
		for (int i = 0; i < models[k]->nfaces(); i++) {
			std::vector<int> face = models[k]->face(i);
			Vec3f world_coords[3];
			geo::Polygon poly;
			poly.points.reserve(3);
			Triangle<Vec3i> trisShd;
			for (int j = 0; j < 3; j++) {
				Vec3f v0 = models[k]->vert(face[j]);
				Vec3f vsh = calculateShadowCoordinate(v0, light_pos);
				world_coords[j] = v0;
				matProj.applyMatrixToVector(v0);
				matProj.applyMatrixToVector(vsh);
				int xsh = (vsh.x + 1.) * width_ / 2.;
				int ysh = ((vsh.y + 1.) * height_ / 2.);
				int x0 = (v0.x + 1.) * width_ / 2.;
				int y0 = ((v0.y + 1.) * height_ / 2.);
				poly.points.push_back(Vec3i(x0, y0, v0.z));
				trisShd.vertex[j] = Vec3i(xsh, ysh, vsh.z);
			}
			drawFilledTriangle(hdc, trisShd.vertex[0], trisShd.vertex[1], trisShd.vertex[2], 1);
			Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
			n.normalize();
			float intensity = n * light_dir;
			poly.intensity = intensity;
			setScreenSpaceCoordinate(k*models[0]->nfaces() + i, poly);
		}
	}
	divide(screenSpaceCoordinates, { {0, 0}, {width_, height_} });
}

void Engine::drawFilledTriangle(HDC hdc, Vec3i t0, Vec3i t1, Vec3i t2, float intensity)
{
	if (t0.y == t1.y && t0.y == t2.y) return;
	if (t0.y > t1.y) std::swap(t0, t1);
	if (t0.y > t2.y) std::swap(t0, t2);
	if (t1.y > t2.y) std::swap(t1, t2);
	int total_height = t2.y - t0.y;
	for (int i = 0; i < total_height; i++) {
		bool second_half = i > t1.y - t0.y || t1.y == t0.y;
		int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
		float alpha = (float)i / total_height;
		float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height;
		Vec3i A = t0 + Vec3f(t2 - t0) * alpha;
		Vec3i B = second_half ? t1 + Vec3f(t2 - t1) * beta : t0 + Vec3f(t1 - t0) * beta;
		if (A.x > B.x) std::swap(A, B);
		for (int j = A.x; j <= B.x; j++) {
			float phi = B.x == A.x ? 1. : (float)(j - A.x) / (float)(B.x - A.x);
			Vec3i P = Vec3f(A) + Vec3f(B - A) * phi;
			int idx = P.x + P.y * width_;
			if (P.x < 0 || P.x >= width_ || P.y < 0 || P.y >= height_) {
				continue;
			}
			SetPixel(hdc, P.x, P.y, RGB(intensity * 240, intensity * 240, intensity * 240));
		}
	}
}

void Engine::setScreenSpaceCoordinate(size_t index, geo::Polygon value)
{
	if (index < screenSpaceCoordinates.size())
	{
		screenSpaceCoordinates[index] = value;
	}
	else
	{
		screenSpaceCoordinates.push_back(value);
	}
}

void Engine::setActive(int numM)
{
	activeModel_ = numM;
}

void Engine::setWidthHeigth(int width, int heigth)
{
	width_ = width;
	height_ = heigth;
}

void Engine::initializeDoubleBuffer(HDC hdc, int width, int height)
{
	doubleBuffer = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);

	HBITMAP hOldBitmap = (HBITMAP)SelectObject(doubleBuffer, hBitmap);

	if (hOldBitmap != nullptr)
	{
		DeleteObject(hOldBitmap);
	}
}

void Engine::divide(std::vector<geo::Polygon>& polygons, rect window, int depth)
{
	//if (window.size_.x - window.pos_.x < eps || window.size_.y - window.pos_.y < eps) return;
	if (depth > MAX_DEPTH) return;

	std::vector<Vec3i> windowPoints = { {window.pos_.x, window.pos_.y}, {window.size_.x, window.pos_.y},
										{window.size_.x, window.size_.y}, {window.pos_.x, window.size_.y} };
	std::vector<geo::Polygon> inPolygons;
	int count = countPolyInWindow(polygons, windowPoints, inPolygons);

	if (!count)
	{
		return;
	}
	else if (count == 1) {
		drawPolygon(inPolygons[0].points, inPolygons[0].intensity, RGB(0, 0, 0));
		return;
	}
	else {
		bool is_window_in_poly;
		std::vector<geo::Polygon> out_tri;

		for (size_t i = 0; i < inPolygons.size(); i++) {
			if (inPolygons[i].points.size() != 4) continue;

			int cnt = 0;

			for (size_t j = 0; j < 4; j++) {
				for (size_t k = 0; k < 4; k++) {
					if (windowPoints[j] == inPolygons[i].points[k]) cnt++;
				}
			}

			if (cnt == 4) out_tri.push_back(inPolygons[i]);
		}

		if (!out_tri.empty())
		{
			std::sort(inPolygons.begin(), inPolygons.end(), &compareZ);

			if (std::find(out_tri.begin(), out_tri.end(), inPolygons[0]) != out_tri.end())
			{
				windowPoints = { {window.pos_.x - 1, window.pos_.y - 1}, {window.size_.x + 1, window.pos_.y - 1},
							 {window.size_.x + 1, window.size_.y + 1}, {window.pos_.x - 1, window.size_.y + 1} };
				drawPolygon(windowPoints, inPolygons[0].intensity, RGB(0, 0, 0));
				return;
			}
		}

		
	}

	int xc = (window.size_.x - window.pos_.x) / 2 + window.pos_.x;
	int yc = (window.size_.y - window.pos_.y) / 2 + window.pos_.y;

	//drawSection(window, xc, yc);

	divide(polygons, { {window.pos_.x, window.pos_.y}, { xc, yc } }, depth + 1);
	divide(polygons, { {xc, window.pos_.y}, { window.size_.x, yc } }, depth + 1);
	divide(polygons, { {window.pos_.x, yc}, { xc, window.size_.y } }, depth + 1);
	divide(polygons, { {xc, yc}, { window.size_.x, window.size_.y } }, depth + 1);
}

int Engine::countPolyInWindow(const std::vector<geo::Polygon>& polygons, const std::vector<Vec3i>& win, std::vector<geo::Polygon>& inPolygons)
{
	int count = 0;
	for (size_t i = 0; i < polygons.size(); i++) {
		std::vector<Vec3i> temp = getPolygonIntersection(polygons[i].points, win, polygons[i].intensity);
		if (temp.size()) {
			count++;
			inPolygons.push_back({ temp, polygons[i].intensity });
		}
	}
	return count;
}

std::vector<Vec3i> Engine::getPolygonIntersection(const std::vector<Vec3i>& p, const std::vector<Vec3i>& w, float intens)
{
	std::vector<Vec3i> res;
	int count_vert_p = p.size(), count_vert_w = w.size();
	int x, y, z;

	int midZ = 0;
	for (const auto& vertex : p) {
		midZ += vertex.z + intens;
	}
	midZ /= static_cast<int>(p.size());
	logFile << "\n MIDZ: " << midZ << std::endl;

	for (size_t i = 0; i < count_vert_w; i++) {
		if (isPointInTriangle(w[i], p)) {
			res.push_back(Vec3i(w[i].x, w[i].y, midZ)); // !!!!!
		}
	}

	for (size_t i = 0; i < count_vert_p; i++) {
		if (isPointInWindow(p[i], w)) res.push_back(p[i]);
	}

	for (size_t i = 0; i < count_vert_p; i++) {
		std::vector<Vec3i> line_p = { p[i], p[(i + 1) % count_vert_p] };
		for (size_t j = 0; j < count_vert_w; j++) {
			std::vector<Vec3i> line_w = { w[j], w[(j + 1) % count_vert_w] };

			if (isIntersection(line_p, line_w, &x, &y, &z)) {
				if (!pointInPoly({ x, y }, res)) res.push_back({ x, y, z}); // !!!!!! Vec3 ==
			}
		}
	}

	if (res.empty()) return res;

	Vec3i center = getPolyCentroid(res);
	sort(res.begin(), res.end(), [center](Vec3i a, Vec3i b) {
		if (a.x - center.x >= 0 && b.x - center.x < 0) return true;
		if (a.x - center.x < 0 && b.x - center.x >= 0) return false;
		if (a.x - center.x == 0 && b.x - center.x == 0) {
			if (a.y - center.y >= 0 || b.y - center.y >= 0) return a.y > b.y;
			return b.y > a.y;
		}

		int det = (a.x - center.x) * (b.y - center.y) - (b.x - center.x) * (a.y - center.y);
		if (det < 0) return true;
		if (det > 0) return false;

		int d1 = (a.x - center.x) * (a.x - center.x) + (a.y - center.y) * (a.y - center.y);
		int d2 = (b.x - center.x) * (b.x - center.x) + (b.y - center.y) * (b.y - center.y);
		return d1 > d2;
		});
	return res;
}

bool Engine::isPointInWindow(Vec3i p, std::vector<Vec3i> w)
{
	return p.x >= w[0].x && p.x <= w[1].x && p.y <= w[2].y && p.y >= w[0].y;
}

bool Engine::isPointInTriangle(Vec3i p, std::vector<Vec3i> t)
{
	double d1, d2, d3;
	d1 = sign(p, t[0], t[1]);
	d2 = sign(p, t[1], t[2]);
	d3 = sign(p, t[2], t[0]);
	return !(((d1 <= 0) || (d2 <= 0) || (d3 <= 0)) && ((d1 >= 0) || (d2 >= 0) || (d3 >= 0)));
}

bool Engine::isPointInPolygon(const std::vector<Vec3i>& poly, const Vec3i& point) {
	int n = poly.size();
	bool inside = false;

	for (int i = 0, j = n - 1; i < n; j = i++) {

		if (((poly[i].y > point.y) != (poly[j].y > point.y)) &&
			(point.x < (poly[j].x - poly[i].x) * (point.y - poly[i].y) / (poly[j].y - poly[i].y) + poly[i].x)) {
			inside = !inside;
		}
	}

	return inside;
}

bool Engine::isIntersection(const std::vector<Vec3i> line_p, const std::vector<Vec3i> line_w, int* x, int* y, int* z)
{
	Vec3i p1 = line_p[0];
	Vec3i q1 = line_p[1];
	Vec3i p2 = line_w[0];
	Vec3i q2 = line_w[1];

	if (doIntersect(p1, q1, p2, q2)) {
		int A1 = q1.y - p1.y;
		int B1 = p1.x - q1.x;
		int C1 = A1 * p1.x + B1 * p1.y;

		int A2 = q2.y - p2.y;
		int B2 = p2.x - q2.x;
		int C2 = A2 * p2.x + B2 * p2.y;

		int det = A1 * B2 - A2 * B1;

		if (det == 0) {
			return false;
		}

		*x = (B2 * C1 - B1 * C2) / det;
		*y = (A1 * C2 - A2 * C1) / det;

		if (p1.z == q1.z) {
			*z = p1.z;
		}
		else {
			*z = static_cast<int>(interpolateZ(p1, q1, *x));
			//*z = (p1.z + q1.z)/2;
		}

		return true;
	}

	return false;
}

bool Engine::pointInPoly(Vec3i p, std::vector<Vec3i> poly)
{
	for (Vec3i& _p : poly) if (_p == p) return true;
	return false;
}

Vec3i Engine::getPolyCentroid(const std::vector<Vec3i> poly)
{
	int x = 0, y = 0, z = 0;
	int count = poly.size();
	for (size_t i = 0; i < count; i++) x += poly[i].x, y += poly[i].y, z += poly[i].z;
	return { x /= count, y /= count, z /= count };
}

double Engine::sign(Vec3i p1, Vec3i p2, Vec3i p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

void Engine::drawSection(const rect& w, int xc, int yc)
{
	SelectObject(doubleBuffer, GetStockObject(DC_PEN));
	SetDCPenColor(doubleBuffer, RGB(0, 255, 0));
	MoveToEx(doubleBuffer, xc, w.pos_.y, NULL);
	LineTo(doubleBuffer, xc, w.size_.y);
	MoveToEx(doubleBuffer, w.pos_.x, yc, NULL);
	LineTo(doubleBuffer, w.size_.x, yc);
}

void Engine::drawPolygon(const std::vector<Vec3i>& poly, float intensity, COLORREF boarderColor)
{
	int count_vert = poly.size();
	if (!count_vert) return;
	//if (intensity > 0) 
		fillPolygon(poly, intensity);

	if (boarderColor != RGB(0, 0, 0)) {

		for (size_t i = 0; i < count_vert; i++) {
			drawLine(poly[i].x, poly[i].y, poly[(i + 1) % count_vert].x, poly[(i + 1) % count_vert].y, doubleBuffer, boarderColor);
		}
	}
}

void Engine::fillPolygon(const std::vector<Vec3i>& poly, float intensity)
{
	int numVertices = poly.size();

	for (int i = 0; i < numVertices; ++i) {
		int x0 = poly[i].x;
		int y0 = poly[i].y;
		int x1 = poly[(i + 1) % numVertices].x;
		int y1 = poly[(i + 1) % numVertices].y;

		drawLine(x0, y0, x1, y1, doubleBuffer, RGB(255 * intensity, 255 * intensity, 255 * intensity));
	}

	int minY = INT_MAX, maxY = INT_MIN;

	for (const Vec3i& vertex : poly) {
		minY = min(minY, vertex.y);
		maxY = max(maxY, vertex.y);
	}

	for (int y = minY; y <= maxY; ++y) {
		std::vector<int> intersectionPoints;

		for (int i = 0; i < numVertices; ++i) {
			int j = (i + 1) % numVertices;
			int y0 = poly[i].y;
			int y1 = poly[j].y;

			if ((y0 <= y && y1 > y) || (y1 <= y && y0 > y)) {
				int x = static_cast<int>((y - y0) * (poly[j].x - poly[i].x) / static_cast<float>(y1 - y0) + poly[i].x);
				intersectionPoints.push_back(x);
			}
		}

		std::sort(intersectionPoints.begin(), intersectionPoints.end());

		for (size_t i = 0; i < intersectionPoints.size(); i += 2) {
			int x0 = max(0, intersectionPoints[i]);
			int x1 = min(GetDeviceCaps(doubleBuffer, HORZRES) - 1, intersectionPoints[i + 1]);

			for (int x = x0; x <= x1; ++x) {
				SetPixel(doubleBuffer, x, y, RGB(255 * intensity, 255 * intensity, 255 * intensity));
			}
		}
	}
}

int Engine::orientation(const Vec3i& p, const Vec3i& q, const Vec3i& r)
{
	int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
	return (val == 0) ? 0 : ((val > 0) ? 1 : 2);
}

bool Engine::onSegment(const Vec3i& p, const Vec3i& q, const Vec3i& r)
{
	if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
		q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
		return true;

	return false;
}

bool Engine::doIntersect(const Vec3i& p1, const Vec3i& q1, const Vec3i& p2, const Vec3i& q2)
{
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	if (o1 != o2 && o3 != o4)
		return true;

	if (o1 == 0 && onSegment(p1, p2, q1)) return true;
	if (o2 == 0 && onSegment(p1, q2, q1)) return true;
	if (o3 == 0 && onSegment(p2, p1, q2)) return true;
	if (o4 == 0 && onSegment(p2, q1, q2)) return true;

	return false;
}

Vec3f Engine::calculateShadowCoordinate(const Vec3f& vsh, const Vec3f& light_pos)
{
	Vec3f tmp;
	tmp.y = -10.0f;

	float coord = (vsh.x - light_pos.x) / (vsh.y - light_pos.y);
	tmp.x = coord * (tmp.y - vsh.y) + vsh.x;

	coord = (vsh.z - light_pos.z) / (vsh.y - light_pos.y);
	tmp.z = coord * (tmp.y - vsh.y) + vsh.z;

	return tmp;
}

void Engine::resizeDoubleBuffer(int width, int height)
{
	width_ = width;
	height_ = height;
	HBITMAP hBitmap = CreateCompatibleBitmap(doubleBuffer, width, height);

	HBITMAP hOldBitmap = (HBITMAP)SelectObject(doubleBuffer, hBitmap);

	if (hOldBitmap != nullptr)
	{
		DeleteObject(hOldBitmap);
	}

	updProjMatrix(0.1f, 1000.0f, 90.0f, height_, width_);
}

bool Engine::compareZ(const geo::Polygon& a, const geo::Polygon& b)
{

	int sumZA = 0;
	int countZA = 0;

	for (const Vec3i& point : a.points) {
		// Игнорируем точки с z == 0
		if (point.z != 0) {
			sumZA += point.z;
			countZA++;
		}
	}

	int sumZB = 0;
	int countZB = 0;

	for (const Vec3i& point : b.points) {
		// Игнорируем точки с z == 0
		if (point.z != 0) {
			sumZB += point.z;
			countZB++;
		}
	}

	if (countZA == 0 || countZB == 0) {
		// Если в одном из полигонов все точки с z == 0, то считаем их равными
		return false;
	}

	return (static_cast<float>(sumZA) / countZA) > (static_cast<float>(sumZB) / countZB);
}

float Engine::interpolateZ(Vec3i z1, Vec3i z2, int x) {
		// Проверка, что точки лежат на одной прямой (по x)
		if (z1.x == z2.x) {
			// Используем линейную интерполяцию для нахождения z
			return z1.z + (z2.z - z1.z) * ((float)x - z1.x) / (z2.x - z1.x);
		}

		// Используем линейную интерполяцию для нахождения z
		float z = z1.z + (z2.z - z1.z) * (x - z1.x) / (z2.x - z1.x);
		return z;
}

void Engine::logAndWriteZ(const std::vector<Vec3i>& points) {

	for (const Vec3i& point : points) {
		logFile << "z: " << point.z << std::endl;
	}
}

void Engine::drawDoubleBuffer(HDC hdc)
{
	RECT rect = { 0, 0, width_, height_ };
	FillRect(doubleBuffer, &rect, (HBRUSH)(COLOR_WINDOW + 2));

	//drawWithShadow(doubleBuffer);
	//draw(doubleBuffer);
	drawWarnockAlg(doubleBuffer);
}
