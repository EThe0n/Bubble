#pragma once

class LinkedCell
{
public :
	LinkedCell(float width, int xCount, int yCount);
	~LinkedCell();

	inline int getIdx(float x, float y);
	void clear();

public :
	int**	neighborCellIndex;
	int*	neighborCellSize;
	int**	particleIndex;
	int*	particleIndexSize;
	
private :
	float	width;
	int		xCount;
	int		yCount;
	int		cellNumber;
private :
	void initNeighborCellIndex();
};