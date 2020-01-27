#include <iostream>
#include <vector>
using namespace std;

vector<int> Improve(vector<int> r){
    for (int i = 0; i < N - 2; i++)
    {
        int iNext = i + 1;

        for (int j = i + 2; j < N; j++)
        {
            int jNext = j + 1;
            if (j == N - 1)
                jNext = 0;
            if (i != 0 || jNext != 0)
            {
                if (dis[r[i]][r[iNext]] + dis[r[j]][r[jNext]] > dis[r[i]][r[j]] + dis[r[iNext]][r[jNext]])
                {
                    int tj = j;
                    int ti = iNext;
                    while (tj - ti > -1)
                    {
                        swap(r[ti], r[tj]);
                        tj--;
                        ti++;
                    }
                }
            }
        }
    }
    return r;
}

vector<int> twoOpt(vector<int> r)
{
    vector<int> nr = Improve(r);
    while(nr != r){
        r = nr;
        nr = Improve(r);
    }
    return nr;
}
