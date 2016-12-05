#ifndef MISC_FUNCTIONS
#define MISC_FUNCTIONS

#include <stdlib.h>
#include <vector>
#include <map>
#include "Block.h"

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define NULLINT -1
#define REMOVEDBLOCK -2
const double YsPi = 3.1415927;
#define DEGTORAD YsPi/180
#define VISIBLE 0
#define ADD 1
#define REMOVE 0
const int WINWID = 1200, WINHEI = 800;
const int dt = 5;
const double GRAV = -5*(double)dt/1000.0;
const int blockSize = 8;

template <class NumType>
void VecPlus(std::vector<NumType> &first, std::vector<NumType> &second) // add second vec to first
{
	for (int i = 0; i < 3; i++)
	{
		first[i] += second[i];
	}
}

template <class NumType>
void VecPlus(std::vector<NumType> &first, NumType i1, NumType i2, NumType i3) // add second vec to first
{
	first[0] += i1;
	first[1] += i2;
	first[2] += i3;
}

template <class NumType>
void VecMinus(std::vector<NumType> &first, std::vector<NumType> &second) // subtract second vec from first
{
	for (int i = 0; i < 3; i++)
	{
		first[i] -= second[i];
	}
}

template <class NumType>
void VecMinus(std::vector<NumType> &first, NumType i1, NumType i2, NumType i3) // add second vec to first
{
	first[0] -= i1;
	first[1] -= i2;
	first[2] -= i3;
}

template <class NumType>
void SetVec(std::vector<NumType> &first, std::vector<NumType> &second)
{
	for (int i = 0; i < 3; i++)
	{
		first[i] = second[i];
	}
}

template <class NumType>
void SetVec(std::vector<NumType> &first, NumType i1, NumType i2, NumType i3) // add second vec to first
{
	first[0] = i1;
	first[1] = i2;
	first[2] = i3;
}

template <class NumType>
double VecDist(const std::vector<NumType> &first, const std::vector<NumType> &second) // add second vec to first
{
	return sqrt(pow(first[0] - second[0], 2) + pow(first[1] - second[1], 2) + pow(first[2] - second[2], 2));
}

#endif // !MISC_FUNCTIONS
