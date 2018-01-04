#include "graph3d.h"
#include<iomanip>
#include<queue>
using namespace std;

//******2d******

const int VOXEL_2D_PRINT_OFFSET = 1;
const char VOXEL_2D_PRINT_CHAR[10] = {'X', '0', '*','V','>','A','<','U','D'};

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

void Voxel_2d::check_circle()
{
	cout<<"check st  ";
	const int dx[4] = {1,0,-1,0};
	const int dy[4] = {0,1,0,-1};
	int current_x = 0, current_y = 0;
	int exist_num = 0;
	for(int i=0;i<_data.size();i++)
		for(int j=0;j<_data[i].size();j++)
			if(_data[i][j] > VOXEL_3D_EXIST)
			{
				exist_num++;
				current_x = i;
				current_y = j;
			}
	int st_x = current_x;
	int st_y = current_y;
	while(exist_num)
	{
		exist_num--;
		if(_data[current_x][current_y] <= VOXEL_3D_EXIST)
		{
			cout<<"check break error"<<endl;
			return;
		}
		int next_x = current_x + dx[_data[current_x][current_y]-2];
		int next_y = current_y + dy[_data[current_x][current_y]-2];
		current_x = next_x;
		current_y = next_y;
	}
	if((current_x == st_x) &&(current_y == st_y))
		cout<<"check OK"<<endl;
	else
	{
		cout<<"check error"<<endl;
		cout<<current_x<<"  "<<st_x<<endl;
		this->print();
		int x;cin>>x;
	}
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
		if(tmp_2d.count_type(VOXEL_3D_EXIST)>3)
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
				int co_edge = 0;
				for(int k=0;k<8;k++)
				{
					if(edge_mask._data[i+dx[k]][j+dy[k]] == VOXEL_3D_EDGE)
						co_edge++;
				}
				if(co_edge);
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

vector<Voxel_2d> Voxel_2d::fix_odd_from_edge()
{
	vector<Voxel_2d> res;
	Voxel_2d rest(_data);
	Voxel_2d count = rest.substract(rest);
	const int dx[4] = {1,-1,0,0};
	const int dy[4] = {0,0,1,-1};
	//count the degree of voxel
	for(int i=1;i<(_data.size()-1);i++)
		for(int j=1;j<(_data[i].size()-1);j++)
			if(_data[i][j] >= VOXEL_3D_EXIST)
				for(int k=0;k<4;k++)
					if(_data[i+dx[k]][j+dy[k]] >= VOXEL_3D_EXIST)
						count._data[i][j]++;
	//delete voxel
	queue<int> x_queue;
	queue<int> y_queue;
	for(int i=1;i<(_data.size()-1);i++)
		for(int j=1;j<(_data[i].size()-1);j++)
			if(_data[i][j] >= VOXEL_3D_EXIST)
				if(count._data[i][j] <= 1)
				{
					x_queue.push(i);
					y_queue.push(j);
					rest._data[i][j] = VOXEL_3D_VOID;
				}
	while(x_queue.size()>0)
	{
		int i = x_queue.front();x_queue.pop();
		int j = y_queue.front();y_queue.pop();
		for(int k=0;k<4;k++)
			if(rest._data[i+dx[k]][j+dy[k]] >= VOXEL_3D_EXIST)
			{
				count._data[i+dx[k]][j+dy[k]]--;
				if(count._data[i+dx[k]][j+dy[k]] == 1)
				{
					x_queue.push(i+dx[k]);
					y_queue.push(j+dy[k]);
					rest._data[i+dx[k]][j+dy[k]] = VOXEL_3D_VOID;
				}
			}
	}
	if(rest.count_type(VOXEL_3D_EXIST) > 0)
		res.push_back(rest);
	return res;
}

Voxel_2d Voxel_2d::find_circle_from_edge(int cw)
{
	Voxel_2d res(_data);
	const int dx[4] = {1,0,-1,0};
	const int dy[4] = {0,1,0,-1};
	int exist_num = 0;
	int direction = 0;
	int current_x = 0, current_y = 0;
	for(int i=0;i<_data.size();i++)
		for(int j=0;j<_data[i].size();j++)
			if(_data[i][j] == VOXEL_3D_EXIST)
			{
				exist_num++;
				current_x = i;
				current_y = j;
			}
	while(exist_num)
	{
		exist_num--;
		//cout<<exist_num<<endl;
		for(int i=0;i<3;i++)
		{
			direction = (direction+cw)%4;
			if(res._data[current_x + dx[direction]][current_y + dy[direction]] == VOXEL_3D_EXIST)
			{
				current_x = current_x + dx[direction];
				current_y = current_y + dy[direction];
				direction = (direction+2)%4;//rotate 180 
				res._data[current_x][current_y] = direction + 2;//write back
				break;
			}
		}
	}

	return res;
}

void Voxel_2d::add_circle(Voxel_2d tar)
{
	for(int i=0;i<_data.size();i++)
		for(int j=0;j<_data[i].size();j++)
			if(tar._data[i][j] >= VOXEL_3D_EXIST)
				_data[i][j] = tar._data[i][j];
}

Voxel_2d Voxel_2d::find_circle(int init_cw, int print_flag)
{
	Voxel_2d res(_data);
	Voxel_2d rest = res;
	vector<Voxel_2d> edge_circle_vec;
	//split circle
	int cw = init_cw;
	while(rest.count_type(VOXEL_3D_EXIST) > 0)
	{
		Voxel_2d tmp_edge = rest.find_edge();
		vector<Voxel_2d> circle_and_patch = tmp_edge.fix_odd_from_edge();
		for(int i=0;i<circle_and_patch.size();i++)
			edge_circle_vec.push_back(circle_and_patch[i].find_circle_from_edge(cw));
		cw = 4 - cw;
		rest = rest.substract(tmp_edge);
	}
	//debug print
	if(print_flag)
		for(int i=0;i<edge_circle_vec.size();i++)
			edge_circle_vec[i].print();
	//merge circle
	res = res.substract(res);
	res.add_circle(edge_circle_vec[0]);
	cw = init_cw;
	for(int i=1;i<edge_circle_vec.size();i++)
	{
		vector<neighbor_point4> tmp_neighbor_vec = res.find_neighbor(edge_circle_vec[i], cw);
		//cout<<tmp_neighbor_vec.size()<<endl;
		cw = 4 - cw;
		{
			//res.print();
			//edge_circle_vec[i].print();
		}
		if(tmp_neighbor_vec.size() > 0)
		{
			res.add_circle(edge_circle_vec[i]);
			neighbor_point4 tmp_neighbor = tmp_neighbor_vec[0];
			res._data[tmp_neighbor.sx][tmp_neighbor.sy] = tmp_neighbor.sd;
			res._data[tmp_neighbor.tx][tmp_neighbor.ty] = tmp_neighbor.td;
		}
		else
		{
			//res.print();
			return res;
		}
	}
	//res.print();
	//res.check_circle();
	return res;
}

vector<neighbor_point4> Voxel_2d::find_neighbor(Voxel_2d tar, int cw)
{
	vector<neighbor_point4> res;
	const int dx[4] = {1,-1,-1,1};
	const int dy[4] = {1,1,-1,-1};
	const int voxel_type[4] = {4,5,2,3};
	const int sd_type[4] = {3,4,5,2};
	const int td_type[4] = {5,2,3,4};
	for(int i=0;i<_data.size();i++)
		for(int j=0;j<_data[i].size();j++)
			if(_data[i][j] > VOXEL_3D_EXIST)
			{
				int type_rank = _data[i][j]-2;
				int direct_rank = (type_rank + cw + 1)%4;
				int data_rank = _data[i][j]-2;
				if(cw == 1)
					data_rank = (data_rank+3)%4;
				if(tar._data[i+dx[data_rank]][j+dy[data_rank]] == voxel_type[type_rank])
				{
					neighbor_point4 tmp_point4;
					tmp_point4.sx = i;
					tmp_point4.sy = j;
					tmp_point4.sd = sd_type[direct_rank];
					tmp_point4.tx = i+dx[data_rank];
					tmp_point4.ty = j+dy[data_rank];
					tmp_point4.td = td_type[direct_rank];
					res.push_back(tmp_point4);
				}
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

vector<neighbor_point4> Voxel_2d::find_layer_connection(Voxel_2d tar)
{
	vector<neighbor_point4> res;
	const int dx[4] = {1,0,-1,0};
	const int dy[4] = {0,1,0,-1};
	const int voxel_type[4] = {4,5,2,3};
	for(int i=0;i<_data.size();i++)
		for(int j=0;j<_data[i].size();j++)
			if((_data[i][j] > VOXEL_3D_EXIST) && (_data[i][j] < VOXEL_3D_UP))
			{
				neighbor_point4 tmp_point4;
				int tmp_rank = _data[i][j]-2;
				if(tar._data[i+dx[tmp_rank]][j+dy[tmp_rank]] == voxel_type[tmp_rank])
				{
					tmp_point4.sx = i;
					tmp_point4.sy = j;
					tmp_point4.sd = VOXEL_3D_DOWN;
					tmp_point4.tx = i+dx[tmp_rank];
					tmp_point4.ty = j+dy[tmp_rank];
					tmp_point4.td = VOXEL_3D_UP;
					res.push_back(tmp_point4);
				}
			}
	return res;
}

void Voxel_2d::add_up_and_down(Voxel_2d& tar, neighbor_point4 nbor)//!!!!!!!!!!!!!!!!!!chang data!!!!!!!!!!
{
	_data[nbor.sx][nbor.sy] = nbor.sd;
	tar._data[nbor.tx][nbor.ty] = nbor.td;
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

bool Voxel_2d::check_2d()
{
	const int dx[6] = {1,0,-1,0,0,0};
	const int dy[6] = {0,1,0,-1,0,0};
	int current_x = 0, current_y = 0;
	int exist_num = 0;
	for(int i=0;i<_data.size();i++)
		for(int j=0;j<_data[i].size();j++)
			if(_data[i][j] > VOXEL_3D_EXIST)
			{
				exist_num++;
				current_x = i;
				current_y = j;
			}
	int st_x = current_x;
	int st_y = current_y;
	while(exist_num)
	{
		exist_num--;
		if(_data[current_x][current_y] <= VOXEL_3D_EXIST)
			return false;
		int next_x = current_x + dx[_data[current_x][current_y]-2];
		int next_y = current_y + dy[_data[current_x][current_y]-2];
		current_x = next_x;
		current_y = next_y;
	}
	return true;
}

Voxel_2d Voxel_2d::circle_reverse()
{
	Voxel_2d res(_data);
	const int dx[4] = {1,0,-1,0};
	const int dy[4] = {0,1,0,-1};
	const int voxel_type[4] = {4,5,2,3};
	int current_x = 0, current_y = 0;
	int exist_num = 0;
	for(int i=0;i<_data.size();i++)
		for(int j=0;j<_data[i].size();j++)
			if(_data[i][j] >= VOXEL_3D_EXIST)
			{
				exist_num++;
				current_x = i;
				current_y = j;
			}
	int st_x = current_x;
	int st_y = current_y;
	while(exist_num)
	{
		exist_num--;
		int next_x = current_x + dx[_data[current_x][current_y]-2];
		int next_y = current_y + dy[_data[current_x][current_y]-2];
		res._data[next_x][next_y] = voxel_type[_data[current_x][current_y]-2];
		current_x = next_x;
		current_y = next_y;
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
	cout<<"x size: "<<size_x<<endl;
	cout<<"y size: "<<size_y<<endl;
	cout<<"z size: "<<size_z<<endl;
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