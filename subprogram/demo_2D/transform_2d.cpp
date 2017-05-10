#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
using namespace std;

struct Link
{
    int fs;
    int ft;
    int fm;
};

vector<Link> read_link(const char* filename)
{
    vector<Link> res;
    ifstream fin(filename);
    Link tmp_link;
    while(fin>>tmp_link.fs)
    {
        fin>>tmp_link.ft;
        fin>>tmp_link.fm;
        res.push_back(tmp_link);
    }
    return res;
}

void link_rotate(Link& x, int clock)
{
    const int face_list[2][5]={
        0,3,4,2,1,
        0,4,3,1,2
    };
    x.fs = face_list[clock][x.fs];
    x.ft = face_list[clock][x.ft];
    x.fm = face_list[clock][x.fm];
}

void link_vec_rotate(vector<Link>& link_src, int loop_i, int fs, int ft)
{
    const int clock_list[4][4]={
        -1,-1,0,1,
        -1,-1,1,0,
        1,0,-1,-1,
        0,1,-1,-1
    };
    int clock = clock_list[fs-1][ft-1];
    Link tmp_link_i = link_src[loop_i];
    link_rotate(tmp_link_i, clock);
    link_src[loop_i].fs = tmp_link_i.fs;
    for(int j=(loop_i+1);j<link_src.size();j++)
    {
        link_rotate(link_src[j], clock);
    }
}

void show(vector<Link> link_vec)
{
    for(int i=0;i<link_vec.size();i++)
        cout<<link_vec[i].fs<<" ";
    cout<<endl;
    //
    const int dx[4] = {1,-1,0,0};
    const int dy[4] = {0,0,1,-1};
    vector<int> x_vec;
    vector<int> y_vec;
    int x_min = 0, x_max = 0, y_min = 0, y_max = 0;
    int x_current = 0, y_current = 0;
    x_vec.push_back(x_current);
    y_vec.push_back(y_current);
    for(int i=0;i<link_vec.size();i++)
    {
        x_current += dx[link_vec[i].fs-1];
        y_current += dy[link_vec[i].fs-1];
        x_vec.push_back(x_current);
        y_vec.push_back(y_current);
        x_min = min(x_min, x_current);
        x_max = max(x_max, x_current);
        y_min = min(y_min, y_current);
        y_max = max(y_max, y_current);
    }
    int x_len = x_max-x_min+1;
    int y_len = y_max-y_min+1;
    vector< vector<int> > tmp_vec_2d;
    for(int i=0;i<y_len;i++)
    {
        vector<int> tmp_vec(x_len, 0);
        tmp_vec_2d.push_back(tmp_vec);
    }
    for(int i=0;i<x_vec.size();i++)
    {
        tmp_vec_2d[y_vec[i]-y_min][x_vec[i]-x_min]++;
    }
    for(int i=0;i<tmp_vec_2d.size();i++)
    {
        for(int j=0;j<tmp_vec_2d[i].size();j++)
        {
            cout<<tmp_vec_2d[i][j]<<" ";
        }
        cout<<endl;
    }
}

void straighten_2d(vector<Link>& link_src)
{
    show(link_src);
    for(int i=0;i<link_src.size();i++)
    {
        if(link_src[i].fs == link_src[i].ft);
        else
        {
            if(link_src[i].fm == 0)
            {
                link_vec_rotate(link_src, i, link_src[i].fs, link_src[i].ft);
            }
            else
            {
                link_vec_rotate(link_src, i, link_src[i].fs, link_src[i].fm);
                link_vec_rotate(link_src, i, link_src[i].fm, link_src[i].ft);
            }
            show(link_src);
        }
    }
}


int main()
{
    vector<Link> link_src = read_link("cross_eight_2d.link");
    straighten_2d(link_src);
    return 0;
}
