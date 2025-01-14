#include "BitmapBrush.h"
#include "common/tpt-minmax.h"
#include "Misc.h"
#include <cmath>

BitmapBrush::BitmapBrush(unsigned char *newBitmap, ui::Point rectSize_):
	Brush(ui::Point(0, 0)),
	origSize(0, 0)
{
	ui::Point newSize = rectSize_;

	//Ensure the rect has odd dimensions so we can pull an integer radius with a 1x1 centre
	if(!(newSize.X % 2))
		newSize.X += 1;
	if(!(newSize.Y % 2))
		newSize.Y += 1;

	radius = (newSize-ui::Point(1, 1))/2;
	size = newSize;
	origSize = size;

	origBitmap = new unsigned char[size.X*size.Y];
	std::fill(origBitmap, origBitmap+(size.X*size.Y), 0);
	for(int y = 0; y < rectSize_.Y; y++)
	{
		for(int x = 0; x < rectSize_.X; x++)
		{
			if(newBitmap[(y*rectSize_.X)+x] >= 128)
				origBitmap[(y*size.X)+x] = newBitmap[(y*rectSize_.X)+x];
		}
	}

	SetRadius(radius);
};

void BitmapBrush::GenerateBitmap()
{
	if(origBitmap)
	{
		delete[] bitmap;
		bitmap = new unsigned char[size.X*size.Y];
		if(size == origSize)
			std::copy(origBitmap, origBitmap+(origSize.X*origSize.Y), bitmap);
		else
		{
			//Bilinear interpolation
			float factorX = ((float)origSize.X)/((float)size.X);
			float factorY = ((float)origSize.Y)/((float)size.Y);
			for(int y = 0; y < size.Y; y++)
			{
				for(int x = 0; x < size.X; x++)
				{
					float originalY = ((float)y)*factorY;
					float originalX = ((float)x)*factorX;

					auto lowerX = int(std::floor(originalX));
					auto upperX = int(std::min((float)(origSize.X-1), std::floor(originalX+1.0f)));
					auto lowerY = int(std::floor(originalY));
					auto upperY = int(std::min((float)(origSize.Y-1), std::floor(originalY+1.0f)));

					unsigned char topRight = origBitmap[(lowerY*origSize.X)+upperX];
					unsigned char topLeft = origBitmap[(lowerY*origSize.X)+lowerX];
					unsigned char bottomRight = origBitmap[(upperY*origSize.X)+upperX];
					unsigned char bottomLeft = origBitmap[(upperY*origSize.X)+lowerX];
					float top = LinearInterpolate<float>(topLeft, topRight, float(lowerX), float(upperX), originalX);
					float bottom = LinearInterpolate<float>(bottomLeft, bottomRight, float(lowerX), float(upperX), originalX);
					float mid = LinearInterpolate<float>(top, bottom, float(lowerY), float(upperY), originalY);
					bitmap[(y*size.X)+x] = mid > 128 ? 255 : 0;
				}
			}
		}
	}
}

BitmapBrush::~BitmapBrush()
{
	delete[] origBitmap;
}
