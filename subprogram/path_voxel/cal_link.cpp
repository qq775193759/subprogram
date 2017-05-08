#include<iostream>
#include<fstream>
#include<vector>
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

void cal_link(const char* src, const char* tar, const char* output)
{
    ofstream fout(output);
    vector<int> path_src = read_path(src);
    vector<int> path_tar = read_path(tar);
    cout<<"src size: "<<path_src.size()<<endl;
    cout<<"tar size: "<<path_tar.size()<<endl;

    fout.close();
}

int main()
{
    cal_link("bunny_y.txt","eight_x.txt","bunny_eight.link");
    cal_link("table_z.txt","chair_y.txt","table_chair.link");
    return 0;
}
