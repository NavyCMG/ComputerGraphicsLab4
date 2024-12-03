#pragma once
#include "Defines.h"
#include <algorithm>


unsigned int Lerp(unsigned int start,unsigned int end, float ratio = -1)
{
	if (ratio == -1)
	{
		ratio = (float)start / (float)end;
	}
	end = ((int)end - (int)start) * ratio + start;
	return end;
}
float Lerp(float start, float end, float ratio = -1)
{
	if (ratio == -1)
	{
		ratio = (float)start / (float)end;
	}
	end = (end - start) * ratio + start;
	return end;
}
unsigned int ColorLerp(unsigned int a, unsigned int b, float ratio)
{
	unsigned int AlphaA = (a & 0xff000000) >> 24;
	unsigned int RedA = (a & 0x00ff0000) >> 16;
	unsigned int GrnA = (a & 0x0000ff00) >> 8;
	unsigned int BluA = (a & 0x000000ff);
	unsigned int AlphaB = (b & 0xff000000) >> 24;
	unsigned int RedB = (b & 0x00ff0000) >> 16;
	unsigned int GrnB = (b & 0x0000ff00) >> 8;
	unsigned int BluB = (b & 0x000000ff);

	RedA = (int(RedA) - int(RedB)) * ratio + RedB;
	GrnA = (int(GrnA) - int(GrnB)) * ratio + GrnB;
	BluA = (int(BluA) - int(BluB)) * ratio + BluB;

	return (AlphaB << 24 | RedA << 16 | GrnA << 8 | BluA);
}
unsigned int ColorBlend(unsigned int currentColor, unsigned int newColor, float ratio = -1)
{
	unsigned int newAlpha = (newColor & 0xff000000);
	if (newAlpha != 0xff000000 && ratio == -1)
	{
		ratio = float(newAlpha) / float(0xff000000);
		/*currentColor = (currentColor & 0x00ffffff);
		newColor = (newColor & 0x00ffffff);
		newColor = (int(newColor) - int(currentColor)) * ratio + currentColor;*/
		unsigned int curRed = (currentColor & 0x00ff0000) >> 16;
		unsigned int curGrn = (currentColor & 0x0000ff00) >> 8;
		unsigned int curBlu = (currentColor & 0x000000ff);
		unsigned int newRed = (newColor & 0x00ff0000) >> 16;
		unsigned int newGrn = (newColor & 0x0000ff00) >> 8;
		unsigned int newBlu = (newColor & 0x000000ff);

		newRed = (int(newRed) - int(curRed)) * ratio + curRed;
		newGrn = (int(newGrn) - int(curGrn)) * ratio + curGrn;
		newBlu = (int(newBlu) - int(curBlu)) * ratio + curBlu;

		newColor = (newAlpha | newRed << 16 | newGrn << 8 | newBlu);
	}
	else if (newAlpha != 0xff000000 && ratio != 1)
	{
		unsigned int curRed = (currentColor & 0x00ff0000) >> 16;
		unsigned int curGrn = (currentColor & 0x0000ff00) >> 8;
		unsigned int curBlu = (currentColor & 0x000000ff);
		unsigned int newRed = (newColor & 0x00ff0000) >> 16;
		unsigned int newGrn = (newColor & 0x0000ff00) >> 8;
		unsigned int newBlu = (newColor & 0x000000ff);

		newRed = (int(newRed) - int(curRed)) * ratio + curRed;
		newGrn = (int(newGrn) - int(curGrn)) * ratio + curGrn;
		newBlu = (int(newBlu) - int(curBlu)) * ratio + curBlu;

		newColor = (newAlpha | newRed << 16 | newGrn << 8 | newBlu);
	}
	return newColor;
}

unsigned int ColorClamp(unsigned int a)
{
	if (a > 0xff)
	{
		a = 0xff;
	}
	else if (a < 0)
	{
		a = 0;
	}
	return a;
}
unsigned int ColorAdd(unsigned int a, unsigned int b)
{
	unsigned int AlphaA = (a & 0xff000000) >> 24;
	unsigned int RedA = (a & 0x00ff0000) >> 16;
	unsigned int GrnA = (a & 0x0000ff00) >> 8;
	unsigned int BluA = (a & 0x000000ff);
	unsigned int AlphaB = (b & 0xff000000) >> 24;
	unsigned int RedB = (b & 0x00ff0000) >> 16;
	unsigned int GrnB = (b & 0x0000ff00) >> 8;
	unsigned int BluB = (b & 0x000000ff);

	AlphaA = ColorClamp(AlphaA + AlphaB);
	RedA = ColorClamp(RedA + RedB);
	GrnA = ColorClamp(GrnA + GrnB);
	BluA = ColorClamp(BluA + BluB);

	return (AlphaA << 24 | RedA << 16 | GrnA << 8 | BluA);
}

unsigned int ColorModulate(unsigned int a, unsigned int b)
{
	float AlphaA = (float)((a & 0xff000000) >> 24) / 0xff;
	float RedA = (float)((a & 0x00ff0000) >> 16) / 0xff;
	float GrnA = (float)((a & 0x0000ff00) >> 8) / 0xff;
	float BluA = (float)((a & 0x000000ff)) / 0xff;
	float AlphaB = (float)((b & 0xff000000) >> 24) / 0xff;
	float RedB = (float)((b & 0x00ff0000) >> 16) / 0xff;
	float GrnB = (float)((b & 0x0000ff00) >> 8) / 0xff;
	float BluB = (float)((b & 0x000000ff)) / 0xff;

	AlphaA = ColorClamp((AlphaA * AlphaB) * 0xff);
	RedA = ColorClamp((RedA * RedB) * 0xff);
	GrnA = ColorClamp((GrnA * GrnB) * 0xff);
	BluA = ColorClamp((BluA * BluB) * 0xff);

	return ((unsigned int)AlphaA << 24 | (unsigned int)RedA << 16 | (unsigned int)GrnA << 8 | (unsigned int)BluA);
}


float ImplicitLine(Point2D x, Point2D y, Point2D p)
{
	return ((x.y - y.y) * p.x) + ((y.x - x.x) * p.y) + (x.x * y.y) - (x.y * y.x);
}

float BaryInterp(float a, float b, float c, Bary BarSource) 
{
	return (a * BarSource.alpha) + (b * BarSource.beta) + (c * BarSource.gamma);
}
unsigned int BaryInterpColor(unsigned int a, unsigned int b, unsigned int c, Bary BarSource)
{
	unsigned int AlphaA = (a & 0xff000000) >> 24;
	unsigned int RedA = (a & 0x00ff0000) >> 16;
	unsigned int GrnA = (a & 0x0000ff00) >> 8;
	unsigned int BluA = (a & 0x000000ff);
	unsigned int AlphaB = (b & 0xff000000) >> 24;
	unsigned int RedB = (b & 0x00ff0000) >> 16;
	unsigned int GrnB = (b & 0x0000ff00) >> 8;
	unsigned int BluB = (b & 0x000000ff);
	unsigned int AlphaC = (c & 0xff000000) >> 24;
	unsigned int RedC = (c & 0x00ff0000) >> 16;
	unsigned int GrnC = (c & 0x0000ff00) >> 8;
	unsigned int BluC = (c & 0x000000ff);

	AlphaA = (AlphaA * BarSource.alpha) + (AlphaB * BarSource.beta) + (AlphaC * BarSource.gamma);
	RedA = (RedA * BarSource.alpha) + (RedB * BarSource.beta) + (RedC * BarSource.gamma);
	GrnA = (GrnA * BarSource.alpha) + (GrnB * BarSource.beta) + (GrnC * BarSource.gamma);
	BluA = (BluA * BarSource.alpha) + (BluB * BarSource.beta) + (BluC * BarSource.gamma);

	return (AlphaA << 24 | RedA << 16 | GrnA << 8 | BluA);
}

Bary FindBary(Point2D a, Point2D b, Point2D c, Point2D p)
{
	float alpha = ImplicitLine(c, b, a);
	float beta = ImplicitLine(a, c, b);
	float gamma = ImplicitLine(b, a, c);
	float A = ImplicitLine(c, b, p);
	float B = ImplicitLine(a, c, p);
	float C = ImplicitLine(b, a, p);
	alpha = A / alpha;
	beta = B / beta;
	gamma = C / gamma;
	float z = (a.z * alpha) + (b.z * beta) + (c.z * gamma);
	Bary bar = Bary(alpha, beta, gamma, z);
	return bar;
}

int TwoDtoOneD(int x, int y, int arrWidth = screenWidth)
{
	if (x < screenWidth && x > 0 && y < screenHeight && y > 0)
	{
		int result = (y * arrWidth) + x;
		return result;
	}
	else { return -1; }
}

void UpdateMatrix(float m[4][4], float u[4][4])
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m[i][j] = u[i][j];
		}
	}
}
Vertex VecMtrxMult(Vertex v, float m[4][4])
{
	Vertex result = Vertex(0, 0, 0, 0, v.color, v.u, v.v);
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			result.xyzw[i] += v.xyzw[j] * m[j][i];
		}
	}
	return result;
}

void SqrMtrxMult(float a[4][4], float b[4][4])
{
	float result[4][4]{ 0 };
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			for (int k = 0; k < 4; ++k)
			{
				result[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	UpdateMatrix(a, result);
}

float* OrthogAffinInverse(float m[4][4])
{
	
	float result[4][4]{ 0 };
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			result[i][j] = m[j][i];
		}
	}
	Vertex posVert = Vertex(0, 0, 0, 0);
	for (int k = 0; k < 4; ++k)
	{
		posVert.xyzw[k] = m[3][k];
	}
	Vertex posVertr = VecMtrxMult(posVert, result);
	for (int l = 0; l < 3; ++l)
	{
		result[3][l] -= posVertr.xyzw[l];
	}
	result[3][3] = 1;
	return reinterpret_cast<float*>(result);
}

void OrthogAffinInverseCam(Camera *c)
{

	float temp[4][4]{ 0 };
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			c->inverse[i][j] = c->local[j][i];
		}
	}
	Vertex posVert = Vertex(0, 0, 0, 0);
	for (int k = 0; k < 4; ++k)
	{
		posVert.xyzw[k] = c->local[3][k];
	}
	Vertex posVertr = VecMtrxMult(posVert, c->inverse);
	for (int l = 0; l < 3; ++l)
	{
		c->inverse[3][l] -= posVertr.xyzw[l];
	}
	c->inverse[3][3] = 1;
}

void ScaleMatrix(float m[4][4], float scale)
{
	float Scaler[4][4]{ {scale,0,0,0},{0,scale,0,0},{0,0,scale,0},{0,0,0,1} };
	SqrMtrxMult(m, Scaler);
}

float DotProduct(Vertex a, Vertex b)
{
	return (a.xyzw[0] * b.xyzw[0]) + (a.xyzw[1] * b.xyzw[1]) + (a.xyzw[2] * b.xyzw[2]);
}
float DotProduct(float a[3], float b[3])
{
	return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
}

Vertex CrossProduct(Vertex a, Vertex b)
{
	Vertex c;
	c.xyzw[0] = (a.xyzw[1] * b.xyzw[2]) - (a.xyzw[2] * b.xyzw[1]);
	c.xyzw[1] = (a.xyzw[2] * b.xyzw[0]) - (a.xyzw[0] * b.xyzw[2]);
	c.xyzw[2] = (a.xyzw[0] * b.xyzw[1]) - (a.xyzw[1] * b.xyzw[0]);
	return c;
}

float VertLength(Vertex v)
{
	return sqrt((v.xyzw[0] * v.xyzw[0]) + (v.xyzw[1] * v.xyzw[1]) + (v.xyzw[2] * v.xyzw[2]));
}
float VertLength(float v[3])
{
	return sqrt((v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]));
}

Vertex VecNormalize(Vertex v)
{
	float lngth = VertLength(v);
	Vertex res;
	res.xyzw[0] = res.xyzw[0] / lngth;
	res.xyzw[1] = res.xyzw[1] / lngth;
	res.xyzw[2] = res.xyzw[2] / lngth;
	return res;
}
void VecNormalize(float* v)
{
	float lngth = VertLength(v);

	v[0] = v[0] / lngth;
	v[1] = v[1] / lngth;
	v[2] = v[2] / lngth;
}

float FloatClamp(float a)
{
	if (a > 1)
	{
		a = 1;
	}
	else if (a < 0)
	{
		a = 0;
	}
	return a;
}