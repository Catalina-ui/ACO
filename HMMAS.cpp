#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
typedef long long ll;
using namespace std;
//static double a = 1;
static double b = 2;
vector<double> roList = {0.8, 0.5, 0.2, 0.1, 0.02, 0.01};
vector<double> pbestList = {0.5, 0.1, 0.05, 0.01, 0.005, 0.001};
double ro = roList[3];
double pbest = pbestList[2];
static const vector<int> seed = {101, 103, 107, 109, 113, 127, 131, 137, 139, 149};
static vector<string> allfilename = {"eil51.tsp", "pr76.tsp", "rat99.tsp", "kroA100.tsp", "ch130.tsp"};
static vector<int> allN = {51, 76, 99, 100, 130};
static int Nnumber = 0;
static int N = allN[Nnumber];
static int M = 25;
vector<vector<int>> dis(N, vector<int>(N, 0));

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

int main()
{
    double avg = 0;
    vector<double> X(N, 0);
    vector<double> Y(N, 0);
    ofstream total_data("h1m51.csv");
    ifstream ifs(allfilename[Nnumber]);
    int cnt = 0;

    //load data file
    while (!ifs.eof())
    {
        string s;
        cnt++;
        if (cnt < 7)
        {
            getline(ifs, s);
        }
        else
        {
            string a, b, c;
            ifs >> a;
            if (a == "EOF")
                break;
            ifs >> b;
            ifs >> c;
            int in = cnt - 7;
            X[in] = stod(b);
            Y[in] = stod(c);
        }
    }
    
    //calculate distance
    for (int i = 0; i < N-1; i++)
        for (int j = i+1; j < N; j++)
        {
            dis[i][j] = (double)(sqrt((X[i] - X[j]) * (X[i] - X[j]) + (Y[i] - Y[j]) * (Y[i] - Y[j])) + 0.5);
            dis[j][i] = dis[i][j];
        }

    int g = 0;
    vector<int> gen(0);
    for (int t = 0; t < 1; t++)
    {
        srand(seed[t]);
        //NN-method and 2-opt
        vector<int> fJ = NN((double)N * rand() / (RAND_MAX + 1.0));
        fJ = twoOpt(fJ);
        fJ.push_back(fJ[0]);
        int L = 0;
        for (int i = 0; i < N; i++)
            L += dis[fJ[i]][fJ[i + 1]];
        if (t == 0)
            gen.push_back(L);

        //ACO-algorithm
        //calclate first pheromone
        vector<vector<double>> phe(N, vector<double>(N,0));
        for (int i = 0; i < N-1;i++)
            for (int j = i+1; j < N;j++){
                phe[i][j] = 1. / (ro * (double)L);
                phe[j][i] = phe[i][j];
            }
        int repeat = 0;
        //全体の最良解とその順路
        double bestL = L;
        vector<int> bestJ = fJ;

        //各イテレーションの最良解とその順路
        double bL = L;
        vector<int> bJ = fJ;
        bool ch = false;
        vector<vector<double>> afterphe(N, vector<double>(N, 0));
        while (repeat < 200)
        {
            for (int i = 0; i < N-1;i++)
                for (int j = i+1; j < N;j++){
                    afterphe[i][j] = 0;
                    afterphe[j][i] = 0;
                }

            double pmax = 1.0 / (ro * bL);
            double powPbest = pow(pbest, (double)1 / N);
            double pmin = ((1. - powPbest) / (((double)N / 2. - 1.) * powPbest)) * pmax;
            
            
            if (ch)
            {
                for (int i = 0; i < N; i++)
                {
                    afterphe[bJ[i]][bJ[i + 1]] = 1.0 / bL;
                    afterphe[bJ[i + 1]][bJ[i]] = 1.0 / bL;
                }
                for (int i = 0; i < N; i++)
                    for (int j = 0; j < N; j++)
                    {
                        phe[i][j] = (1.0 - ro) * phe[i][j] + afterphe[i][j];
                        if (phe[i][j] > pmax)
                            phe[i][j] = pmax;
                        if (phe[i][j] < pmin)
                            phe[i][j] = pmin;
                    }
            }
            bL = 100000000;
            ch = true;
            for (int k = 0; k < M; k++)
            {
                int f = 0; //(double)N * rand() / (RAND_MAX + 1.0);
                vector<int> visit(N, 0);
                for (int i = 1; i < N; i++)
                    visit[i] = i;
                vector<int> J(N, 0);
                J[0] = f;
                visit.erase(visit.begin() + f);

                for (int i = 1; i < N; i++)
                {
                    vector<double> P(N - i, 0);
                    double Psum = 0;
                    for (int j = 0; j < N - i; j++)
                    {
                        double tphe = phe[J[i - 1]][visit[j]];
                        P[j] = tphe * pow((double)1 / dis[J[i - 1]][visit[j]], b);
                        Psum += P[j];
                    }

                    for (int j = 0; j < N - i; j++)
                        P[j] = P[j] / Psum;

                    double pro = (double)rand() / (RAND_MAX + 1.0);
                    int pix = 0;
                    double pp = P[0];
                    while (pp < pro)
                    {
                        pix++;
                        pp += P[pix];
                    }
                    J[i] = visit[pix];
                    visit.erase(visit.begin() + pix);
                }

                //2-opt
                J = twoOpt(J);
                J.push_back(J[0]);

                int tL = 0;
                for (int i = 0; i < N; i++)
                    tL += dis[J[i]][J[i + 1]];


                //cout << endl;
                if (bL > tL)
                {
                    bL = tL;
                    bJ = J;
                }
            }

            if (bestL > bL)
            {
                if (t == 0)
                {
                    gen.push_back((int)bL);
                    g++;
                }
                bestL = bL;
                bestJ = bJ;
                repeat = 0;
            }
            else
            {
                repeat++;
                if (t == 0)
                {
                    gen.push_back((int)bestL);
                }
            }
        }
        avg += bestL;
        cout << bestL << " ";
        cout << t*10 + 10 << "% finished." << endl;

        //cout << pL << endl;
    }
    cout << avg / 10 << endl;
    int gens = gen.size();
    for (int i = 0; i < gens; i++)
    {
        total_data << gen[i];
        total_data << endl;
    }
    return 0;
}
