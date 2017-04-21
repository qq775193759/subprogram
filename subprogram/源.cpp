#include "reeb.h"
using namespace std;

void test_union(int times);

void main_process(string input_name)
{
	Voxel_3d bunny_3d(input_name.c_str());
	vector<Voxel_2d> bunny_2d = bunny_3d.trans_2d();
	
	reeb_graph bunny_reeb(bunny_2d);
	bunny_reeb.print();
	bunny_reeb.find_circle();
	cout<<"finish finding circle"<<endl; 
}

int main()
{
	ifstream fin("config.txt");
	string input_name;
	int input_cal;
	while(fin>>input_name)
	{
		fin>>input_cal;
		cout<<input_name<<"  "<<input_cal<<endl;
		if(input_cal)
			main_process(input_name);
	}
	fin.close();

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