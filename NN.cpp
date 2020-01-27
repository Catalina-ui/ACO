#include <iostream>
#include <vector>
using namespace std;

vector<int> NN(int pos)
{
    vector<int> route(N, pos);
    vector<bool> visited(N, false);
    visited[pos] = true;
    int now = 1;
    while (now < N)
    {
        int vMin = 2147483647, iMin = N + 1;
        for (int i = 0; i < N; i++)
        {
            if (!visited[i] && vMin > dis[route[now - 1]][i])
            {
                vMin = dis[route[now - 1]][i];
                iMin = i;
            }
        }
        visited[iMin] = true;
        route[now] = iMin;
        now++;
    }
    return route;
}
