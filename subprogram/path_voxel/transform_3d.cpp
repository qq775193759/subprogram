#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<iomanip>
using namespace std;

struct Link
{
    int fs;//face source
    int ft;//face target
    int fm;//face middle
    int fe;//face extend
    int fc;//face current
    Link(){fc = 0;}
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
        fin>>tmp_link.fe;
        tmp_link.fc=tmp_link.fs;
        res.push_back(tmp_link);
    }
    return res;
}

int check(vector<Link> link_vec)
{
    return 0;
}


void save_link_vec(vector<Link> link_src, const char* filename)
{
    ofstream fout(filename);
    for(int i=0;i<link_src.size();i++)
    {
        fout<<link_src[i].fc<<" "<<link_src[i].fs<<" "<<link_src[i].fm<<" "<<link_src[i].fe<<endl;
    }
    fout.close();
}


//
//
//
//
//
//
void link_rotate(Link& x, int clock)
{
    const int face_list[6][7]={
        0,3,4,2,1,
        0,4,3,1,2
    };
    x.fs = face_list[clock][x.fs];
    x.ft = face_list[clock][x.ft];
    x.fm = face_list[clock][x.fm];
    x.fe = face_list[clock][x.fe];
    x.fc = face_list[clock][x.fc];
}

struct RotLog
{
    int loop_i;
    int fs;
    int ft;
    RotLog(int x, int y, int z):loop_i(x), fs(y), ft(z){}
};




int main()
{
    const char* filename_src = "table_chair.link";
    const char* filename_tar = "chair_table.link";
    const char* filename_res = "table_chair.res";

    return 0;
}
