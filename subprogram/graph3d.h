#pragma once
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
using namespace std;

const int VOXEL_3D_EXIST = 1;
const int VOXEL_3D_VOID = 0;

class Voxel_2d
{
	vector<vector<int> > _data;
public:
	Voxel_2d(){}
	Voxel_2d(vector<vector<int> > x) : _data(x){}
	vector<vector<int> > get_data();
	vector<vector<int> > get_empty_data();
	vector<Voxel_2d> continuous_2d();//to voxel_2d which is continuous
	void print();
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