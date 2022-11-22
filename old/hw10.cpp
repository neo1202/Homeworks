#include <iostream>
#include <stack>
#include <algorithm>
using namespace std;
string N;

bool checkFront(int &N_front, int &N_back)
{
    for (int i = 0; i < (N_back + 1 - N_front) / 2; i++)
    { //比幾次,最多剩餘的一半
        stack<char> tmpStack;
        int k = N_front + i + 1;
        for (int j = N_front; j <= N_front + i; j++)
        {
            tmpStack.push(N[j]);
        }
        while (!tmpStack.empty())
        {
            if (tmpStack.top() == N[k])
            {
                tmpStack.pop();
                k++;
            }
            else
            {
                break;
            }
        }
        if (tmpStack.empty())
        {
            N_front = N_front + i + 1;
            return true;
        }
    }
    return false;
}
bool checkBack(int &N_front, int &N_back)
{
    for (int i = 0; i < (N_back + 1 - N_front) / 2; i++)
    { //比幾次,最多剩餘的一半
        stack<char> tmpStack;
        int k = N_back - i - 1;
        for (int j = N_back; j >= N_back - i; j--)
        {
            tmpStack.push(N[j]);
        }
        while (!tmpStack.empty())
        {
            if (tmpStack.top() == N[k])
            {
                tmpStack.pop();
                k--;
            }
            else
            {
                break;
            }
        }
        if (tmpStack.empty())
        {
            N_back = N_back - i - 1;
            return true;
        }
    }
    return false;
}

int main()
{
    int successf, successb;
    cin >> N;
    //cout << N[1] << "," << N.length() << endl;
    int N_front = 0;
    int N_back = N.length() - 1; //前後
    while (1)
    {
        successf = checkFront(N_front, N_back);
        //cout << "F:" << successf << " ," << N_front << " ," << N_back << endl;
        successb = checkBack(N_front, N_back);
        //cout << "B:" << successb << " ," << N_front << " ," << N_back << endl;
        if (successf == 0 and successb == 0)
        {
            break;
        }
        if (N_back <= N_front)
        {
            break;
        }
    }
    //cout << "N_back = " << N_back << ", N_front = " << N_front << endl;
    cout << N_back - N_front + 1 << endl;
    return 0;
}