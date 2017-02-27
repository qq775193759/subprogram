#include "graph3d.h"
using namespace std;

//******2d******

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

vector<vector<int> > Voxel_2d::get_data()
{
	return _data;
}

vector<vector<int> > Voxel_2d::get_empty_data()
{
	vector<vector<int> > res = _data;
	for(int i=0;i<res.size();i++)
		for(int j=0;j<res[i].size();j++)
			res[i][j] = 0;
	return res;
}

int count_voxel(vector<vector<int> > x)
{
	int res = 0;
	for(int i=0;i<x.size();i++)
		for(int j=0;j<x[i].size();i++)
			res += x[i][j];
	return res;
}

vector<Voxel_2d> Voxel_2d::continuous_2d()
{
	vector<Voxel_2d> res;
	vector<vector<int> > tmp_data = _data;
	int voxel_num = count_voxel(tmp_data);
	while(voxel_num > 0)
	{
		vector<vector<int> > tmp_continuous = get_empty_data();
	}
	return res;
}


//for Voxel_3d::Voxel_3d(const char* filename)
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

//******3d******

//using splitLine(string s)
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