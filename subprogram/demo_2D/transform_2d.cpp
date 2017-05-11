#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<iomanip>
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

void show(vector<Link> link_vec)
{
    //for(int i=0;i<link_vec.size();i++)
    //    cout<<link_vec[i].fs<<" ";
    cout<<endl;
    //show
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
        //tmp_vec_2d[y_vec[i]-y_min][x_vec[i]-x_min]+=i+1;
    }
    for(int i=0;i<tmp_vec_2d.size();i++)
    {
        for(int j=0;j<tmp_vec_2d[i].size();j++)
        {
            cout<<tmp_vec_2d[i][j]<<" ";
            //cout<<setiosflags(ios::left)<<setw(3)<<tmp_vec_2d[i][j];
        }
        cout<<endl;
    }
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
    show(link_src);
}

void link_vec_rotate_entry(vector<Link>& link_src, int loop_i, int fs, int ft, int fm)
{
    if(fs == ft) return;
    const int ft_list[7] = {0,2,1,4,3,6,5};
    if(ft_list[fs] != ft)
    {
        link_vec_rotate(link_src, loop_i, fs, ft);
    }
    else
    {
        link_vec_rotate(link_src, loop_i, fs, fm);
        link_vec_rotate(link_src, loop_i, fm, ft);
    }
}

void check_link_2d(vector<Link>& link_src)
{
    show(link_src);
    for(int i=0;i<link_src.size();i++)
    {
        if(link_src[i].fs == link_src[i].ft);
        else
        {
            link_vec_rotate_entry(link_src, i, link_src[i].fs, link_src[i].ft, link_src[i].fm);
        }
    }
}

int check_positive(int x)
{
    if(x == 1) return 1;
    if(x == 3) return 1;
    return 0;
}

void straighten_2d(vector<Link>& link_src, int constraint)
{
    show(link_src);
    for(int i=0;i<link_src.size();i++)
    {
        int move_flag = 0;
        int tmp_ft;
        int tmp_fm;
        //strategy
        if(link_src[i].fs == link_src[i].ft)
        {
            if(check_positive(link_src[i].fs) == 0)
            {
                move_flag = 1;
                tmp_ft = link_src[i].fs %4 + 1;
                tmp_fm = 0;
            }
        }
        else if(link_src[i].fm == 0)
        {
            if(check_positive(link_src[i].fs) == 0)
            {
                if(check_positive(link_src[i].ft) == 1)
                {
                    move_flag = 1;
                    tmp_ft = link_src[i].ft;
                    tmp_fm = 0;
                }
                else
                {
                    move_flag = 1;
                    if(constraint==0)
                    {
                        tmp_ft = link_src[i].fs %4 + 1;
                        tmp_fm = 0;
                    }
                    else
                    {
                        tmp_ft = (link_src[i].fs+2)%4 + 1;
                        tmp_fm = link_src[i].ft;
                    }

                }
            }
        }
        else
        {
            if(check_positive(link_src[i].fs) == 0)
            {
                move_flag = 1;
                tmp_ft = link_src[i].ft;
                tmp_fm = link_src[i].fm;
            }
        }
        if(move_flag)
        {
            link_vec_rotate_entry(link_src, i, link_src[i].fs, tmp_ft, tmp_fm);
            show(link_src);
        }
    }
}

int main()
{
    const char* filename_src = "demo4/cross_eight_2d.link";
    const char* filename_tar = "demo4/eight_cross_2d.link";
    vector<Link> link_src = read_link(filename_src);
    vector<Link> link_tar = read_link(filename_tar);
    //check_link_2d(link_src);
    //straighten_2d(link_src, 0);
    straighten_2d(link_tar, 1);
    return 0;
}
