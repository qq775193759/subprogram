#include "reeb.h"
using namespace std;

reeb_graph::reeb_graph(vector<Voxel_2d> x)
{
	plane_vec = x;
	node_num = 0;
	//node_2d & node_no
	for(int i=0;i<x.size();i++)
	{
		vector<Voxel_2d> plane = x[i].continuous_2d();
		node_2d.push_back(plane);
		vector<int> tmp_no;
		for(int j=0;j<plane.size();j++)
		{
			node_num++;
			tmp_no.push_back(node_num);
		}
		node_no.push_back(tmp_no);
	}
	//edge & edge_value
	for(int i=1;i<x.size();i++)
	{
		for(int j=0;j<node_2d[i].size();j++)
		{
			for(int k=0;k<node_2d[i-1].size();k++)
			{
				int overlap = node_2d[i][j].count_overlap(node_2d[i-1][k]);
				if(overlap > 0)
				{
					edge.push_back(pair<int, int>(node_no[i-1][k], node_no[i][j]));
					edge_value.push_back(overlap);
				}
			}
		}
	}
	cout<<edge.size()<<endl;
}

void reeb_graph::print()
{
	for(int i=0;i<node_no.size();i++)
	{
		for(int j=0;j<node_no[i].size();j++)
		{
			cout<<node_no[i][j]<<" ";
		}
		cout<<endl;
	}
	for(int i=0;i<edge.size();i++)
	{
		cout<<edge[i].first<<" "<<edge[i].second<<" "<<edge_value[i]<<endl;
	}
}

vector<Voxel_2d> reeb_graph::find_circle()
{
	int cw = 1;
	for(int i=0;i<node_2d.size();i++)
	{
		for(int j=0;j<node_2d[i].size();j++)
		{
			node_2d[i][j].find_circle(cw);
		}
		cw = 4 - cw;
	}
	return plane_vec;
}