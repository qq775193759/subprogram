#include "graph3d.h"
using namespace std;

int main()
{
	Voxel_3d bunny_3d("voxel/bunny.txt");
	vector<Voxel_2d> bunny_2d = bunny_3d.trans_2d();

	

	int x;cin>>x;
	return 0;
}