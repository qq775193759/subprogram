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

int SHOW_SWITCH = 0;
int SHOW_WAY = 0;

void show(vector<Link> link_vec)
{
    if(SHOW_SWITCH == 0) return;
    //for(int i=0;i<link_vec.size();i++)
    //    cout<<link_vec[i].fs<<" ";
    //cout<<endl;
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
        x_current += dx[link_vec[i].fc-1];
        y_current += dy[link_vec[i].fc-1];
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
        if(SHOW_WAY)
            tmp_vec_2d[y_vec[i]-y_min][x_vec[i]-x_min]++;
        else
            tmp_vec_2d[y_vec[i]-y_min][x_vec[i]-x_min]+=i+1;
    }
    for(int i=0;i<tmp_vec_2d.size();i++)
    {
        for(int j=0;j<tmp_vec_2d[i].size();j++)
        {
            if(SHOW_WAY)
                if(tmp_vec_2d[i][j])
                    cout<<tmp_vec_2d[i][j]<<" ";
                else
                    cout<<"  ";
            else
                cout<<setiosflags(ios::left)<<setw(3)<<tmp_vec_2d[i][j];
        }
        cout<<endl;
    }
    cout<<endl;
}

int check(vector<Link> link_vec)
{
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
        x_current += dx[link_vec[i].fc-1];
        y_current += dy[link_vec[i].fc-1];
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
            if(tmp_vec_2d[y_vec[i]-y_min][x_vec[i]-x_min]>=2)
                return 0;
    }
    return 1;
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

void link_rotate(Link& x, int clock)
{
    const int face_list[2][5]={
        0,3,4,2,1,
        0,4,3,1,2
    };
    x.fs = face_list[clock][x.fs];
    x.ft = face_list[clock][x.ft];
    x.fm = face_list[clock][x.fm];
    x.fe = face_list[clock][x.fe];
    x.fc = face_list[clock][x.fc];
}

void link_vec_rotate_without_show(vector<Link>& link_src, int loop_i, int fs, int ft)
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
    link_src[loop_i].fc = tmp_link_i.fc;
    for(int j=(loop_i+1);j<link_src.size();j++)
    {
        link_rotate(link_src[j], clock);
    }
}

void link_vec_rotate(vector<Link>& link_src, int loop_i, int fs, int ft)
{
    link_vec_rotate_without_show(link_src, loop_i, fs, ft);
    show(link_src);
}

int check_and_link_vec_rotate(vector<Link>& link_src, int loop_i, int fs, int ft)
{
    link_vec_rotate_without_show(link_src, loop_i, fs, ft);
    int res = check(link_src);
    if(res == 0)
        link_vec_rotate_without_show(link_src, loop_i, ft, fs);
    else
        show(link_src);
    return res;
}

struct RotLog
{
    int loop_i;
    int fs;
    int ft;
    RotLog(int x, int y, int z):loop_i(x), fs(y), ft(z){}
};

//rot entries
vector<RotLog> rotlog;

//rotate entry
void link_vec_rotate_entry(vector<Link>& link_src, int loop_i, int fs, int ft, int fm)
{
    if(fs == ft) return;
    const int ft_list[7] = {0,2,1,4,3,6,5};
    if(ft_list[fs] != ft)
    {
        rotlog.push_back(RotLog(loop_i, fs, ft));
        link_vec_rotate(link_src, loop_i, fs, ft);
    }
    else
    {
        rotlog.push_back(RotLog(loop_i, fs, fm));
        link_vec_rotate(link_src, loop_i, fs, fm);
        rotlog.push_back(RotLog(loop_i, fm, ft));
        link_vec_rotate(link_src, loop_i, fm, ft);
    }
}

//recover entry
void link_vec_recover_entry(vector<Link>& link_src, vector<RotLog> loclog)
{
    for(int i=1;i<=loclog.size();i++)
    {
        RotLog tmp_log = rotlog[loclog.size()-i];
        link_vec_rotate(link_src, tmp_log.loop_i, tmp_log.ft, tmp_log.fs);
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

void straighten_2d(vector<Link>& link_src, int constraint)
{
    show(link_src);
    for(int i=0;i<link_src.size();i++)
    {
        int move_flag = 0;
        int tmp_ft;
        int tmp_fm=0;
        //strategy
        if(link_src[i].fs == 1);
        else if(link_src[i].fs == 3)
        {
            //change middle
            if((link_src[i].fm == 2) && (constraint == 0))
                link_src[i].fm = 1;
            //extend 180
            if((link_src[i].fm == 0) && (link_src[i].fe == 0))
            {
                link_src[i].fe = 1;
            }
            //try to face 1
            if((link_src[i].ft == 1)||(link_src[i].fm == 1)||(link_src[i].fe == 1))
            {
                if(SHOW_SWITCH)
                    cout<<"case 1:"<<endl;
                move_flag = 1;
                tmp_ft = 1;
            }
        }
        else
        {
            //extend 180
            if((link_src[i].fm == 0) && (link_src[i].fe == 0))
            {
                link_src[i].fe = 1;
            }
            //try to face 1
            if((link_src[i].ft == 1)||(link_src[i].fm == 1))
            {
                if(SHOW_SWITCH)
                    cout<<"case 2:"<<endl;
                move_flag = 1;
                tmp_ft = 1;
                tmp_fm = link_src[i].fm;
            }
            else if(link_src[i].fe == 1)
            {
                if(SHOW_SWITCH)
                    cout<<"case 3:"<<endl;
                move_flag = 1;
                tmp_ft = 1;
                tmp_fm = link_src[i].ft;
            }
            else if((link_src[i].ft == 3)||(link_src[i].fm == 3))
            {
                if(SHOW_SWITCH)
                    cout<<"case 4:"<<endl;
                move_flag = 1;
                tmp_ft = 1;
                tmp_fm = link_src[i].fm;
            }
        }
        //do rotate
        if(move_flag)
        {
            link_vec_rotate_entry(link_src, i, link_src[i].fs, tmp_ft, tmp_fm);
        }
    }
}

void straighten_2d_entry(vector<Link>& link_src, int constraint)
{
    rotlog.clear();
    straighten_2d(link_src, 0);
    vector<RotLog> tmplog = rotlog;
    link_vec_recover_entry(link_src, tmplog);

    SHOW_SWITCH = 1;
    vector<Link>link_src_cpy = link_src;
    show(link_src_cpy);
    for(int i=0;i<tmplog.size();i++)
    {
        RotLog tmp_log = tmplog[i];
        int res = check_and_link_vec_rotate(link_src_cpy, tmp_log.loop_i, tmp_log.fs, tmp_log.ft);
        if(res == 0)
            tmplog.push_back(RotLog(tmp_log.loop_i, tmp_log.fs, tmp_log.ft));
    }
    SHOW_SWITCH = 0;
}


int main()
{
    const char* filename_src = "demo8/cross_eight_2d.link";
    const char* filename_tar = "demo8/eight_cross_2d.link";
    const char* filename_res = "demo8/res_2d.link";
    vector<Link> link_src = read_link(filename_src);
    straighten_2d_entry(link_src, 0);
    check_link_2d(link_src);
    save_link_vec(link_src, filename_tar);
    //link
    vector<Link> link_tar = read_link(filename_tar);
    straighten_2d_entry(link_tar, 1);
    check_link_2d(link_tar);
    save_link_vec(link_tar, filename_res);
    return 0;
}
