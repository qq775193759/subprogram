#pragma once
#include "graph3d.h"
#include <iostream>
#include <vector>
using namespace std;

class reeb_graph
{
	vector<Voxel_2d> plane_vec;
	vector<vector<Voxel_2d> > node_2d;
	vector<vector<int> > node_no;
	int node_num;
	vector<Voxel_2d> circle_vec;//rank is node
	vector<Voxel_2d> plane_circle_vec;//rank is layer
	vector<pair<int, int> > edge;
	vector<int> edge_mask;
	vector<int> edge_value;
public:
	reeb_graph(vector<Voxel_2d> x);
	void print();
	vector<Voxel_2d> find_circle();
	void check_whole_circle();
};