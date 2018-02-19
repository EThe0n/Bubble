#include "LinkedCell.h"

#include <exception>
#include <memory>

LinkedCell::LinkedCell(int particleNumber, float width, int xCount, int yCount) :
	particleNumber(particleNumber), width(width), xCount(xCount), yCount(yCount)
{
	if (particleNumber <= 0) {
		throw std::exception("Linked Cell> invalid particle number");
	}
	if (width <= 0.0f) {
		throw std::exception("Linked Cell> invalid cell width");
	}
	if (xCount <= 0 || yCount <= 0) {
		throw std::exception("Linked Cell> invalid cell x count or y count");
	}

	cellNumber = xCount * yCount;
	digit = (int)log10(cellNumber) + 1;

	neighborCellIndex		= new int*[cellNumber];
	neighborCellSize		= new int[cellNumber];
	particleIndex			= new int[particleNumber + 1];
	tempParticleIndex		= new int[particleNumber + 1];
	cellIndex				= new int[particleNumber + 1];
	tempCellIndex			= new int[particleNumber + 1];
	cellStartIndex			= new int[cellNumber];
	particleNumberInCell	= new int[cellNumber];

	for (register int i = 1; i <= particleNumber; ++i) {
		particleIndex[i] = i - 1;
	}
	memset(cellIndex, 0, sizeof(int) * (particleNumber + 1));

	initNeighborCellIndex();
}

LinkedCell::~LinkedCell()
{
	for (register int i = 0; i < cellNumber; ++i) {
		delete[] neighborCellIndex[i];
	}
	delete[] neighborCellIndex;
	delete[] neighborCellSize;
	delete[] particleIndex;
	delete[] tempParticleIndex;
	delete[] cellIndex;
	delete[] tempCellIndex;
	delete[] cellStartIndex;
	delete[] particleNumberInCell;
}

int LinkedCell::getIdx(float x, float y)
{
	unsigned int x_idx = x / width;
	unsigned int y_idx = y / width;

	if (x_idx >= xCount || y_idx >= yCount) {
		return -1;
	}

	return x_idx + y_idx * xCount;
}

void LinkedCell::clear()
{
	if (cellNumber <= 0) {
		throw std::exception("Linked Cell> invalid number of cells");
	}


}

void LinkedCell::update(float* position)
{
	register int x, y;
	register int idx;

	memset(particleNumberInCell, 0, sizeof(int) * cellNumber);
	memset(cellStartIndex, 0, sizeof(int) * cellNumber);

	for (register int i = 1; i <= particleNumber; ++i) {
		x = particleIndex[i] * 2;
		y = x + 1;

		try {
			idx = getIdx(position[x], position[y]);
		}
		catch (std::exception& e) {
			throw e.what();
		}

		cellIndex[i] = idx;
		particleNumberInCell[idx]++;
	}

	// cell index에 대해서 정렬
	radixSort();

	// cell start index 구하기
	register int iter = 1;
	for (register int i = 0; i < cellNumber; ++i) {
		if (particleNumberInCell[i] != 0) {
			cellStartIndex[i] = iter;
			iter += particleNumberInCell[i];
		}
	}
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

				if (_x < xCount && _y < yCount) {
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

void LinkedCell::radixSort()
{
	for (register int i = 1; i <= digit; ++i) {
		countingSort(i);
	}
}

void LinkedCell::countingSort(int point)
{
	int c[10] = { 0 };
	const int bytes = sizeof(int) * (particleNumber + 1);
	register int mod = 1;
	while (point--) {
		mod *= 10;
	}
	register int div = mod / 10;

	memset(tempParticleIndex, 0, bytes);
	memset(tempCellIndex, 0, bytes);

	for (register int i = 1; i <= particleNumber; ++i) {
		c[cellIndex[i] % mod / div]++;
	}
	for (register int i = 1; i <= 9; ++i) {
		c[i] += c[i - 1];
	}
	register int idx;
	for (register int i = particleNumber; i >= 1; --i) {
		idx = c[cellIndex[i] % mod / div]--;
		tempCellIndex[idx] = cellIndex[i];
		tempParticleIndex[idx] = particleIndex[i];
	}

	memcpy(particleIndex, tempParticleIndex, bytes);
	memcpy(cellIndex, tempCellIndex, bytes);
}
