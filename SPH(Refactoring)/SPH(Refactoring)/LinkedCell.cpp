#include "LinkedCell.h"

#include <exception>
#include <memory>

LinkedCell::LinkedCell(float width, int xCount, int yCount) :
	width(width), xCount(xCount), yCount(yCount)
{
	if (width <= 0.0f) {
		throw std::exception("Linked Cell> invalid cell width");
	}
	if (xCount <= 0 || yCount <= 0) {
		throw std::exception("Linked Cell> invalid cell x count or y count");
	}

	cellNumber = xCount * yCount;

	register int bytes = sizeof(int) * cellNumber;

	neighborCellIndex	= new int*[cellNumber];
	neighborCellSize	= new int[cellNumber];
	particleIndex		= new int*[cellNumber];
	particleIndexSize	= new int[cellNumber];
	
	memset(neighborCellSize, 0, bytes);
	memset(particleIndexSize, 0, bytes);

	initNeighborCellIndex();
}

LinkedCell::~LinkedCell()
{
	for (register int i = 0; i < cellNumber; ++i) {
		delete neighborCellIndex[i];
		delete particleIndex[i];
	}
	delete neighborCellIndex;
	delete neighborCellSize;
	delete particleIndex;
	delete particleIndexSize;
}

inline int LinkedCell::getIdx(float x, float y)
{
	unsigned int x_idx = x / width;
	unsigned int y_idx = y / width;

	if (x_idx >= xCount || y_idx >= yCount) {
		throw std::exception("cell out of bound");
	}

	return x_idx + y_idx * xCount;
}

void LinkedCell::clear()
{
	if (cellNumber <= 0) {
		throw std::exception("Linked Cell> invalid number of cells");
	}

	for (register int i = 0; i < cellNumber; ++i) {
		delete particleIndex[i];
	}
	memset(particleIndexSize, 0, sizeof(int) * cellNumber);
}

void LinkedCell::initNeighborCellIndex()
{
	const int offsetX[] = { 1, -1, 0, 1 };
	const int offsetY[] = { 0, 1, 1, 1 };
	register int index[4];
	register int count = 0;

	for (register int y = 0; y < yCount; ++y) {
		for (register int x = 0; x < xCount; ++x) {
			register int idx = y * xCount + x;
			count = 0;

			for (register int i = 0; i < 4; ++i) {
				register unsigned int _x = x + offsetX[i];
				register unsigned int _y = y + offsetY[i];

				if (_x < xCount || _y < yCount) {
					index[count] = _y * xCount + _x;
					++count;
				}
			}

			neighborCellIndex[idx] = new int[count];
			neighborCellSize[idx] = count;
			memcpy(neighborCellIndex[idx], index, sizeof(int) * count);
		}
	}
}
