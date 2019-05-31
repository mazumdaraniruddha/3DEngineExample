#include "olcConsoleGameEngine.h"
#include <vector>

using namespace std;

struct vec3d {
	float x, y, z;
};

struct triangle {
	vec3d p[3];
};

struct mesh {
	vector<triangle> tris;
};

struct mat4x4 {
	float m[4][4] = { 0 };
};

class Engine3D : public olcConsoleGameEngine {
private:
	mesh meshCube;
	mat4x4 matProj;
	float fTheta;

	void MatrixMultiply(vec3d &i, vec3d &o, mat4x4 &m) {
		o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
		o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
		o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
		float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];
		if (w != 0.0f) {
			o.x /= w;
			o.y /= w;
			o.z /= w;
		}
	}

public:
	Engine3D() {
		m_sAppName = L"3D Engine";
	}

	bool OnUserCreate() override {
		meshCube.tris = {
			// South face
			{0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f},

			// East face
			{1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f},
			{1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 1.0f},

			// North face
			{1.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 1.0f},
			{1.0f, 0.0f, 1.0f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f},

			// West face
			{0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f},

			// Top face
			{0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f},
			{0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f},

			// Bottom face (draw order?)
			{1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f},
			{1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f}
		};

		// Projection Matrix
		float fNear = 0.1f;
		float fFar = 1000.0f;
		float fFov = 90.0f;
		float fAspectRatio = (float) ScreenHeight() / (float) ScreenWidth();
		float fFovRad = 1.0f / tanf(fFov * 0.5f * 3.14159f / 180.0f);
		float q = (float) fNear / (float)(fFar - fNear);

		matProj.m[0][0] = fAspectRatio * fFovRad;
		matProj.m[0][1] = 0;
		matProj.m[0][2] = 0;
		matProj.m[0][3] = 0;

		matProj.m[1][0] = 0;
		matProj.m[1][1] = fAspectRatio;
		matProj.m[1][2] = 0;
		matProj.m[1][3] = 0;

		matProj.m[2][0] = 0;
		matProj.m[2][1] = 0;
		matProj.m[2][2] = q;
		matProj.m[2][3] = 1;

		matProj.m[3][0] = 0;
		matProj.m[3][1] = 0;
		matProj.m[3][2] = -q * (float)fNear;
		matProj.m[3][3] = 0;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {
		// Clear screen
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);
		// Rotation effect
		mat4x4 matRotZ, matRotX;
		fTheta += 1.0f * fElapsedTime;

		// Rotation Z
		matRotZ.m[0][0] = cosf(fTheta);
		matRotZ.m[0][1] = sinf(fTheta);
		matRotZ.m[0][2] = 0.0f;
		matRotZ.m[1][0] = -sinf(fTheta);
		matRotZ.m[1][1] = cosf(fTheta);
		matRotZ.m[1][2] = 0.0f;
		matRotZ.m[2][0] = 0.0f;
		matRotZ.m[2][1] = 0.0f;
		matRotZ.m[2][2] = 1.0f;
		matRotZ.m[3][3] = 1.0f;

		// Rotation X
		matRotX.m[0][0] = 1.0f;
		matRotX.m[0][1] = 0.0f;
		matRotX.m[0][2] = 0.0f;
		matRotX.m[1][0] = 0.0f;
		matRotX.m[1][1] = cosf(fTheta * 0.5f);
		matRotX.m[1][2] = sinf(fTheta * 0.5f);
		matRotX.m[2][0] = 0.0f;
		matRotX.m[2][1] = -sinf(fTheta * 0.5f);
		matRotX.m[2][2] = cosf(fTheta * 0.5f);
		matRotX.m[3][3] = 1.0f;

		// Draw triangles of the cube
		for (auto tri : meshCube.tris) {
			triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

			MatrixMultiply(tri.p[0], triRotatedZ.p[0], matRotZ);
			MatrixMultiply(tri.p[1], triRotatedZ.p[1], matRotZ);
			MatrixMultiply(tri.p[2], triRotatedZ.p[2], matRotZ);

			MatrixMultiply(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
			MatrixMultiply(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
			MatrixMultiply(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

			triTranslated = triRotatedZX;
			triTranslated.p[0].z = triRotatedZX.p[0].z + 3.0f;
			triTranslated.p[1].z = triRotatedZX.p[1].z + 3.0f;
			triTranslated.p[2].z = triRotatedZX.p[2].z + 3.0f;

			MatrixMultiply(triTranslated.p[0], triProjected.p[0], matProj);
			MatrixMultiply(triTranslated.p[1], triProjected.p[1], matProj);
			MatrixMultiply(triTranslated.p[2], triProjected.p[2], matProj);

			triProjected.p[0].x += 1.0f;
			triProjected.p[0].y += 1.0f;
			triProjected.p[1].x += 1.0f;
			triProjected.p[1].y += 1.0f;
			triProjected.p[2].x += 1.0f;
			triProjected.p[2].y += 1.0f;

			triProjected.p[0].x *= 0.5f * (float) ScreenWidth();
			triProjected.p[0].y *= 0.5f * (float) ScreenHeight();
			triProjected.p[1].x *= 0.5f * (float) ScreenWidth();
			triProjected.p[1].y *= 0.5f * (float) ScreenHeight();
			triProjected.p[2].x *= 0.5f * (float) ScreenWidth();
			triProjected.p[2].y *= 0.5f * (float) ScreenHeight();

			DrawTriangle(triProjected.p[0].x, triProjected.p[0].y,
				triProjected.p[1].x, triProjected.p[1].y,
				triProjected.p[2].x, triProjected.p[2].y,
				PIXEL_SOLID,
				FG_WHITE);
		}
		return true;
	}
};

int main() {
	Engine3D engine3d;
	if (engine3d.ConstructConsole(256, 240, 4, 4)) {
		engine3d.Start();
	}
	return 0;
}