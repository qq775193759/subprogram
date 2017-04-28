#pragma once
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
using namespace std;

//VOID
const int VOXEL_3D_EDGE = -1;
const int VOXEL_3D_VOID = 0;
//EXIST and more
const int VOXEL_3D_EXIST = 1;
const int VOXEL_3D_OUT = 2;//x++
const int VOXEL_3D_RIGHT = 3;//y++
const int VOXEL_3D_IN = 4;//x--
const int VOXEL_3D_LEFT = 5;//y--
const int VOXEL_3D_UP = 6;//up in the top layer means ending
const int VOXEL_3D_DOWN = 7;//down in the bottom layer means beginning

int union_label_2d(vector<vector<int> > &x);

struct neighbor_point4
{
	int sx;
	int sy;
	int sd;//direction
	int tx;
	int ty;
	int td;//direction
};

class Voxel_2d
{
	vector<vector<int> > _data;
	friend class reeb_graph;
public:
	Voxel_2d(){}
	Voxel_2d(vector<vector<int> > x) : _data(x){}
	void print();
	void check_circle();
//the same layer
	void fix_single();//wait for finishing
	vector<Voxel_2d> continuous_2d();//to voxel_2d which is continuous
//find circle
	Voxel_2d make_edge();//
	Voxel_2d find_edge();//find voxel in edge
	vector<Voxel_2d> fix_odd_from_edge();//delete odd point from edge
	Voxel_2d find_circle_from_edge(int cw);
	void add_circle(Voxel_2d tar);//!!!!!!!!!!!!!!!!!!chang data!!!!!!!!!!
	Voxel_2d find_circle(int init_cw);//2345 wsad
//between 2 circle in the same layer
	vector<neighbor_point4> find_neighbor(Voxel_2d tar, int cw);
//between 2 layers
	int count_overlap(Voxel_2d tar);
	Voxel_2d build_overlap(Voxel_2d tar);
	vector<neighbor_point4> find_layer_connection(Voxel_2d tar);
	void add_up_and_down(Voxel_2d& tar, neighbor_point4 nbor);//!!!!!!!!!!!!!!!!!!chang data!!!!!!!!!!
	Voxel_2d substract(Voxel_2d x);//return this - x
//other function
	Voxel_2d circle_reverse();
	int count_type(int type);
};

class Voxel_3d
{
	vector<vector<vector<int> > > _data;
	int size_x;
	int size_y;
	int size_z;
public:
	Voxel_3d(const char* filename);
	vector<Voxel_2d> trans_2d();
};