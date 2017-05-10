#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
using namespace std;

vector<int> read_path(const char* filename)
{
    vector<int> res;
    ifstream fin(filename);
    int x;
    while(fin>>x)
    {
        res.push_back(x);
    }
    return res;
}

void face_state_change(int face[6], int fs, int ft)
{
    //13241324  12
    //15261526  34
    //35463546  56
    const int rank_list[6][6] = {
        0,0,1,2,3,4,
        0,0,2,1,4,3,
        2,1,0,0,5,6,
        1,2,0,0,6,5,
        4,3,6,5,0,0,
        3,4,5,6,0,0
    };
    const int substitution[6][6] = {
        4,3,1,2,5,6,
        3,4,2,1,5,6,
        6,5,3,4,1,2,
        5,6,3,4,2,1,
        1,2,6,5,3,4,
        1,2,5,6,4,3
    };
    int tmp_rank = rank_list[fs-1][ft-1];
    for(int j=0;j<6;j++)
    {
        face[j] = substitution[tmp_rank-1][face[j]-1];
    }
}

void cal_link(const char* src, const char* tar, const char* output)
{
    const int fm_list[4][4] = {
        0,3,0,0,
        4,0,0,0,
        0,0,0,2,
        0,0,1,0
    };
    ofstream fout(output);
    vector<int> path_src = read_path(src);
    vector<int> path_tar = read_path(tar);
    cout<<"src size: "<<path_src.size()<<endl;
    cout<<"tar size: "<<path_tar.size()<<endl;
    if(path_src.size() != path_tar.size())
        cout<<"different length!"<<endl;
    int path_len = path_src.size()-1;
    int face[6] = {1,2,3,4,5,6};
    for(int i=0;i<path_len;i++)
    {
        int fs = path_src[i] + 1;
        int ft = face[path_tar[i]];
        int fm = fm_list[fs-1][ft-1];
        if(fm>0)
        {
            face_state_change(face, fs, fm);
            face_state_change(face, fm, ft);
        }
        else if(fs != ft)
            face_state_change(face, fs, ft);
        fout<<fs<<' ';
        fout<<ft<<' ';
        fout<<fm<<' ';
        fout<<endl;
    }
    fout.close();
}

int main()
{
    cal_link("cross_2d.txt","eight_2d.txt","cross_eight_2d.link");
    cal_link("eight_2d.txt","cross_2d.txt","eight_cross_2d.link");
    return 0;
}
