#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include <fstream>
#include <string>
#include <queue>

using namespace std;

typedef pair<pair<int, int>, pair<int, int>> constrain;



class FutoshikiPuzzle {
public:
    //记录当前的
    class node {
    public:
        //当前位置的值
        int val = 0;
        //候选队列，false代表可以选择
        //开了十个空间但是可能只用前几个
        bool digits[10] = { false, false, false, false, false, false, false, false, false, false };
    };

    struct file {
        int numConstrains;
        int numRow;
        string filename;
    };

    //地图
    vector<vector<node>> maps;
    //限制
    vector<pair<pair<int, int>, pair<int, int>>> less_constraints;
    vector<file> allfile;
    queue<constrain> GAC_constrains;
    int nRow, nColumn;

    void initial() {
        vector<vector<int>> copy_map = { {0, 0, 0, 7, 3, 8, 0, 5, 0},
                                        {0, 0, 7, 0, 0, 2, 0, 0, 0},
                                        {0, 0, 0, 0, 0, 9, 0, 0, 0},
                                        {0, 0, 0, 4, 0, 0, 0, 0, 0},
                                        {0, 0, 1, 0, 0, 0, 6, 4, 0},
                                        {0, 0, 0, 0, 0, 0, 2, 0, 0},
                                        {0, 0, 0, 0, 0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0, 0, 0, 0, 6} };

        nRow = copy_map.size();
        nColumn = copy_map[0].size();

        maps.resize(nRow);

        //复制到真实地图上
        for (int i = 0; i < nRow; i++) {
            maps[i].resize(nColumn);
            for (int j = 0; j < nColumn; j++) {
                maps[i][j].val = copy_map[i][j];
            }
        }

        //添加限制
        addConstraints(0, 0, 0, 1);
        addConstraints(0, 3, 0, 2);
        addConstraints(1, 3, 1, 4);
        addConstraints(1, 6, 1, 7);
        addConstraints(2, 6, 1, 6);
        addConstraints(2, 1, 2, 0);
        addConstraints(2, 2, 2, 3);
        addConstraints(2, 3, 3, 3);
        addConstraints(3, 3, 3, 2);
        addConstraints(3, 5, 3, 4);
        addConstraints(3, 5, 3, 6);
        addConstraints(3, 8, 3, 7);
        addConstraints(4, 1, 3, 1);
        addConstraints(4, 5, 3, 5);
        addConstraints(4, 0, 4, 1);
        addConstraints(5, 4, 4, 4);
        addConstraints(5, 8, 4, 8);
        addConstraints(5, 1, 5, 2);
        addConstraints(5, 4, 5, 5);
        addConstraints(5, 7, 5, 6);
        addConstraints(5, 1, 6, 1);
        addConstraints(6, 6, 5, 6);
        addConstraints(6, 8, 5, 8);
        addConstraints(6, 3, 6, 4);
        addConstraints(7, 7, 6, 7);
        addConstraints(7, 1, 8, 1);
        addConstraints(8, 2, 7, 2);
        addConstraints(7, 5, 8, 5);
        addConstraints(8, 8, 7, 8);
        addConstraints(8, 5, 8, 6);

        //根据已经填写的数字开始删除各个位置的候选值
        for (int i = 0; i < nRow; i++) {
            for (int j = 0; j < nColumn; j++) {
                //vector<pair<pair<int, int>, int>> catches = check(i, j);
                vector<pair<pair<int, int>, int>> catches;
                check(i, j, catches);
            }
        }
    }

    //filename带路径，否则就要在同一文件夹下
    void initial(int filenum) {
        allfile.resize(6);
        allfile[0].filename = "data1.txt";
        allfile[0].numRow = 5;
        allfile[0].numConstrains = 7;
        allfile[1].filename = "data2.txt";
        allfile[1].numRow = 6;
        allfile[1].numConstrains = 9;
        allfile[2].filename = "data3.txt";
        allfile[2].numRow = 7;
        allfile[2].numConstrains = 19;
        allfile[3].filename = "data4.txt";
        allfile[3].numRow = 8;
        allfile[3].numConstrains = 25;
        allfile[4].filename = "data5.txt";
        allfile[4].numRow = 9;
        allfile[4].numConstrains = 43;
        allfile[5].filename = "data6.txt";
        allfile[5].numRow = 9;
        allfile[5].numConstrains = 32;

        //这一部分是文件读取初始化
        fstream f;
        f.open(allfile[filenum].filename);
        vector<vector<int>> copy_map;
        copy_map.resize(allfile[filenum].numRow);
        string str;
        if (f.is_open()) {
            //获取地图
            for (int i = 0; i < allfile[filenum].numRow; i++) {
                copy_map[i].resize(allfile[filenum].numRow);
                getline(f, str);
                int k = 0;
                for (int j = 0; j < str.size(); j++) {
                    if (str.at(j) != ' ') {
                        copy_map[i][k] = str.at(j) - '0';
                        k++;
                        if (k == allfile[filenum].numRow) {
                            break;
                        }
                    }
                }
            }
            //获取约束
            for (int i = 0; i < allfile[filenum].numConstrains; i++) {
                getline(f, str);
                vector<int> temp;
                for (int i = 0; i < str.size(); i++) {
                    if (str.at(i) != ' ') {
                        temp.push_back(str.at(i) - '0');
                    }
                }
                addConstraints(temp[0], temp[1], temp[2], temp[3]);
            }
        }

        //到这里所有的初始化就结束了
        

        //复制到真实地图上
        nRow = copy_map.size();
        nColumn = copy_map[0].size();
        maps.resize(nRow);
        for (int i = 0; i < nRow; i++) {
            maps[i].resize(nColumn);
            for (int j = 0; j < nColumn; j++) {
                maps[i][j].val = copy_map[i][j];
            }
        }
        


        //进行初始的GAC检测
        //最外面两重循环是对于每一行每一列，也就是每一个元素
        for (int x = 0; x < nRow; x++) {
            for (int y = 0; y < nColumn; y++) {      
                //这里是删除行列的候选值，没有使用GAC
                for (int i = 0; i < nRow; i++) {
                    //如果本来就是false，那就没必要加入缓存了
                    if (!maps[i][y].digits[maps[x][y].val]) {
                        maps[i][y].digits[maps[x][y].val] = true;
                        //意思是在[i,y]格子的候选者中删除了maps[x][y].val候选者
                        //catches.push_back({ {i, y}, maps[x][y].val });
                    }

                    if (!maps[x][i].digits[maps[x][y].val]) {
                        maps[x][i].digits[maps[x][y].val] = true;
                        //catches.push_back({ {x, i}, maps[x][y].val });
                    }
                }

                //删除关于限制的候选值
                //这里用的是GAC检测，就是对这个位置的论域中的每一个值都判断是不是有一个有效支撑，因为约束只是简单的比较大小，所以其实另一方的所有不满足不等式的都不是有效支撑
                //将变量置为true的过程起始就是GAC检测删掉无效支撑的过程
        
                for (int i = 0; i < less_constraints.size(); i++) {
                    if (x == less_constraints[i].first.first && y == less_constraints[i].first.second) {
                        maps[x][y].digits[nRow] = true;
                        for (int j = maps[x][y].val; j >= 1; j--) {
                            if (!maps[less_constraints[i].second.first][less_constraints[i].second.second].digits[j]) {
                                maps[less_constraints[i].second.first][less_constraints[i].second.second].digits[j] = true;
                                //catches.push_back({ {less_constraints[i].second.first, less_constraints[i].second.second}, j });
                            }
                        }
                    }

                    if (x == less_constraints[i].second.first && y == less_constraints[i].second.second) {
                        maps[x][y].digits[1] = true;
                        if (maps[x][y].val != 0) {
                            for (int j = maps[x][y].val; j <= nRow; j++) {
                                if (!maps[less_constraints[i].first.first][less_constraints[i].first.second].digits[j]) {
                                    maps[less_constraints[i].first.first][less_constraints[i].first.second].digits[j] = true;
                                    //catches.push_back({ {less_constraints[i].first.first, less_constraints[i].first.second}, j });
                                }
                            }
                        }
                        

                    }
                }
            }
        }
    }

    void addConstraints(int x, int y, int x1, int y1) {
        less_constraints.push_back({ {x,  y},
                                    {x1, y1} });
    }



    void enqueueConstrains(int x, int y, queue<constrain>& relatedconstrains) {
        pair<int, int> temp = make_pair(x, y);
        for (int i = 0; i < less_constraints.size(); i++) {
            if (less_constraints[i].first == temp || less_constraints[i].second == temp) {
                //GAC_constrains.push(less_constraints[i]);
                
                relatedconstrains.push(less_constraints[i]);
            }
        }
    }

    //删除候选值,cache记录在哪个位置删除了哪个元素，方便回溯
    void check(int x, int y, vector<pair<pair<int, int>, int>>& catches) {
        //缓存，如果失败可能需要回溯
        //vector<pair<pair<int, int>, int>> catches;
        //纵向横向删除候选值：
        for (int i = 0; i < nRow; i++) {
            //如果本来就是false，那就没必要加入缓存了
            if (!maps[i][y].digits[maps[x][y].val]) {
                maps[i][y].digits[maps[x][y].val] = true;
                //意思是在[i,y]格子的候选者中删除了maps[x][y].val候选者
                catches.push_back({ {i, y}, maps[x][y].val });
            }

            if (!maps[x][i].digits[maps[x][y].val]) {
                maps[x][i].digits[maps[x][y].val] = true;
                catches.push_back({ {x, i}, maps[x][y].val });
            }
        }

        //删除关于限制的候选值
        /*for (int i = 0; i < less_constraints.size(); i++) {
            if (x == less_constraints[i].first.first && y == less_constraints[i].first.second) {
                for (int j = maps[x][y].val; j >= 1; j--) {
                    if (!maps[less_constraints[i].second.first][less_constraints[i].second.second].digits[j]) {
                        maps[less_constraints[i].second.first][less_constraints[i].second.second].digits[j] = true;
                        catches.push_back({ {less_constraints[i].second.first, less_constraints[i].second.second}, j });
                    }
                }
            }

            if (x == less_constraints[i].second.first && y == less_constraints[i].second.second) {
                for (int j = 1; j <= maps[x][y].val; j++) {
                    if (!maps[less_constraints[i].first.first][less_constraints[i].first.second].digits[j]) {
                        maps[less_constraints[i].first.first][less_constraints[i].first.second].digits[j] = true;
                        catches.push_back({ {less_constraints[i].first.first, less_constraints[i].first.second}, j });
                    }
                }
            }
        }*/
        
        //return catches;
    }

    bool can_be_satisfied(int x, int y, int val) {
        queue<constrain> related;
        enqueueConstrains(x, y, related);
        bool can_satis = false;
        while (related.size()) {
            can_satis = false;
            constrain temp = related.front();
            related.pop();
            //如果这个点是约束中较小的那一个
            if (x == temp.first.first && y == temp.first.second) {
                //那么我就对较大的那一个进行检查
                //如果较大的那一个还没有被赋值
                if (maps[temp.second.first][temp.second.second].val == 0) {
                    for (int i = 1; i < nRow + 1; i++) {
                        if (!maps[temp.second.first][temp.second.second].digits[i]) {
                            //说明这个约束存在可满足状态
                            if (i > val) {
                                can_satis = true;
                                break;
                            }
                        }
                    }
                    if (!can_satis) return false;                                    
                }
                else {
                    if (maps[temp.second.first][temp.second.second].val < val) {
                        return false;
                    }
                    else {
                        can_satis = true;
                    }
                }
            }
            else if (x == temp.second.first && y == temp.second.second) {
                if (maps[temp.first.first][temp.first.second].val == 0) {
                    for (int i = 1; i < nRow + 1; i++) {
                        if (!maps[temp.first.first][temp.first.second].digits[i]) {
                            //说明这个约束存在可满足状态
                            if (i < val) {
                                can_satis = true;
                                break;
                            }
                        }
                    }
                    if (!can_satis) return false;
                }
                else {
                    if (maps[temp.first.first][temp.first.second].val > val) {
                        return false;
                    }
                    else {
                        can_satis = true;
                    }
                }
            }                      
        }
        return can_satis;
    }

    //行约束和列约束是我们已知的，如果一个值发生了变化那么一定要入队行列约束
    //所以可以不入队这两个约束，而是每次结束都检测
    void GACEnforce(vector<pair<pair<int, int>, int>>& catches) {
        //vector<pair<pair<int, int>, int>> catches;
        bool res = true;
        while (GAC_constrains.size()) {           
            constrain c = GAC_constrains.front();
            GAC_constrains.pop();
            int cnt = catches.size();
            for (int i = 1; i < nRow + 1; i++) {
                if (!maps[c.first.first][c.first.second].digits[i]) {
                    res = can_be_satisfied(c.first.first, c.first.second, i);
                    if (!res) {
                        maps[c.first.first][c.first.second].digits[i] = true;
                        //这一轮的GAC检测在这个位置上删掉了这个值
                        catches.push_back({ { c.first.first, c.first.second },i });                           
                    }
                }
            }
            //如果删掉了值，那么相关约束要重新入队
            if (catches.size() > cnt) enqueueConstrains(c.first.first, c.first.second, GAC_constrains);
             
            cnt = catches.size();
            for (int i = 1; i < nRow + 1; i++) {
                if (!maps[c.second.first][c.second.second].digits[i]) {
                    bool res = can_be_satisfied(c.second.first, c.second.second, i);
                    if (!res) {
                        maps[c.second.first][c.second.second].digits[i] = true;
                        catches.push_back({ {c.second.first,c.second.second},i });
                        enqueueConstrains(c.second.first, c.second.second, GAC_constrains);
                    }
                }
            }
            if (catches.size() > cnt) enqueueConstrains(c.second.first, c.second.second, GAC_constrains);
        }

        //return catches;
    }

    bool search(int x, int y) {     
        //this->show();
        if (maps[x][y].val == 0) {
            for (int i = 1; i < nRow + 1; i++) {
                //还在队列中没有被访问过
                if (!maps[x][y].digits[i]) {
                    vector<pair<pair<int, int>, int>> catches;
                    maps[x][y].val = i;
                    for (int j = 0; j < nRow; j++) {
                        if (j != i && !maps[x][y].digits[j]) {
                            maps[x][y].digits[j] = true;
                            catches.push_back({ {x,y},j });
                        }
                    }

                    //返回缓存，缓存存储了三个特征，横坐标，纵坐标，删除的值。恢复只需要把候选队列中的值变为true即可。
                    //vector<pair<pair<int, int>, int>> catches = check(x, y);
                    
                    check(x, y, catches);
                    enqueueConstrains(x, y, GAC_constrains);
                    GACEnforce(catches);
                    if (x == nRow-1 && y == nRow-1) {
                        return true;
                    }
                    int next_x, next_y;
                    if (y != nRow-1) {
                        next_x = x;
                        next_y = y + 1;
                    }
                    else {
                        next_x = x + 1;
                        next_y = 0;
                    }


                    if (search(next_x, next_y)) {
                        return true;
                    }
                    //查找失败，需要回退到上一版本。这里回退比较简单，只需要设置true即可
                    pull_back(catches);
                    maps[x][y].val = 0;
                }
            }
        }
        else {
            //当前位置已经填上，只需要跳过即可
            if (x == nRow-1 && y == nRow-1) {
                return true;
            }
            int next_x, next_y;
            if (y != nRow-1) {
                next_x = x;
                next_y = y + 1;
            }
            else {
                next_x = x + 1;
                next_y = 0;
            }


            if (search(next_x, next_y)) {
                return true;
            }
        }
        return false;
    }

    //倒退。根据缓存倒退上一版本
    void pull_back(vector<pair<pair<int, int>, int>> catches) {
        for (int i = 0; i < catches.size(); i++) {
            int x = catches[i].first.first;
            int y = catches[i].first.second;
            int digit = catches[i].second;
            maps[x][y].digits[digit] = false;
        }
    }

    void show() {
        for (int i = 0; i < nRow; i++) {
            for (int j = 0; j < nColumn; j++) {
                cout << maps[i][j].val << " ";
            }
            cout << endl;
        }
        cout << "======================" << endl;
    }

    void showconstrain() {
        for (int i = 0; i < less_constraints.size(); i++) {
            cout << less_constraints[i].first.first << less_constraints[i].first.second << less_constraints[i].second.first << less_constraints[i].second.second << endl;
        }
    }
};

int main(int argc,char* argv[]) {
    FutoshikiPuzzle* futoshikiPuzzle = new FutoshikiPuzzle();
    //初始化
    futoshikiPuzzle->initial(5);
    //显示空表
    futoshikiPuzzle->show();

    ////记录时间
    //clock_t start, end;
    //start = clock();
    ////开始搜索
    futoshikiPuzzle->search(0, 0);
    //end = clock();

    ////显示最终结果
    futoshikiPuzzle->show();

    //cout << "FC Time cost : " << (double)(end - start) / CLOCKS_PER_SEC << " s" << endl;
}