#pragma once
#include "Shaders.h"


void DrawPxl(int currentPos, unsigned int newColor, float z)
{
	if (currentPos > 0 && currentPos < numPxls && (screenZ[currentPos] == -1 || z < screenZ[currentPos]))
	{
		curPxl = screen[currentPos];
		screen[currentPos] = ColorBlend(curPxl, newColor);
		screenZ[currentPos] = z;
	}
}

void LinePara(Point2D start, Point2D end, unsigned int color)
{
	int curX = start.x;
	int curY = start.y;
	float curZ = start.z;
	int length = max(std::abs(end.x - start.x), std::abs(end.y - start.y));

	for (int i = 0; i < length; ++i)
	{
		float ratio = (float)i / (float)length;
		curX = Lerp((unsigned int)start.x, (unsigned int)end.x, ratio);
		curY = Lerp((unsigned int)start.y, (unsigned int)end.y, ratio);
		curZ = Lerp(start.z, end.z, ratio);
		DrawPxl(TwoDtoOneD(curX, curY), color, curZ);
	}
}
Point2D NDCtoScreen(Vertex v)
{
	Point2D result;
	result.x = (v.xyzw[0] + 1) * (screenWidth / 2);
	result.y = ((1 - v.xyzw[1]) * (screenHeight / 2));
	result.z = v.xyzw[2];
	result.u = v.u;
	result.v = v.v;
	return result;
}
void TDrawTriangle(Vertex x, Vertex y, Vertex z, const unsigned int* img, const unsigned int* imgWidth, const unsigned int* imgHeight)
{
	Point2D px = NDCtoScreen(x);
	Point2D py = NDCtoScreen(y);
	Point2D pz = NDCtoScreen(z);
	Point2D p;

	float direction = ImplicitLine(px, py, pz);
	if (direction < 0)
	{
		return;
	}

	int startX = min(px.x, py.x);
	startX = min(startX, pz.x);
	int startY = min(px.y, py.y);
	startY = min(startY, pz.y);
	int endX = max(px.x, py.x);
	endX = max(endX, pz.x);
	int endY = max(px.y, py.y);
	endY = max(endY, pz.y);

	for (int i = startY; i < endY; ++i)
	{
		for (int j = startX; j < endX; ++j)
		{
			p.x = j;
			p.y = i;
			Bary bar = FindBary(px, py, pz, p);
			if (bar.alpha >= 0 && bar.alpha <= 1 && bar.beta >= 0 && bar.beta <= 1 && bar.gamma >= 0 && bar.gamma <= 1)
			{
				DrawPxl(TwoDtoOneD(j, i), ColorShader2(img, imgWidth, imgHeight, x,y,z, bar), bar.depth);
			}
		}
	}
}

void DrawTriangle(Vertex x, Vertex y, Vertex z,unsigned int color)
{
	Point2D px = NDCtoScreen(x);
	Point2D py = NDCtoScreen(y);
	Point2D pz = NDCtoScreen(z);
	Point2D p;

	float direction = ImplicitLine(px, py, pz);
	if (direction < 0)
	{
		return;
	}

	int startX = min(px.x, py.x);
	startX = min(startX, pz.x);
	int startY = min(px.y, py.y);
	startY = min(startY, pz.y);
	int endX = max(px.x, py.x);
	endX = max(endX, pz.x);
	int endY = max(px.y, py.y);
	endY = max(endY, pz.y);

	

	for (int i = startY; i < endY; ++i)
	{
		for (int j = startX; j < endX; ++j)
		{
			p.x = j;
			p.y = i;
			Bary bar = FindBary(px, py, pz, p);
			if (bar.alpha >= 0 && bar.alpha <= 1 && bar.beta >= 0 && bar.beta <= 1 && bar.gamma >= 0 && bar.gamma <= 1)
			{
				DrawPxl(TwoDtoOneD(j, i), color, bar.depth);
			}
		}
	}
}

void DrawGrid(Grid g, Camera* c)
{
	for (int i = 0; i < g.tops.size(); ++i)
	{
		LinePara(NDCtoScreen(VertShader(g.tops[i], g.local, c)), NDCtoScreen(VertShader(g.bottoms[i], g.local, c)), g.tops[i].color);
	}
	for (int j = 0; j < g.lefts.size(); ++j)
	{
		LinePara(NDCtoScreen(VertShader(g.lefts[j], g.local, c)), NDCtoScreen(VertShader(g.rights[j], g.local, c)), g.lefts[j].color);
	}
}

void DrawCube(Cube c, Camera* cam)
{
	LinePara(NDCtoScreen(VertShader(c.vertsChange[0], c.local, cam)), NDCtoScreen(VertShader(c.vertsChange[2], c.local, cam)), c.vertsChange[0].color);
	LinePara(NDCtoScreen(VertShader(c.vertsChange[0], c.local, cam)), NDCtoScreen(VertShader(c.vertsChange[1], c.local, cam)), c.vertsChange[0].color);
	LinePara(NDCtoScreen(VertShader(c.vertsChange[0], c.local, cam)), NDCtoScreen(VertShader(c.vertsChange[4], c.local, cam)), c.vertsChange[0].color);

	LinePara(NDCtoScreen(VertShader(c.vertsChange[3], c.local, cam)), NDCtoScreen(VertShader(c.vertsChange[2], c.local, cam)), c.vertsChange[0].color);
	LinePara(NDCtoScreen(VertShader(c.vertsChange[3], c.local, cam)), NDCtoScreen(VertShader(c.vertsChange[1], c.local, cam)), c.vertsChange[0].color);
	LinePara(NDCtoScreen(VertShader(c.vertsChange[3], c.local, cam)), NDCtoScreen(VertShader(c.vertsChange[7], c.local, cam)), c.vertsChange[0].color);

	LinePara(NDCtoScreen(VertShader(c.vertsChange[6], c.local, cam)), NDCtoScreen(VertShader(c.vertsChange[7], c.local, cam)), c.vertsChange[0].color);
	LinePara(NDCtoScreen(VertShader(c.vertsChange[6], c.local, cam)), NDCtoScreen(VertShader(c.vertsChange[4], c.local, cam)), c.vertsChange[0].color);
	LinePara(NDCtoScreen(VertShader(c.vertsChange[6], c.local, cam)), NDCtoScreen(VertShader(c.vertsChange[2], c.local, cam)), c.vertsChange[0].color);
	
	LinePara(NDCtoScreen(VertShader(c.vertsChange[5], c.local, cam)), NDCtoScreen(VertShader(c.vertsChange[4], c.local, cam)), c.vertsChange[0].color);
	LinePara(NDCtoScreen(VertShader(c.vertsChange[5], c.local, cam)), NDCtoScreen(VertShader(c.vertsChange[7], c.local, cam)), c.vertsChange[0].color);
	LinePara(NDCtoScreen(VertShader(c.vertsChange[5], c.local, cam)), NDCtoScreen(VertShader(c.vertsChange[1], c.local, cam)), c.vertsChange[0].color);
}

void TDrawCube(Cube c, Camera* cam)
{
	DrawTriangle(VertShader(c.vertsChange[0], c.local, cam), VertShader(c.vertsChange[2], c.local, cam), VertShader(c.vertsChange[4], c.local, cam), 0xff009900);
	DrawTriangle(VertShader(c.vertsChange[2], c.local, cam), VertShader(c.vertsChange[6], c.local, cam), VertShader(c.vertsChange[4], c.local, cam), 0xff009900);

	DrawTriangle(VertShader(c.vertsChange[1], c.local, cam), VertShader(c.vertsChange[3], c.local, cam), VertShader(c.vertsChange[5], c.local, cam), 0xff000099);
	DrawTriangle(VertShader(c.vertsChange[7], c.local, cam), VertShader(c.vertsChange[3], c.local, cam), VertShader(c.vertsChange[5], c.local, cam), 0xff000099);

	DrawTriangle(VertShader(c.vertsChange[0], c.local, cam), VertShader(c.vertsChange[1], c.local, cam), VertShader(c.vertsChange[4], c.local, cam), 0xff990000);
	DrawTriangle(VertShader(c.vertsChange[5], c.local, cam), VertShader(c.vertsChange[1], c.local, cam), VertShader(c.vertsChange[4], c.local, cam), 0xff990000);

	DrawTriangle(VertShader(c.vertsChange[2], c.local, cam), VertShader(c.vertsChange[3], c.local, cam), VertShader(c.vertsChange[6], c.local, cam), 0xff999900);
	DrawTriangle(VertShader(c.vertsChange[7], c.local, cam), VertShader(c.vertsChange[3], c.local, cam), VertShader(c.vertsChange[6], c.local, cam), 0xff999900);
}
//void TreeDrawCube(Cube c, Camera* cam)
//{
//	TDrawTriangle(VertShader(c.vertsChange[0], c.local, cam), VertShader(c.vertsChange[2], c.local, cam), VertShader(c.vertsChange[4], c.local, cam), treeolife_pixels, &treeolife_width, &treeolife_height);
//	TDrawTriangle(VertShader(c.vertsChange[2], c.local, cam), VertShader(c.vertsChange[6], c.local, cam), VertShader(c.vertsChange[4], c.local, cam), treeolife_pixels, &treeolife_width, &treeolife_height);
//
//	TDrawTriangle(VertShader(c.vertsChange[1], c.local, cam), VertShader(c.vertsChange[3], c.local, cam), VertShader(c.vertsChange[5], c.local, cam), treeolife_pixels, &treeolife_width, &treeolife_height);
//	TDrawTriangle(VertShader(c.vertsChange[7], c.local, cam), VertShader(c.vertsChange[3], c.local, cam), VertShader(c.vertsChange[5], c.local, cam), treeolife_pixels, &treeolife_width, &treeolife_height);
//
//	TDrawTriangle(VertShader(c.vertsChange[8], c.local, cam), VertShader(c.vertsChange[9], c.local, cam), VertShader(c.vertsChange[12], c.local, cam), treeolife_pixels, &treeolife_width, &treeolife_height);
//	TDrawTriangle(VertShader(c.vertsChange[13], c.local, cam), VertShader(c.vertsChange[9], c.local, cam), VertShader(c.vertsChange[12], c.local, cam), treeolife_pixels, &treeolife_width, &treeolife_height);
//
//	TDrawTriangle(VertShader(c.vertsChange[10], c.local, cam), VertShader(c.vertsChange[11], c.local, cam), VertShader(c.vertsChange[14], c.local, cam), treeolife_pixels, &treeolife_width, &treeolife_height);
//	TDrawTriangle(VertShader(c.vertsChange[15], c.local, cam), VertShader(c.vertsChange[11], c.local, cam), VertShader(c.vertsChange[14], c.local, cam), treeolife_pixels, &treeolife_width, &treeolife_height);
//}

void DrawStars(StarField s, Camera* cam)
{
	for (int i = 0; i < s.stars.size(); ++i)
	{
		Vertex v = VertShader(s.stars[i], Identity, cam);
		if (v.xyzw[0] >= -1 && v.xyzw[0] <= 1 && v.xyzw[1] >= -1 && v.xyzw[1] <= 1 && v.xyzw[2] >= 0)
		{
			Point2D p = NDCtoScreen(v);
			DrawPxl(TwoDtoOneD(p.x, p.y), white, v.xyzw[2]);
		}
	}
}

void DrawStonehenge(Stonehenge stone, Camera* cam)
{
	for (int i = 0; i < 2532; i += 3)
	{
		TDrawTriangle(VertShader(stone.stones[StoneHenge_indicies[i]], Identity, cam), VertShader(stone.stones[StoneHenge_indicies[i + 1]], Identity, cam),
			VertShader(stone.stones[StoneHenge_indicies[i + 2]], Identity, cam), StoneHenge_pixels, &StoneHenge_width, &StoneHenge_height);
	}
}
void DrawStonehenge2(Stonehenge stone, Camera* cam, DirLight d, PosLight* p)
{
	for (int i = 0; i < 2532; i += 3)
	{
		TDrawTriangle(VertShader2(stone.stones[StoneHenge_indicies[i]], Identity, cam, d, p), VertShader2(stone.stones[StoneHenge_indicies[i + 1]], Identity, cam, d, p), 
			VertShader2(stone.stones[StoneHenge_indicies[i + 2]], Identity, cam, d, p), StoneHenge_pixels, &StoneHenge_width, &StoneHenge_height);
	}
}