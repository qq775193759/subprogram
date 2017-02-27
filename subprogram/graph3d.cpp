#include "graph3d.h"
using namespace std;

void Voxel_2d::print()
{
	for(int i=0;i<_data.size();i++)
	{
		for(int j=0;j<_data[i].size();j++)
			cout<<_data[i][j]<<" ";
		cout<<endl;
	}
	cout<<endl;
}

vector<Voxel_2d> Voxel_2d::continuous_2d()
{
	vector<Voxel_2d> res;
	
	return res;
}

vector<int> splitLine(string s)
{
	vector<int> res;
	//split a line
	for(int i=0;i<s.size();i++)
	{
		if(s[i] == '-')
		{
			res.push_back(VOXEL_3D_VOID);
			i+=2;
		}
		else if(s[i] == '\t')
		{
			res.push_back(VOXEL_3D_EXIST);
		}
	}
	return res;
}

Voxel_3d::Voxel_3d(const char* filename)
{
	ifstream fin(filename);
	string str;
	vector<vector<int> > tmp_plane;
	getline(fin, str);//delete the first line to avoid sth
	while(getline(fin, str))
	{
		if(str[0] == 'l')
		{
			_data.push_back(tmp_plane);
			tmp_plane.clear();
		}
		else
		{
			tmp_plane.push_back(splitLine(str));
		}
	}
	_data.push_back(tmp_plane);//push the last plane
	size_x = _data.size();
	size_y = _data[0].size();
	size_z = _data[0][0].size();
	cout<<size_x<<endl;
	cout<<size_y<<endl;
	cout<<size_z<<endl;
}

vector<Voxel_2d> Voxel_3d::trans_2d()
{
	vector<Voxel_2d> res;
	for(int i=0;i<size_x;i++)
	{
		Voxel_2d tmp_voxel_2d(_data[i]);
		res.push_back(tmp_voxel_2d);
	}
	return res;
}