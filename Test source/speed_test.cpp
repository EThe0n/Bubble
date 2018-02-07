#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>

using namespace std;
using chrono::system_clock;

#define PARTICLE_NUMBER 10000
#define GRID_X_COUNT	30
#define GRID_Y_COUNT	30
#define CELL_SIZE		0.0457f

void method1();
void method2();
void method3();
void radixSort(int* particleIdx, int* cellIdx, int d);
void countSort(int* particleIdx, int* cellIdx, int d, int* temp_pIdx, int* temp_cIdx);

void initRandom(float* position);

inline int getIdx(float x, float y)
{
	unsigned int x_idx = x / CELL_SIZE;
	unsigned int y_idx = y / CELL_SIZE;

	if (x_idx >= GRID_X_COUNT || y_idx >= GRID_Y_COUNT) {
		throw std::exception("cell out of bound");
	}

	return x_idx + y_idx * GRID_X_COUNT;
}

int main()
{	
	//method1();

	//method2();
	
	method3();

	return 0;
}

void method1()
{
	vector<vector<int>> linkedCell;
	float* position = new float[PARTICLE_NUMBER * 2];
	const int cell_size = GRID_X_COUNT * GRID_Y_COUNT;
	linkedCell.resize(cell_size);
	initRandom(position);

	register int x, y;
	register int step = 2;
	auto start = system_clock::now();

	while (step--) {
		for (register int i = 0; i < PARTICLE_NUMBER; ++i) {
			x = i * 2;
			y = x + 1;

			linkedCell[getIdx(position[x], position[y])].push_back(i);
			for (register int j = 0; j < cell_size; ++j) {
				linkedCell[j].clear();
			}
		}
	}

	auto end = system_clock::now();
	chrono::duration<double> elapsedTime = end - start;
	printf("vector<vector<int>> : %.6lfs \n", elapsedTime.count());

	delete position;
}

void method2()
{
	typedef struct {
		int particleIndex;
		int cellIndex;
	} Cell;
	float* position = new float[PARTICLE_NUMBER * 2];
	const int cell_size = GRID_X_COUNT * GRID_Y_COUNT;
	const int bytes = sizeof(int) * cell_size;

	Cell* cell = new Cell[PARTICLE_NUMBER];
	int* cellSize = new int[cell_size];

	initRandom(position);

	register int x, y, idx;
	register int step = 2;
	auto start = system_clock::now();

	for (register int i = 0; i < PARTICLE_NUMBER; ++i) {
		cell[i].particleIndex = i;
	}

	while (step--) {
		memset(cellSize, 0, bytes);
		for (register int i = 0; i < PARTICLE_NUMBER; ++i) {
			x = cell[i].particleIndex * 2;
			y = x + 1;

			idx = getIdx(position[x], position[y]);
			cell[i].cellIndex = idx;
			cellSize[idx]++;
		}
		sort(cell, cell + PARTICLE_NUMBER, [&](Cell& a, Cell& b) {
			return a.cellIndex < b.cellIndex;
		});
	}

	auto end = system_clock::now();
	chrono::duration<double> elapsedTime = end - start;
	printf("integer array : %.6lfs \n", elapsedTime.count());

	delete cellSize;
	delete position;
	delete cell;
}

void method3()
{
	float* position = new float[PARTICLE_NUMBER * 2];
	const int cell_size = GRID_X_COUNT * GRID_Y_COUNT;
	const int bytes = sizeof(int) * cell_size;

	initRandom(position);

	int* cellIdx = new int[PARTICLE_NUMBER + 1];
	int* particleIdx = new int[PARTICLE_NUMBER + 1];
	register int step = 10;

	auto start = system_clock::now();
	while (step--) {
		for (register int i = 1; i <= PARTICLE_NUMBER; ++i) {
			cellIdx[i] = rand() % cell_size;
		}

		radixSort(particleIdx, cellIdx, (int)log10(cell_size) + 1);
	}
	auto end = system_clock::now();
	chrono::duration<double> elapsedTime = end - start;
	printf("%.6lfs \n", elapsedTime.count());


	step = 10;
	start = system_clock::now();
	while (step--) {
		for (register int i = 1; i <= PARTICLE_NUMBER; ++i) {
			cellIdx[i] = rand() % cell_size;
		}

		sort(cellIdx + 1, cellIdx + PARTICLE_NUMBER);
	}
	end = system_clock::now();
	elapsedTime = end - start;
	printf("%.6lfs \n", elapsedTime.count());

	delete position;
}

void radixSort(int * particleIdx, int * cellIdx, int d)
{	
	int* temp_pIdx = new int[PARTICLE_NUMBER + 1];
	int* temp_cIdx = new int[PARTICLE_NUMBER + 1];

	for (register int i = 1; i <= d; ++i) {
		countSort(particleIdx, cellIdx, i, temp_pIdx, temp_cIdx);
	}

	delete temp_pIdx;
	delete temp_cIdx;
}

void countSort(int* particleIdx, int* cellIdx, int d, int* temp_pIdx, int* temp_cIdx)
{
	int c[10] = { 0 };
	const int bytes = sizeof(int) * (PARTICLE_NUMBER + 1);
	register int mod = 1;
	while (d--) {
		mod *= 10;
	}
	register int div = mod / 10;
	
	memset(temp_pIdx, 0, bytes);
	memset(temp_cIdx, 0, bytes);

	for (register int i = 1; i <= PARTICLE_NUMBER; ++i) {
		c[cellIdx[i] % mod / div]++;
	}
	for (register int i = 1; i <= 9; ++i) {
		c[i] += c[i - 1];
	}
	register int idx;
	for (register int i = PARTICLE_NUMBER; i >= 1; --i) {
		idx = c[cellIdx[i] % mod / div]--;
		temp_cIdx[idx] = cellIdx[i];
		temp_pIdx[idx] = particleIdx[i];
	}

	memcpy(particleIdx, temp_pIdx, bytes);
	memcpy(cellIdx, temp_cIdx, bytes);
}

void initRandom(float * position)
{
	register int idx = 0;
	std::random_device rd;
	std::mt19937 e2(rd());
	std::uniform_real_distribution<float> xValue(0.0f, GRID_X_COUNT * CELL_SIZE);
	std::uniform_real_distribution<float> yValue(0.0f, GRID_Y_COUNT * CELL_SIZE);

	for (register int i = 0; i < PARTICLE_NUMBER; ++i) {
		idx = i * 2;
		position[idx] = xValue(e2);
		position[idx + 1] = yValue(e2);
	}
}

