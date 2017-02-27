#include "graph3d.h"
using namespace std;

int main()
{
	Voxel_3d bunny_3d("voxel/bunny.txt");
	auto bunny_2d = bunny_3d.trans_2d();
	return 0;
}