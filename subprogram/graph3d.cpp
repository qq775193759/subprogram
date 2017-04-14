#include "graph3d.h"
#include<iomanip>
using namespace std;

//******2d******

void Voxel_2d::print()
{
	for(int i=0;i<_data.size();i++)
	{
		for(int j=0;j<_data[i].size();j++)
			cout<<setw(2)<<_data[i][j]<<" ";
		cout<<endl;
	}
	cout<<endl;
}

void Voxel_2d::fix_single()
{
	//wait for finishing
}

//for continuous_2d()
//from 2 to part+1, return part+1
int union_label_2d(vector<vector<int> > &x)
{
	int res = 2;
	int root_no = 2;
	vector<int> union_root(2);
	for(int i=0;i<x.size();i++)
		for(int j=0;j<x[i].size();j++)
		{
			if(x[i][j] == VOXEL_3D_EXIST)
			{
				if(j>0 && x[i][j-1]>0)
				{
					if(i>0 && x[i-1][j]>0)
					{
						int tmp_min = min(union_root[x[i][j-1]], union_root[x[i-1][j]]);
						union_root[x[i][j-1]] = tmp_min;
						union_root[x[i-1][j]] = tmp_min;
						x[i][j] = tmp_min;
					}
					else
					{
						x[i][j] = union_root[x[i][j-1]];
					}
				}
				else if(i>0 && x[i-1][j]>0)
				{
					x[i][j] = union_root[x[i-1][j]];
				}
				else
				{
					x[i][j] = root_no;
					union_root.push_back(root_no);
					root_no ++;
				}
			}
		}
	for(int i=2;i<union_root.size();i++)
	{
		if(union_root[i] == i)
		{
			union_root[i] = res;
			res++;
		}
		else
			union_root[i] = union_root[union_root[i]];
	}
	for(int i=0;i<x.size();i++)
		for(int j=0;j<x[i].size();j++)
			x[i][j] = union_root[x[i][j]];
	return res;
}

//for continuous_2d()
vector<vector<int> > find_label_2d(vector<vector<int> > res, int y)
{
	for(int i=0;i<res.size();i++)
		for(int j=0;j<res[i].size();j++)
		{
			if(res[i][j] != y)
				res[i][j] = VOXEL_3D_VOID;
			else
				res[i][j] = VOXEL_3D_EXIST;
		}
	return res;
}

vector<Voxel_2d> Voxel_2d::continuous_2d()
{
	vector<Voxel_2d> res;
	vector<vector<int> > tmp_data = _data;
	int union_num = union_label_2d(tmp_data);
	for(int i=2;i<union_num;i++)
	{
		Voxel_2d tmp_2d(find_label_2d(tmp_data, i));
		res.push_back(tmp_2d);
	}
	return res;
}

Voxel_2d Voxel_2d::find_edge()
{
	Voxel_2d res(_data);
	const int dx[4] = {-1,-1,1,1};
	const int dy[4] = {-1,1,-1,1};
	for(int i=1;i<(_data.size()-1);i++)
		for(int j=1;j<(_data[i].size()-1);j++)
			if(_data[i][j] == 1)
			{
				int co_void = 0, co_exist = 0;
				for(int k=0;k<4;k++)
				{
					if(_data[i+dx[k]][j+dy[k]] == VOXEL_3D_VOID)
						co_void++;
					else
						co_exist++;
				}
				if(co_void && co_exist);
				else
					res._data[i][j] = VOXEL_3D_VOID;
			}
	return res;
}

Voxel_2d Voxel_2d::substract(Voxel_2d x)
{
	Voxel_2d res(_data);
	for(int i=0;i<_data.size();i++)
		for(int j=0;j<_data[i].size();j++)
			if(x._data[i][j] == VOXEL_3D_EXIST)
				res._data[i][j] = VOXEL_3D_VOID;
	return res;
}

Voxel_2d Voxel_2d::find_circle()
{
	Voxel_2d res(_data);
	Voxel_2d rest = res;
	vector<Voxel_2d> edge_vec;
	while(rest.count_type(VOXEL_3D_EXIST) > 0)
	{
		Voxel_2d tmp_edge = rest.find_edge();
		edge_vec.push_back(tmp_edge);
		rest = rest.substract(tmp_edge);
	}
	return res;
}

//for build a reeb tree
int Voxel_2d::count_overlap(Voxel_2d tar)
{
	int res = 0;
	for(int i=0;i<_data.size();i++)
		for(int j=0;j<_data[i].size();j++)
		{
			if((_data[i][j] == VOXEL_3D_EXIST) && (tar._data[i][j] == VOXEL_3D_EXIST))
				res++;
		}
	return res;
}

Voxel_2d Voxel_2d::build_overlap(Voxel_2d tar)
{
	Voxel_2d res = tar;
	for(int i=0;i<_data.size();i++)
		for(int j=0;j<_data[i].size();j++)
		{
			if(_data[i][j] == 0)
				res._data[i][j]=0;
		}
	return res;
}

int Voxel_2d::count_type(int type)
{
	int res = 0;
	for(int i=0;i<_data.size();i++)
		for(int j=0;j<_data[i].size();j++)
		{
			if(_data[i][j] == type)
				res++;
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