/*
	�Ż�˼·��MRV�㷨ÿ��Ѱ������dom_num��ֵ
	ά��һ��vector<vector<int>>dom_num�������ʾ�Ƿ������ָ�����
	����ʽѡȡ������һ��������һ������
	ֱ�����ж���������
*/
#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include <fstream>
#include <string>
#include <queue>

using namespace std;

typedef pair<pair<int, int>, pair<int, int>> constrain;
#define NUM_OF_TEST 1


class FutoshikiPuzzle {
public:
	//��¼��ǰ��
	class node {
	public:
		//��ǰλ�õ�ֵ
		int val = 0;
		//��ѡ���У�false�������ѡ��
		//����ʮ���ռ䵫�ǿ���ֻ��ǰ����
		bool digits[10] = { false, false, false, false, false, false, false, false, false, false };
	};

	struct file {
		int numConstrains;
		int numRow;
		string filename;
	};

	//��ͼ
	vector<vector<node>> maps;
	//��ŷǷ������ָ�������ı����ȸ�ֵ
	vector<vector<int>>dom_num;
	//��ʱ�������ڱ���ֵ
	vector<vector<int>> copy_dom_num;

	//����
	vector<pair<pair<int, int>, pair<int, int>>> less_constraints;
	vector<file> allfile;
	queue<constrain> GAC_constrains;
	int nRow, nColumn;
	int cnt = 0; //���������Ѿ������Ľڵ�

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

		//���Ƶ���ʵ��ͼ��
		for (int i = 0; i < nRow; i++) {
			maps[i].resize(nColumn);
			for (int j = 0; j < nColumn; j++) {
				maps[i][j].val = copy_map[i][j];
			}
		}

		//�������
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

		//�����Ѿ���д�����ֿ�ʼɾ������λ�õĺ�ѡֵ
		for (int i = 0; i < nRow; i++) {
			for (int j = 0; j < nColumn; j++) {
				//vector<pair<pair<int, int>, int>> catches = check(i, j);
				vector<pair<pair<int, int>, int>> catches;
				check(i, j, catches);
			}
		}
	}

	//filename��·���������Ҫ��ͬһ�ļ�����
	void initial(int filenum)
	{
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

		//��һ�������ļ���ȡ��ʼ��
		fstream f;
		f.open(allfile[filenum].filename);
		vector<vector<int>> copy_map;
		copy_map.resize(allfile[filenum].numRow);
		string str;
		if (f.is_open()) {
			//��ȡ��ͼ
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
			//��ȡԼ��
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

		//���������еĳ�ʼ���ͽ�����


		//���Ƶ���ʵ��ͼ��
		nRow = copy_map.size();
		nColumn = copy_map[0].size();
		maps.resize(nRow);
		for (int i = 0; i < nRow; i++) {
			maps[i].resize(nColumn);
			for (int j = 0; j < nColumn; j++) {
				maps[i][j].val = copy_map[i][j];
			}
		}

		//������ĸ���
		dom_num.resize(nRow);
		for (int i = 0; i < nRow; i++)
		{
			dom_num[i].resize(nColumn);
			for (int j = 0; j < nColumn; j++)
			{
				dom_num[i][j] = 0;//��ʾ�ж��ٷǷ�������
			}
		}


		copy_dom_num.resize(nRow);
		for (int i = 0; i < nRow; i++)
		{
			copy_dom_num[i].resize(nColumn);
		}

		//���г�ʼ��GAC���
		//����������ѭ���Ƕ���ÿһ��ÿһ�У�Ҳ����ÿһ��Ԫ��
		for (int x = 0; x < nRow; x++)
		{
			for (int y = 0; y < nColumn; y++)
			{
				//������ɾ�����еĺ�ѡֵ��û��ʹ��GAC
				for (int i = 0; i < nRow; i++) {
					//�����������false���Ǿ�û��Ҫ���뻺����
					if (!maps[i][y].digits[maps[x][y].val]) {
						maps[i][y].digits[maps[x][y].val] = true;
						dom_num[i][y]++;
						//��˼����[i,y]���ӵĺ�ѡ����ɾ����maps[x][y].val��ѡ��
						//catches.push_back({ {i, y}, maps[x][y].val });
					}

					if (!maps[x][i].digits[maps[x][y].val]) {
						maps[x][i].digits[maps[x][y].val] = true;
						dom_num[x][i]++;
						//catches.push_back({ {x, i}, maps[x][y].val });
					}
				}

				//ɾ���������Ƶĺ�ѡֵ
				//�����õ���GAC��⣬���Ƕ����λ�õ������е�ÿһ��ֵ���ж��ǲ�����һ����Ч֧�ţ���ΪԼ��ֻ�Ǽ򵥵ıȽϴ�С��������ʵ��һ�������в����㲻��ʽ�Ķ�������Ч֧��
				//��������Ϊtrue�Ĺ�����ʼ����GAC���ɾ����Ч֧�ŵĹ���

				for (int i = 0; i < less_constraints.size(); i++) {
					if (x == less_constraints[i].first.first && y == less_constraints[i].first.second) {
						maps[x][y].digits[nRow] = true;
						dom_num[x][y]++;
						for (int j = maps[x][y].val; j >= 1; j--) {
							if (!maps[less_constraints[i].second.first][less_constraints[i].second.second].digits[j]) {
								maps[less_constraints[i].second.first][less_constraints[i].second.second].digits[j] = true;
								dom_num[less_constraints[i].second.first][less_constraints[i].second.second]++;
								//catches.push_back({ {less_constraints[i].second.first, less_constraints[i].second.second}, j });
							}
						}
					}

					if (x == less_constraints[i].second.first && y == less_constraints[i].second.second) {
						maps[x][y].digits[1] = true;
						dom_num[x][y]++;
						if (maps[x][y].val != 0) {
							for (int j = maps[x][y].val; j <= nRow; j++) {
								if (!maps[less_constraints[i].first.first][less_constraints[i].first.second].digits[j]) {
									maps[less_constraints[i].first.first][less_constraints[i].first.second].digits[j] = true;
									dom_num[less_constraints[i].first.first][less_constraints[i].first.second]++;
									//catches.push_back({ {less_constraints[i].first.first, less_constraints[i].first.second}, j });
								}
							}
						}


					}
				}
			}
		}

		//print_dom();
	}//END OF INITIAL

	void addConstraints(int x, int y, int x1, int y1) {
		less_constraints.push_back({ {x,  y},
									{x1, y1} });
	}

	//���dom�Ĳ���ȡ�����ָ���������������ѡ��ֵ
	void print_dom()
	{
		cout << "------------dom_num-------------\n";
		//test to printout dom_num
		for (int i = 0; i < nRow; i++)
		{
			for (int j = 0; j < nColumn; j++)
			{
				cout << dom_num[i][j] << " ";
			}
			cout << endl;
		}
		cout << "-------------------------------\n";
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

	//ɾ����ѡֵ,cache��¼���ĸ�λ��ɾ�����ĸ�Ԫ�أ��������
	void check(int x, int y, vector<pair<pair<int, int>, int>>& catches) {
		//���棬���ʧ�ܿ�����Ҫ����
		//vector<pair<pair<int, int>, int>> catches;
		//�������ɾ����ѡֵ��
		for (int i = 0; i < nRow; i++) {
			//�����������false���Ǿ�û��Ҫ���뻺����
			if (!maps[i][y].digits[maps[x][y].val]) 
			{
				maps[i][y].digits[maps[x][y].val] = true;
				
				//��˼����[i,y]���ӵĺ�ѡ����ɾ����maps[x][y].val��ѡ��
				catches.push_back({ {i, y}, maps[x][y].val });
			}

			if (!maps[x][i].digits[maps[x][y].val]) {
				maps[x][i].digits[maps[x][y].val] = true;
				
				catches.push_back({ {x, i}, maps[x][y].val });
			}
		}

	}

	bool can_be_satisfied(int x, int y, int val) {
		queue<constrain> related;
		enqueueConstrains(x, y, related);
		bool can_satis = false;
		while (related.size()) {
			can_satis = false;
			constrain temp = related.front();
			related.pop();
			//����������Լ���н�С����һ��
			if (x == temp.first.first && y == temp.first.second) {
				//��ô�ҾͶԽϴ����һ�����м��
				//����ϴ����һ����û�б���ֵ
				if (maps[temp.second.first][temp.second.second].val == 0) {
					for (int i = 1; i < nRow + 1; i++) {
						if (!maps[temp.second.first][temp.second.second].digits[i]) {
							//˵�����Լ�����ڿ�����״̬
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
							//˵�����Լ�����ڿ�����״̬
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

	//��Լ������Լ����������֪�ģ����һ��ֵ�����˱仯��ôһ��Ҫ�������Լ��
	//���Կ��Բ����������Լ��������ÿ�ν��������
	void GACEnforce(vector<pair<pair<int, int>, int>>& catches) {
		//vector<pair<pair<int, int>, int>> catches;
		bool res = true;
		while (GAC_constrains.size()) 
		{
			constrain c = GAC_constrains.front();
			GAC_constrains.pop();
			int cnt = catches.size();
			for (int i = 1; i < nRow + 1; i++) {
				if (!maps[c.first.first][c.first.second].digits[i]) {
					res = can_be_satisfied(c.first.first, c.first.second, i);
					if (!res) {
						maps[c.first.first][c.first.second].digits[i] = true;
						
						//��һ�ֵ�GAC��������λ����ɾ�������ֵ
						catches.push_back({ { c.first.first, c.first.second },i });
					}
				}
			}
			//���ɾ����ֵ����ô���Լ��Ҫ�������
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

	/*bool search(int x, int y) {
		//this->show();
		cnt++;
		if (maps[x][y].val == 0) {
			for (int i = 1; i < nRow + 1; i++) {
				//���ڶ�����û�б����ʹ�
				if (!maps[x][y].digits[i]) {
					vector<pair<pair<int, int>, int>> catches;
					maps[x][y].val = i;
					for (int j = 0; j < nRow; j++) {
						if (j != i && !maps[x][y].digits[j]) {
							maps[x][y].digits[j] = true;
							catches.push_back({ {x,y},j });
						}
					}

					//���ػ��棬����洢�����������������꣬�����꣬ɾ����ֵ���ָ�ֻ��Ҫ�Ѻ�ѡ�����е�ֵ��Ϊtrue���ɡ�
					//vector<pair<pair<int, int>, int>> catches = check(x, y);

					check(x, y, catches);
					enqueueConstrains(x, y, GAC_constrains);
					GACEnforce(catches);
					if (x == nRow - 1 && y == nRow - 1) {
						return true;
					}
					int next_x, next_y;
					if (y != nRow - 1) {
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
					//����ʧ�ܣ���Ҫ���˵���һ�汾��������˱Ƚϼ򵥣�ֻ��Ҫ����true����
					pull_back(catches);
					maps[x][y].val = 0;
				}
			}
		}
		else {
			//��ǰλ���Ѿ����ϣ�ֻ��Ҫ��������
			if (x == nRow - 1 && y == nRow - 1) {
				return true;
			}
			int next_x, next_y;
			if (y != nRow - 1) {
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
	}*/

	//�ж��Ƿ�����ֹ״̬
	bool Goal()
	{
		//this->show();
		for (int i = 0; i < nRow; i++)
		{
			for (int j = 0; j < nColumn; j++)
			{
				if (maps[i][j].val == 0)
				{
					return false;
				}
			}
		}
		return true;
	}

	//MRV�㷨ȷ����һ����x��y
	void heuristicPick(int& x, int& y)
	{
		int maxi=0, maxj=0;
		//Ѱ�����dom_num��i,j
		for (int i = 0; i < nRow; i++)
		{
			for (int j = 0; j < nColumn; j++)
			{
				
				if (maps[i][j].val != 0)continue;//�Ѿ�����ֵ
				
				if (dom_num[i][j] > dom_num[maxi][maxj] || maps[maxi][maxj].val != 0)
				{
					maxi = i;
					maxj = j;
					if (dom_num[maxi][maxj] == nRow - 1)//�ҵ�ֻ��һ��ѡ���
					{
						//print_dom();
						x = maxi;
						y = maxj;
						return;
					}
				}
				
			}
		}
		
		//print_dom();
		x = maxi;
		y = maxj;
		return;
	}

	//��ֵ��ʱdom_num���ѵڶ���copy����һ��
	void copy_dom(vector<vector<int>>& A, vector<vector<int>>& B)
	{
		for (int i = 0; i < B.size(); i++)
		{
			for (int j = 0; j < B[0].size(); j++)
			{
				A[i][j] = B[i][j];
			}
		}
	}
	
	//����һ���ڵ�ķǷ�����
	void countDom(node n,int x,int y)
	{
		dom_num[x][y] = 0;
		for (int i = 0; i < nRow; i++)
		{
			if (n.digits[i] == true)
			{
				dom_num[x][y]++;
			}
		}
	}

	//��������
	bool search(int x, int y) 
	{
		cnt++;//search��Ŀ��һ

		if (Goal())
		{
			cout << "-----Goal!!-----\n";
			return true;
		}

		else if(maps[x][y].val==0)//û�и�ֵ
		{
			

			for (int i = 1; i < nRow + 1; i++) //��������
			{
				//���ڶ�����û�б����ʹ��͸�ֵ
				if (!maps[x][y].digits[i]) 
				{
					vector<pair<pair<int, int>, int>> catches;
					maps[x][y].val = i;
					
					//cout << "assign " << x << " , " << y << " Ϊ " << i << endl;

					//����
					copy_dom(copy_dom_num, dom_num);

					for (int j = 0; j < nRow; j++) 
					{
						if (j != i && !maps[x][y].digits[j]) 
						{
							maps[x][y].digits[j] = true;
							
							catches.push_back({ {x,y},j });
						}
					}
					
					//���ػ��棬����洢�����������������꣬�����꣬ɾ����ֵ���ָ�ֻ��Ҫ�Ѻ�ѡ�����е�ֵ��Ϊtrue���ɡ�
					//vector<pair<pair<int, int>, int>> catches = check(x, y);

					check(x, y, catches);
					enqueueConstrains(x, y, GAC_constrains);
					GACEnforce(catches);

					countDom(maps[x][y], x, y);
					//print_dom();

					//��ѡ��һ��	
					int next_x, next_y;
					heuristicPick(next_x, next_y);
					//cout << "next_x= " << next_x << " next_y= " << next_y << endl;

					if (search(next_x, next_y)) 
					{
						return true;
					}
					//����ʧ�ܣ���Ҫ���˵���һ�汾��������˱Ƚϼ򵥣�ֻ��Ҫ����true����
					pull_back(catches);

					//�ָ�
					copy_dom( dom_num, copy_dom_num);
					

					//cout << "�ָ� " << x << " , " << y << endl;
					maps[x][y].val = 0;//���Ϊû�и�ֵ
				}
			}
		}
		else//�Ѿ���ֵ
		{
			int next_x, next_y;
			heuristicPick(next_x, next_y);


			if (search(next_x, next_y)) {
				return true;
			}
		}
		return false;
	}

	//���ˡ����ݻ��浹����һ�汾
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

int main(int argc, char* argv[]) {
	FutoshikiPuzzle* futoshikiPuzzle = new FutoshikiPuzzle();
	//��ʼ��
	futoshikiPuzzle->initial(NUM_OF_TEST);
	//��ʾ�ձ�
	futoshikiPuzzle->show();

	////��¼ʱ��
	clock_t start, end;
	start = clock();
	////��ʼ����
	futoshikiPuzzle->search(0, 0);
	end = clock();

	////��ʾ���ս��
	futoshikiPuzzle->show();

	cout << "-----test case " << NUM_OF_TEST << " ----- \n" << endl;
	cout << "GAC Time cost : " << (double)(end - start) / CLOCKS_PER_SEC << " s" << endl;
	cout << "Total nodes expanded: " << futoshikiPuzzle->cnt << endl;

	return 0;
}