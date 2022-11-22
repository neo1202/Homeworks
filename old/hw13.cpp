#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

void findPath(int **matrix, int weightArray[], int from, int all_num)
{
    int *passed = new int[all_num]; //創造一個走過的點set
    for (int i = 0; i < all_num; i++)
    {
        passed[i] = 0; //歸零代表都沒走過
        weightArray[i] = matrix[from][i];
        //cout << "weightArray :" << i << "  " << weightArray[i] << endl;
    }
    passed[from] = 1; //開始的那人先走過了
    //cout << "firstpassed: " << from << endl;
    for (int i = 1; i < all_num; i++)
    {
        int who = -1, mini = 222222222;
        for (int j = 0; j < all_num; j++)
        {
            if (passed[j] == 0 and weightArray[j] < mini)
            {
                who = j;
                mini = weightArray[j];
            }
        }
        passed[who] = 1; //找到這次是誰
        //cout << "passed: " << who << endl;
        for (int k = 0; k < all_num; k++)
        {
            if (passed[k] == 0)
            {
                if (weightArray[k] > weightArray[who] + matrix[who][k])
                {
                    weightArray[k] = weightArray[who] + matrix[who][k];
                }
            }
        }
    }
    //for (int i = 0; i < all_num; i++)
    //{
    //    cout << weightArray[i] << " ";
    //}
    //cout << endl;
}

int main()
{
    int tmp_num; //輸入時候用來存stoi
    int all_num = 0;
    string token;
    string delimiter = " "; //用空白切
    vector<int> vec;
    int **matrix = new int *[1000]; //先創一個matrix,每次遇到n=1會清空內容
    for (int i = 0; i < 1000; i++)
    {
        matrix[i] = new int[1000];
        for (int j = 0; j < 1000; j++)
        {
            matrix[i][j] = 10000000;
        }
    }
    while (cin.peek() != EOF) //每一行都確認
    {
        vec.clear();
        string s; //用字串存
        getline(cin, s);
        int pos = 0;
        pos = s.find(delimiter);
        while (pos != string::npos)
        {
            token = s.substr(0, pos);
            tmp_num = stoi(token);
            vec.push_back(tmp_num);
            s.erase(0, pos + delimiter.length());
            pos = s.find(delimiter);
        }
        if (s != "#")
        { //如果是的話什麼也不幹
            vec.push_back((stoi(s)));
            int n = vec.size(); //n有1,2,3的情況
            //如果是1, 代表循環的開始, 初始化matrix
            //如果是2, 第一個數字是從誰, 第二個數字是到誰, 最後輸出
            //如果是3, 增加matrix中數值
            if (n == 1)
            {
                all_num = vec[0]; //這次最高到多少
                for (int i = 0; i < 1000; i++)
                {
                    for (int j = 0; j < 1000; j++)
                    {
                        matrix[i][j] = 10000000;
                    }
                }
            }
            else if (n == 3)
            {
                matrix[vec[0]][vec[1]] = vec[2];
            }
            else if (n == 2)
            {
                int weightArray[1000];
                for (int i = 0; i < 1000; i++)
                {
                    weightArray[i] = 100000; //初始最大化
                }
                //cout << "all_num: " << all_num << endl;
                findPath(matrix, weightArray, vec[0], all_num);
                /*
                for (int i = 0; i < 10; i++)
                {
                    cout << weightArray[i] << " ";
                }
                cout << endl
                     << "vec: " << vec[0] << " " << vec[1] << endl;
                */
                cout << weightArray[vec[1]] << endl;
            }
        }
    }

    return 0;
}