#include <iostream>
#include <vector>
using namespace std;

vector<int> NN(vector<vector<int>> dis,int pos){
    int dis_size = dis.size();
    vector<int> route(dis_size + 1,pos);
    vector<bool> visited(dis_size, false);
    visited[pos] = true;
    int now = 1;
    while(now <= dis_size){
        int vMin = 2147483647,iMin = dis_size + 1;
        for (int i = 0; i < dis_size;i++){
            if(!visited[i] && vMin > dis[route[now-1]][i]){
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
