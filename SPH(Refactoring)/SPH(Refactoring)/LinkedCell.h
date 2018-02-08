#pragma once

class LinkedCell
{
public :
	LinkedCell(int particleNumber, float width, int xCount, int yCount);
	~LinkedCell();

	int getIdx(float x, float y);
	void clear();
	void update(float* position);

public :
	int**	neighborCellIndex;		// 각 셀의 자신보다 큰 인덱스를 가지고 있는 주변 셀의 인덱스.
	int*	neighborCellSize;		// 주변 셀의 인덱스 시작지점.
	int*	particleIndex;			// 각 파티클의 인덱스. 셀 인덱스에 따라 정렬됨. 1부터 사용함.
	int*	cellIndex;				// 각 파티클의 셀 인덱스. 정렬됨. 1부터 사용함.
	int*	cellStartIndex;			// 각 셀의 시작 인덱스 지점. 끝을 표시하기 위해 뒤에 엘리멘트 하나 추가.
	int*	particleNumberInCell;	// 각 셀에 있는 파티클 수

private :
	int		digit;				// 자릿수
	float	width;
	int		xCount;
	int		yCount;
	int		cellNumber;
	int		particleNumber;
	int*	tempParticleIndex;	// new delete를 최소화하기 위함.
	int*	tempCellIndex;		// new delete를 최소화하기 위함.

private :
	void initNeighborCellIndex();
	void radixSort();
	void countingSort(int point);

};