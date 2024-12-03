
#include <iostream>
#include "Helper.H"
#include "RasterSurface.h"
#include "XTime.h"
#include "Defines.h"
#include "MyMath.h"
#include "Shaders.h"
#include "RasterFunc.h"

int main()
{
	Camera cam = Camera();
	StarField s = StarField(3000);
	Stonehenge henge = Stonehenge();
	DirLight dir = DirLight();
	PosLight pos = PosLight();
	srand(time(NULL));
	XTime* timer = new XTime();
	timer->Restart();
	float elapTime = 0;
	RotateCameraX(-18, &cam);
	TransposeCamera(&cam, 0, 0, -4, 0);
	OrthogAffinInverseCam(&cam);

	bool dim = true;

	RS_Initialize("Grimes Connor", screenWidth, screenHeight);
	do {
		timer->Signal();
		elapTime += timer->Delta();

		for (int i = 0; i < numPxls; ++i)
		{
			DrawPxl(i, 0xff000000, -1);
		}

		DrawStars(s, &cam);

		if (GetKeyState(VK_LEFT) >> 15) {
			RotateCameraY(-.5, &cam);
			OrthogAffinInverseCam(&cam);
		}
		if (GetKeyState(VK_RIGHT) >> 15) {
			RotateCameraY(.5, &cam);
			OrthogAffinInverseCam(&cam);
		}

		//DrawStonehenge(henge, &cam);
		DrawStonehenge2(henge, &cam, dir, &pos);

		if (elapTime > .033)
		{
			if (dim)
			{
				pos.radius += .012;
				if (pos.radius > 1.5)
				{
					dim = false;
				}
			}
			else if (!dim)
			{
				pos.radius -= .012;
				if (pos.radius < 1.2)
				{
					dim = true;
				}
			}
			elapTime -= .033;
		}

	} while (RS_Update(screen, sizeof(screen) >> 2));
	RS_Shutdown();
}
