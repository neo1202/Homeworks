#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
using namespace std;

int main()
{
    int tmp;
    int i;
    int player[4][13];
    int countCard[4];
    for (i = 0; i < 4; i++)
    {
        countCard[i] = 0;
    }
    for (i = 0; i < 52; i++)
    {
        tmp = ((i ^ 3) * 13 / 7) % 4;
        cout << tmp << " ";
        while (countCard[tmp] >= 13)
        {
            tmp = (tmp + 1) % 4;
        }
        player[tmp][countCard[tmp]] = i;
        countCard[tmp]++;
    }
    for (i = 0; i < 4; i++)
    {
        for (int j = 0; i < 13; j++)
        {
            cout << player[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
