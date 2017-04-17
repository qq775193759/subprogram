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
const int VOXEL_3D_IN = 2;
const int VOXEL_3D_OUT = 3;
const int VOXEL_3D_LEFT = 4;
const int VOXEL_3D_RIGHT = 5;
const int VOXEL_3D_UP = 6;//up in the top layer means ending
const int VOXEL_3D_DOWN = 7;//down in the bottom layer means beginning

int union_label_2d(vector<vector<int> > &x);

struct neighbor_int4
{
	int sx;
	int sy;
	int tx;
	int ty;
	neighbor_int4(int sx, int sy, int tx, int ty):sx(sx), sy(sy), tx(tx), ty(ty) {}
};

class Voxel_2d
{
	vector<vector<int> > _data;
public:
	Voxel_2d(){}
	Voxel_2d(vector<vector<int> > x) : _data(x){}
	void print();
//the same layer
	void fix_single();//wait for finishing
	vector<Voxel_2d> continuous_2d();//to voxel_2d which is continuous
//find circle
	Voxel_2d make_edge();//
	Voxel_2d find_edge();//find voxel in edge
	Voxel_2d find_circle_from_edge();
	Voxel_2d find_circle();//2345 wsad
//between 2 circle in the same layer
	vector<neighbor_int4> find_neighbor(Voxel_2d tar);
//between 2 layers
	int count_overlap(Voxel_2d tar);
	Voxel_2d build_overlap(Voxel_2d tar);
	Voxel_2d substract(Voxel_2d x);//return this - x
//other function
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