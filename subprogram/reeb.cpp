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
	cout<<"node num: "<<node_num<<endl;
	cout<<"edge size: "<<edge.size()<<endl;
	//make edge mask to build a tree
	vector<int> node_set(node_num, 0);
	edge_mask = vector<int>(edge.size(), 0);
	node_set[0] = 1;
	for(int iter=0;iter<edge.size();iter++)
		for(int i=0;i<edge.size();i++)
		{
			int edge_x = edge[i].first-1;
			int edge_y = edge[i].second-1;
			int set_sum = node_set[edge_x] + node_set[edge_y];
			if(set_sum == 1)
			{
				node_set[edge_x] = 1;
				node_set[edge_y] = 1;
				edge_mask[i] = 1;
			}
		}
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
		cout<<edge[i].first<<" "<<edge[i].second<<" "<<edge_value[i]<<"  \t"<<edge_mask[i]<<endl;
	}
}

vector<Voxel_2d> reeb_graph::find_circle()
{
	//find circle for node
	int cw = 1;
	for(int i=0;i<node_2d.size();i++)
	{
		for(int j=0;j<node_2d[i].size();j++)
		{
			Voxel_2d tmp_circle = node_2d[i][j].find_circle(cw);
			if(tmp_circle.check_2d() == false)
			{
				cout<<i<<" "<<j<<" "<<node_no[i][j]<<endl;
				node_2d[i][j].find_circle(cw, 1);
			}
			circle_vec.push_back(tmp_circle);
		}
		cw = 4 - cw;
	}
	//debug print
	//for(int i=0;i<circle_vec.size();i++)
	//	circle_vec[i].print();
	//connect layer
	for(int i=0;i<edge.size();i++)
		if(edge_mask[i] == 1)
		{
			vector<neighbor_point4> tmp_neighbor_vec = 
				circle_vec[edge[i].first-1].find_layer_connection(circle_vec[edge[i].second-1]);
			//cout<<tmp_neighbor_vec.size()<<endl;
			if(tmp_neighbor_vec.size() == 0)
			{
				circle_vec[edge[i].second-1] = circle_vec[edge[i].second-1].circle_reverse();
				vector<neighbor_point4> tmp_neighbor_vec = 
				circle_vec[edge[i].first-1].find_layer_connection(circle_vec[edge[i].second-1]);
				if(tmp_neighbor_vec.size() == 0)
				{
					cout<<edge[i].first<<" "<<edge[i].second<<endl;
					circle_vec[edge[i].first-1].print();
					circle_vec[edge[i].second-1].print();
				}
				//cout<<"reverse : "<<tmp_neighbor_vec.size()<<endl;
				circle_vec[edge[i].first-1].add_up_and_down(circle_vec[edge[i].second-1], tmp_neighbor_vec[0]);
			}
			else
				circle_vec[edge[i].first-1].add_up_and_down(circle_vec[edge[i].second-1], tmp_neighbor_vec[0]);
		}
	//merge node in the same layer
	for(int i=0;i<node_no.size();i++)
		if(node_no[i].size()>0)
		{
			Voxel_2d tmp_plane = circle_vec[node_no[i][0]-1];
			for(int j=0;j<node_no[i].size();j++)
				tmp_plane.add_circle(circle_vec[node_no[i][j]-1]);
			//tmp_plane.print();
			plane_circle_vec.push_back(tmp_plane);
		}
		check_whole_circle();
	return plane_vec;
}

void reeb_graph::check_whole_circle()
{
	const int dx[6] = {1,0,-1,0,0,0};
	const int dy[6] = {0,1,0,-1,0,0};
	const int dz[6] = {0,0,0,0,-1,1};
	int current_x = 0, current_y = 0, current_z = 0;
	int exist_num = 0;
	for(int k=0;k<plane_circle_vec.size();k++)
		for(int i=0;i<plane_circle_vec[k]._data.size();i++)
			for(int j=0;j<plane_circle_vec[k]._data[i].size();j++)
				if(plane_circle_vec[k]._data[i][j] > VOXEL_3D_EXIST)
				{
					exist_num++;
					current_x = i;
					current_y = j;
					current_z = k;
				}
	cout<<"total voxel:  "<<exist_num<<endl;
	int st_x = current_x;
	int st_y = current_y;
	int st_z = current_z;
	while(exist_num)
	{
		exist_num--;
		if(plane_circle_vec[current_z]._data[current_x][current_y] <= VOXEL_3D_EXIST)
		{
			cout<<"check break error"<<endl;
			return;
		}
		int next_x = current_x + dx[plane_circle_vec[current_z]._data[current_x][current_y]-2];
		int next_y = current_y + dy[plane_circle_vec[current_z]._data[current_x][current_y]-2];
		int next_z = current_z + dz[plane_circle_vec[current_z]._data[current_x][current_y]-2];
		current_x = next_x;
		current_y = next_y;
		current_z = next_z;
		//cout<<current_x<<" "<<current_y<<" "<<current_z<<endl;
	}
	if((current_x == st_x) &&(current_y == st_y)&&(current_z == st_z))
		cout<<"check OK"<<endl;
	else
	{
		cout<<"check error"<<endl;
		cout<<current_x<<"  "<<st_x<<endl;
		int x;cin>>x;
	}
}

const int VOXEL_2D_PRINT_OFFSET = 1;
const char VOXEL_2D_PRINT_CHAR[10] = {'X', '0', '*','V','>','A','<','U','D'};

void reeb_graph::save(const char* filename)
{
	ofstream fout(filename);
	for(int k=0;k<plane_circle_vec.size();k++)
	{
		fout<<"level: "<<k<<endl;
		for(int i=0;i<plane_circle_vec[k]._data.size();i++)
		{
			for(int j=0;j<plane_circle_vec[k]._data[i].size();j++)
				fout<<VOXEL_2D_PRINT_CHAR[plane_circle_vec[k]._data[i][j] + VOXEL_2D_PRINT_OFFSET]<<" ";
			fout<<endl;
		}
	}
	fout.close();
}

void reeb_graph::save_path(const char* filename, const char* filename_edge)
{
	ofstream fout(filename);
	ofstream fout_edge(filename_edge);
	const int dx[6] = {1,0,-1,0,0,0};
	const int dy[6] = {0,1,0,-1,0,0};
	const int dz[6] = {0,0,0,0,-1,1};
	const int direction[6] = {0,2,1,3,4,5};
	int current_x = 0, current_y = 0, current_z = 0;
	int exist_num = 0;
	for(int k=0;k<plane_circle_vec.size();k++)
		for(int i=0;i<plane_circle_vec[k]._data.size();i++)
			for(int j=0;j<plane_circle_vec[k]._data[i].size();j++)
				if(plane_circle_vec[k]._data[i][j] > VOXEL_3D_EXIST)
				{
					exist_num++;
					current_x = i;
					current_y = j;
					current_z = k;
				}
	int st_x = current_x;
	int st_y = current_y;
	int st_z = current_z;
	while(exist_num)
	{
		exist_num--;
		fout<<direction[plane_circle_vec[current_z]._data[current_x][current_y]-2]<<endl;
		fout_edge<<current_x<<" "<<current_y<<" "<<current_z<<endl;
		int next_x = current_x + dx[plane_circle_vec[current_z]._data[current_x][current_y]-2];
		int next_y = current_y + dy[plane_circle_vec[current_z]._data[current_x][current_y]-2];
		int next_z = current_z + dz[plane_circle_vec[current_z]._data[current_x][current_y]-2];
		current_x = next_x;
		current_y = next_y;
		current_z = next_z;
	}
	fout.close();
	fout_edge.close();
}