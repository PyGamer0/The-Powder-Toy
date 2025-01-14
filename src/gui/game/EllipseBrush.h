#pragma once
#include "Brush.h"
#include <cmath>

class EllipseBrush: public Brush
{
	bool perfectCircle;

public:
	EllipseBrush(ui::Point size, bool perfectCircle = true):
		Brush(size)
	{
		this->perfectCircle = perfectCircle;
		SetRadius(size);
	}

	void GenerateBitmap() override
	{
		delete[] bitmap;
		bitmap = new unsigned char[size.X*size.Y];
		int rx = radius.X;
		int ry = radius.Y;

		if (!rx)
		{
			for (int j = 0; j <= 2*ry; j++)
			{
				bitmap[j*(size.X)+rx] = 255;
			}
		}
		else
		{
			int yTop = ry+1, yBottom, i;
			for (i = 0; i <= rx; i++)
			{
				if (perfectCircle)
				{
					while (pow(i - rx, 2.0) * pow(ry - 0.5, 2.0) + pow(yTop - ry, 2.0) * pow(rx - 0.5, 2.0) <= pow(rx, 2.0) * pow(ry, 2.0))
						yTop++;
				}
				else
				{
					while (pow(i - rx, 2.0) * pow(ry, 2.0) + pow(yTop - ry, 2.0) * pow(rx, 2.0) <= pow(rx, 2.0) * pow(ry, 2.0))
						yTop++;
				}
				yBottom = 2*ry - yTop;
				for (int j = 0; j <= ry*2; j++)
				{
					if (j > yBottom && j < yTop)
					{
						bitmap[j*(size.X)+i] = 255;
						bitmap[j*(size.X)+2*rx-i] = 255;
					}
					else
					{
						bitmap[j*(size.X)+i] = 0;
						bitmap[j*(size.X)+2*rx-i] = 0;
					}
				}
			}
			bitmap[size.X/2] = 255;
			bitmap[size.X*size.Y-size.X/2-1] = 255;
		}
	}
};
