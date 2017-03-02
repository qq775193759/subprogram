#include "graph3d.h"
using namespace std;

void test_union(int times);

int main()
{
	Voxel_3d bunny_3d("voxel/bunny.txt");
	vector<Voxel_2d> bunny_2d = bunny_3d.trans_2d();
	
	for(int i=0;i<bunny_2d.size();i++)
	{
		vector<Voxel_2d> plane = bunny_2d[i].continuous_2d();
		cout<<plane.size()<<endl;
		for(int j=0;j<plane.size();j++)
		{
			plane[j].print();
		}
	}

	int x;cin>>x;
	return 0;
}



void test_union(int times)
{
	for(int times=0;times<2;times++)
	{
		vector<vector<int> > test;
		for(int i=0;i<15;i++)
		{
			vector<int> tmp;
			for(int j=0;j<12;j++)
			{
				tmp.push_back(rand()%2);
			}
			test.push_back(tmp);
		}
		union_label_2d(test);
		Voxel_2d test_2d(test);
		test_2d.print();
	}
}