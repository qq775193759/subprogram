#include "graph3d.h"
#include<iomanip>
#include<queue>
using namespace std;

//******2d******

const int VOXEL_2D_PRINT_OFFSET = 1;
const char VOXEL_2D_PRINT_CHAR[10] = {'X', '0', '*','A','V','<','>','U','D'};

void Voxel_2d::print()
{
	for(int i=0;i<_data.size();i++)
	{
		for(int j=0;j<_data[i].size();j++)
			cout<<setw(1)<<VOXEL_2D_PRINT_CHAR[_data[i][j] + VOXEL_2D_PRINT_OFFSET]<<" ";
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

Voxel_2d Voxel_2d::make_edge()
{
	Voxel_2d res(_data);
	const int dx[4] = {-1,0,0,1};
	const int dy[4] = {0,-1,1,0};
	queue<int> x_queue;
	queue<int> y_queue;
	x_queue.push(0);
	y_queue.push(0);
	while(x_queue.empty() == 0)
	{
		int tmp_x = x_queue.front();x_queue.pop();
		int tmp_y = y_queue.front();y_queue.pop();
		for(int i=0;i<4;i++)
		{
			int x_plus = tmp_x + dx[i];
			int y_plus = tmp_y + dy[i];
			if(x_plus < 0 || x_plus >= _data.size()) continue;
			if(y_plus < 0 || y_plus >= _data[0].size()) continue;
			if(res._data[x_plus][y_plus] == VOXEL_3D_VOID) 
			{
				res._data[x_plus][y_plus] = VOXEL_3D_EDGE;
				x_queue.push(x_plus);
				y_queue.push(y_plus);
			}
		}
	}
	return res;
}

Voxel_2d Voxel_2d::find_edge()
{
	Voxel_2d res(_data);
	Voxel_2d edge_mask = this->make_edge();
	const int dx[8] = {-1,-1,-1,0,0,1,1,1};
	const int dy[8] = {-1,0,1,-1,1,-1,0,1};
	for(int i=1;i<(_data.size()-1);i++)
		for(int j=1;j<(_data[i].size()-1);j++)
			if(_data[i][j] == 1)
			{
				int co_edge = 0, co_exist = 0;
				for(int k=0;k<8;k++)
				{
					if(edge_mask._data[i+dx[k]][j+dy[k]] == VOXEL_3D_EDGE)
						co_edge++;
					else if(edge_mask._data[i+dx[k]][j+dy[k]] == VOXEL_3D_EXIST)
						co_exist++;
				}
				if(co_edge && co_exist);
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
			if(x._data[i][j] >= VOXEL_3D_EXIST)
				res._data[i][j] = VOXEL_3D_VOID;
	return res;
}

Voxel_2d Voxel_2d::find_circle_from_edge()
{
	Voxel_2d res(_data);
	const int dx[4] = {-1,0,0,1};
	const int dy[4] = {0,-1,1,0};
	int exist_num = 0;
	int current_x = 0, current_y = 0;
	for(int i=0;i<_data.size();i++)
		for(int j=0;j<_data[i].size();j++)
			if(_data[i][j] == VOXEL_3D_EXIST)
			{
				exist_num++;
				current_x = i;
				current_y = j;
			}

	return res;
}

Voxel_2d Voxel_2d::find_circle()
{
	Voxel_2d res(_data);
	Voxel_2d rest = res;
	vector<Voxel_2d> edge_circle_vec;
	while(rest.count_type(VOXEL_3D_EXIST) > 0)
	{
		Voxel_2d tmp_edge = rest.find_edge();
		edge_circle_vec.push_back(tmp_edge.find_circle_from_edge());
		rest = rest.substract(tmp_edge);
	}
	return res;
}

vector<neighbor_int4> Voxel_2d::find_neighbor(Voxel_2d tar)
{
	vector<neighbor_int4> res;
	const int dx[4] = {-1,0,0,1};
	const int dy[4] = {0,-1,1,0};
	for(int i=0;i<_data.size();i++)
		for(int j=0;j<_data[i].size();j++)
			if(_data[i][j] >= VOXEL_3D_EXIST)
				for(int k=0;k<4;k++)
					if(tar._data[i+dx[k]][j+dy[k]] >= VOXEL_3D_EXIST)
						res.push_back(neighbor_int4(i,j,i+dx[k],j+dy[k]));
	return res;
}

//for build a reeb tree
int Voxel_2d::count_overlap(Voxel_2d tar)
{
	int res = 0;
	for(int i=0;i<_data.size();i++)
		for(int j=0;j<_data[i].size();j++)
		{
			if((_data[i][j] >= VOXEL_3D_EXIST) && (tar._data[i][j] >= VOXEL_3D_EXIST))
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
			if(_data[i][j] == VOXEL_3D_VOID)
				res._data[i][j] = VOXEL_3D_VOID;
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