#pragma once
#include "graph3d.h"
#include <iostream>
#include <vector>
using namespace std;

class reeb_graph
{
	vector<vector<Voxel_2d> > node_2d;
	vector<vector<int> > node_no;
	int node_num;
	vector<pair<int, int> > edge;
public:
	reeb_graph(vector<Voxel_2d> x);
};