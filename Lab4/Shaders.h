#pragma once
#include "MyMath.h"
#include <algorithm>
#include <cmath>

unsigned int ColorShift(unsigned int orig) //shifts collor from BGRA to ARGB
{
	unsigned int alpha = ((orig & 0x000000ff) << 24);
	unsigned int red = ((orig & 0x0000ff00) << 8);
	unsigned int blue = ((orig & 0xff000000) >> 24);
	unsigned int green = ((orig & 0x00ff0000) >> 8);
	//unsigned int tempNew = (red | blue | green);
	unsigned int result = (alpha | red | blue | green);
	return result;
}

void TranslateCube(Cube *c, float x = 0, float y = 0, float z = 0)
{
	float t[3]{ x, y, z };
	for (int i = 0; i < 3; ++i)
	{
		c->local[3][i] += t[i];
	}
}

void RotateCubeY(float deg, Cube *c)
{
	double rad = deg * (pi / 180);
	float yRot[4][4]{ {std::cos(rad), 0, std::sin(rad), 0}, {0,1,0,0}, {-std::sin(rad), 0, std::cos(rad), 0}, {0,0,0,1} };
	SqrMtrxMult(c->local, yRot);
}

void RotateCameraX(float deg, Camera* c)
{
	double rad = deg * (pi / 180);
	float xRot[4][4]{ {1,0,0,0},{0, std::cos(rad), -std::sin(rad), 0}, {0, std::sin(rad), std::cos(rad), 0}, {0,0,0,1} };
	SqrMtrxMult(c->local, xRot);
}

void RotateCameraY(float deg, Camera* c)
{
	double rad = deg * (pi / 180);
	float yRot[4][4]{ {std::cos(rad), 0, std::sin(rad), 0}, {0,1,0,0}, {-std::sin(rad), 0, std::cos(rad), 0}, {0,0,0,1} };
	SqrMtrxMult(c->local, yRot);
}

void TransposeCamera(Camera* c, float x = 0, float y = 0, float z = 0, float w = 0)
{
	float t[3]{ x, y, z };
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
			c->local[3][i] += t[j] * c->local[j][i];
	}
}

Vertex VertShader(Vertex v, float m[4][4], Camera* c)
{
	Vertex res = VecMtrxMult(v, m);
	Vertex invres = VecMtrxMult(res, c->inverse);
	Vertex persres = VecMtrxMult(invres, c->perspectivePorj);
	for (int i = 0; i < 3; ++i)
	{
		persres.xyzw[i] = (persres.xyzw[i] / persres.xyzw[3]);
	}
	return persres;
}
Vertex VertShader2(Vertex v, float m[4][4], Camera* c, DirLight d, PosLight* p)
{
	float dir[3]{ (p->light.xyzw[0] - v.xyzw[0]), (p->light.xyzw[1] - v.xyzw[1]), (p->light.xyzw[2] - v.xyzw[2]) };

	VecNormalize(dir);
	float ratioP = FloatClamp( DotProduct(dir, v.norm));
	ratioP *= -1;
	float atten = 1 - FloatClamp(VertLength(dir) / p->radius);

	float ratioD = FloatClamp( DotProduct(d.dir, v.norm));
	// ratioD *= -1;
	unsigned int colorP = ColorLerp(black, p->light.color, (ratioP * atten));
	v.color = ColorLerp(black, d.color, ratioD);
	v.color = ColorAdd(v.color, colorP);
	//v.color = colorP;

	Vertex res = VecMtrxMult(v, m);
	Vertex invres = VecMtrxMult(res, c->inverse);
	Vertex persres = VecMtrxMult(invres, c->perspectivePorj);
	for (int i = 0; i < 3; ++i)
	{
		persres.xyzw[i] = (persres.xyzw[i] / persres.xyzw[3]);
	}
	return persres;
}

Vertex VertShaderPt1(Vertex v, float m[4][4], Camera* c)
{
	Vertex res = VecMtrxMult(v, m);
	Vertex invres = VecMtrxMult(res, c->inverse);
	Vertex persres = VecMtrxMult(invres, c->perspectivePorj);
	return persres;
}
Vertex VertShaderPt2(Vertex v)
{
	for (int i = 0; i < 3; ++i)
	{
		v.xyzw[i] = (v.xyzw[i] / v.xyzw[3]);
	}
	return v;
}

Point2D UVtoPoint(float u, float v, const unsigned int* imgWidth, const unsigned int* imgHeight)
{
	u *= *imgWidth;
	v *= *imgHeight;
	Point2D point = Point2D(u, v);
	return point;
}
unsigned int ColorShader(const unsigned int* img, const unsigned int* imgWidth, const unsigned int* imgHeight, Vertex a, Vertex b, Vertex c, Bary brp)
{
	unsigned int color = 0x00000000;
	float x = BaryInterp(a.u, b.u, c.u, brp);
	float y = BaryInterp(a.v, b.v, c.v, brp);
	x *= *imgWidth;
	y *= *imgHeight;
	int point = TwoDtoOneD(x, y, *imgWidth);
	color = ColorShift(img[point]);
	return color;
}
unsigned int ColorShader2(const unsigned int* img, const unsigned int* imgWidth, const unsigned int* imgHeight, Vertex a, Vertex b, Vertex c, Bary brp)
{
	unsigned int color = 0x00000000;
	float x = BaryInterp(a.u, b.u, c.u, brp);
	float y = BaryInterp(a.v, b.v, c.v, brp);
	x *= *imgWidth;
	y *= *imgHeight;
	int point = TwoDtoOneD(x, y, *imgWidth);
	color = ColorShift(img[point]);

	unsigned int lighting = BaryInterpColor(a.color, b.color, c.color, brp);
	color = ColorModulate(color, lighting);

	return color;
}
