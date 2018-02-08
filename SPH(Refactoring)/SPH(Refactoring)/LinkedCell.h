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
	int**	neighborCellIndex;		// �� ���� �ڽź��� ū �ε����� ������ �ִ� �ֺ� ���� �ε���.
	int*	neighborCellSize;		// �ֺ� ���� �ε��� ��������.
	int*	particleIndex;			// �� ��ƼŬ�� �ε���. �� �ε����� ���� ���ĵ�. 1���� �����.
	int*	cellIndex;				// �� ��ƼŬ�� �� �ε���. ���ĵ�. 1���� �����.
	int*	cellStartIndex;			// �� ���� ���� �ε��� ����. ���� ǥ���ϱ� ���� �ڿ� ������Ʈ �ϳ� �߰�.
	int*	particleNumberInCell;	// �� ���� �ִ� ��ƼŬ ��

private :
	int		digit;				// �ڸ���
	float	width;
	int		xCount;
	int		yCount;
	int		cellNumber;
	int		particleNumber;
	int*	tempParticleIndex;	// new delete�� �ּ�ȭ�ϱ� ����.
	int*	tempCellIndex;		// new delete�� �ּ�ȭ�ϱ� ����.

private :
	void initNeighborCellIndex();
	void radixSort();
	void countingSort(int point);

};