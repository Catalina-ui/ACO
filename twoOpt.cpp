#include <iostream>
#include <vector>
using namespace std;

vector<int> twoOpt(vector<vector<int>> dis,vector<int> route){
    for (int i = 0; i < (int)route.size()-1; i++)
    {
        int vMax = 0, iMax = route.size() + 1;

        for (int j = i + 2; j < (int)route.size()-1;j++){
            int tmp = dis[route[i]][route[i + 1]] + dis[route[j]][route[j + 1]] - dis[route[i]][route[j]] - dis[route[i + 1]][route[j + 1]];
            if(vMax < tmp && route[i] != route[j+1]){
                vMax = tmp;
                iMax = j;
            }
        }

        if (vMax != 0)
        {
            int r = i + 1, l = iMax;
            while(l - r > 0){
                swap(route[r], route[l]);
                r++;
                l--;
            }
        }
    }
    return route;
}
