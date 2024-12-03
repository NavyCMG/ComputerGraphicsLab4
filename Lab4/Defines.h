#pragma once
#include <vector>
#include <math.h>
#include "Helper.H"
#include "Helper.H"
#include "StoneHenge.h"
#include "StoneHenge_Texture.h"

#define screenWidth 1600
#define screenHeight 900
#define numPxls (screenWidth * screenHeight)
#define pi 3.14159265358979323846
#define aspectratio ((float)screenHeight/ (float)screenWidth)
#define yscale (std::cos(.785)/ std::sin(.785))
#define xscale (yscale * aspectratio)
#define znear .1
#define zfar 10
#define white 0xffffffff
#define black 0x00000000
unsigned int screen[numPxls];
float screenZ[numPxls]{-1,};
unsigned int curPxl = 0;
float test1[4][4]{ {1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16} };
float test2[4][4]{ {17,18,19,20},{21,22,23,24},{25,26,27,28},{29,30,31,32} };
float testInv[4][4]{ {1,0,0,0},{0,.309,.951,0}, {0, -.951, .309, 0}, {7, -2.282, 9.513, 1} };
float Identity[4][4]{ {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1} };


struct Bary
{
	Bary(float a, float b, float c, float z)
	{
		alpha = a;
		beta = b;
		gamma = c;
		depth = z;
	}
	float alpha;
	float beta;
	float gamma;
	float depth;
};

struct Vertex
{
	Vertex(float x = 0, float y = 0, float z = 0, float w = 1, unsigned int clr = 0xffffffff, float U = 0, float V = 0, float Xnorm = 0, float Ynorm = 0, float Znorm = 0)
	{
		xyzw[0] = x;
		xyzw[1] = y;
		xyzw[2] = z;
		xyzw[3] = w;
		color = clr;
		u = U;
		v = V;
		norm[0] = Xnorm;
		norm[1] = Ynorm;
		norm[2] = Znorm;
	}
	float norm[3];
	float xyzw[4];
	unsigned int color;
	float u, v;
};
struct Point2D
{
	Point2D(){}
	Point2D(int X, int Y)
	{
		x = X;
		y = Y;
	}
	int x, y;
	float u,v,z;
};
struct Grid //color white
{
	float local[4][4] { {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1} };;
	//total size should go from -.5 to .5
	std::vector<Vertex> bottoms = {
		Vertex(-.5, 0,-.5, 1, 0xffffffff),
		Vertex(-.4, 0, -.5, 1, 0xffffffff),
		Vertex(-.3, 0, -.5, 1, 0xffffffff),
		Vertex(-.2, 0, -.5, 1, 0xffffffff),
		Vertex(-.1, 0, -.5, 1, 0xffffffff),
		Vertex(0, 0, -.5, 1, 0xffffffff),
		Vertex(.1 , 0, -.5, 1, 0xffffffff),
		Vertex(.2, 0, -.5, 1, 0xffffffff),
		Vertex(.3, 0, -.5, 1, 0xffffffff),
		Vertex(.4, 0, -.5, 1, 0xffffffff),
		Vertex(.5, 0, -.5, 1, 0xffffffff)
	};
	std::vector<Vertex> lefts = {
		Vertex(-.5, 0, -.5, 1, 0xffffffff),
		Vertex(-.5, 0, -.4, 1, 0xffffffff),
		Vertex(-.5, 0, -.3, 1, 0xffffffff),
		Vertex(-.5, 0, -.2, 1, 0xffffffff),
		Vertex(-.5, 0, -.1, 1, 0xffffffff),
		Vertex(-.5, 0, 0, 1, 0xffffffff),
		Vertex(-.5, 0, .1, 1, 0xffffffff),
		Vertex(-.5, 0, .2, 1, 0xffffffff),
		Vertex(-.5, 0, .3, 1, 0xffffffff),
		Vertex(-.5, 0, .4, 1, 0xffffffff),
		Vertex(-.5, 0, .5, 1, 0xffffffff)
	};
	std::vector<Vertex> tops = {
		Vertex(-.5, 0, .5, 1, 0xffffffff),
		Vertex(-.4, 0, .5, 1, 0xffffffff),
		Vertex(-.3, 0, .5, 1, 0xffffffff),
		Vertex(-.2, 0, .5, 1, 0xffffffff),
		Vertex(-.1, 0, .5, 1, 0xffffffff),
		Vertex(0, 0, .5, 1, 0xffffffff),
		Vertex(.1, 0, .5, 1, 0xffffffff),
		Vertex(.2, 0, .5, 1, 0xffffffff),
		Vertex(.3, 0, .5, 1, 0xffffffff),
		Vertex(.4, 0, .5, 1, 0xffffffff),
		Vertex(.5, 0, .5, 1, 0xffffffff)
	};
		std::vector<Vertex> rights = {
		Vertex(.5, 0, -.5, 1, 0xffffffff),
		Vertex(.5, 0, -.4, 1, 0xffffffff),
		Vertex(.5, 0, -.3, 1, 0xffffffff),
		Vertex(.5, 0, -.2, 1, 0xffffffff),
		Vertex(.5, 0, -.1, 1, 0xffffffff),
		Vertex(.5, 0, 0, 1, 0xffffffff),
		Vertex(.5, 0, .1, 1, 0xffffffff),
		Vertex(.5, 0, .2, 1, 0xffffffff),
		Vertex(.5, 0, .3, 1, 0xffffffff),
		Vertex(.5, 0, .4, 1, 0xffffffff),
		Vertex(.5, 0, .5, 1, 0xffffffff)
	};
};
struct Cube //color green
{
	float local[4][4]{ {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1} };;
	//total size should go from -.25 to .25
	std::vector<Vertex> vertsOrig{ 
		Vertex(-.25, .25, .25, 1, 0xff00ff00),	//top left close	sub 0
		Vertex(-.25, .25, -.25, 1, 0xff00ff00), //top left far		sub 1
		Vertex(.25, .25, .25, 1, 0xff00ff00),	//top right close	sub 2
		Vertex(.25, .25, -.25, 1, 0xff00ff00),	//top right far		sub 3
		Vertex(-.25, -.25, .25, 1, 0xff00ff00),	//bottom left close	sub 4
		Vertex(-.25, -.25, -.25, 1, 0xff00ff00),//bottom left far	sub 5
		Vertex(.25, -.25, .25, 1, 0xff00ff00),	//bottom right close sub 6
		Vertex(.25, -.25, -.25, 1, 0xff00ff00)  //bottom right far	sub 7
	};
	std::vector<Vertex> vertsChange{
		Vertex(-.25, .25, .25, 1, 0xff00ff00, 0, 0),	//top left far		sub 0
		Vertex(-.25, .25, -.25, 1, 0xff00ff00, 0, 0),	//top left close	sub 1
		Vertex(.25, .25, .25, 1, 0xff00ff00, 1, 0),		//top right far		sub 2
		Vertex(.25, .25, -.25, 1, 0xff00ff00, 1, 0),	//top right close	sub 3
		Vertex(-.25, -.25, .25, 1, 0xff00ff00, 0, 1),	//bottom left far	sub 4
		Vertex(-.25, -.25, -.25, 1, 0xff00ff00, 0, 1),//bottom left close	sub 5
		Vertex(.25, -.25, .25, 1, 0xff00ff00, 1, 1),	//bottom right far	sub 6
		Vertex(.25, -.25, -.25, 1, 0xff00ff00, 1, 1),  //bottom right close	sub 7
		//duplicate verts for sides
		Vertex(-.25, .25, .25, 1, 0xff00ff00, 0, 0),	//top left far		sub 8
		Vertex(-.25, .25, -.25, 1, 0xff00ff00, 1, 0),	//top left close	sub 9
		Vertex(.25, .25, .25, 1, 0xff00ff00, 1, 0),		//top right far		sub 10
		Vertex(.25, .25, -.25, 1, 0xff00ff00, 0, 0),	//top right close	sub 11
		Vertex(-.25, -.25, .25, 1, 0xff00ff00, 0, 1),	//bottom left far	sub 12
		Vertex(-.25, -.25, -.25, 1, 0xff00ff00, 1, 1),//bottom left close	sub 13
		Vertex(.25, -.25, .25, 1, 0xff00ff00, 1, 1),	//bottom right far	sub 14
		Vertex(.25, -.25, -.25, 1, 0xff00ff00, 0, 1)  //bottom right close	sub 15
	};
};
struct Camera
{
	float local[4][4]{ {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1} };
	float inverse[4][4]{ {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1} };
	float perspectivePorj[4][4]{ {(float)xscale, 0,0,0}, {0, (float)yscale, 0,0}, {0,0, (float)(zfar / (zfar - znear)), 1},{0,0, (float)-(zfar * znear) / (zfar - znear), 0} };
};

struct Stonehenge
{
	std::vector<Vertex> stones;
	Stonehenge() 
	{
		for (int i = 0; i < 1457; ++i)
		{
			stones.push_back(Vertex(StoneHenge_data[i].pos[0] * .1, StoneHenge_data[i].pos[1] *.1, StoneHenge_data[i].pos[2] * .1, 1, 0xfffffff, StoneHenge_data[i].uvw[0], StoneHenge_data[i].uvw[1],
				StoneHenge_data[i].nrm[0], StoneHenge_data[i].nrm[1], StoneHenge_data[i].nrm[2]));
		}
	}
};

struct StarField
{
	std::vector<Vertex> stars;
	StarField(int ammount)
	{
		for (int i = 0; i < ammount; ++i)
		{
			stars.push_back(Vertex(Helper::RNfloat(-1, 1)*50, Helper::RNfloat(-1, 1)*50, Helper::RNfloat(-1, 1)*50));
		}
	}
};

struct PosLight
{
	Vertex light = Vertex(-1, .5, 1, 1, 0xffffff00);
	float radius = 2;
};

struct DirLight
{
	float dir[3]{ .577, .577, -.577 };
	unsigned int color = 0xffc0c0f0;
};