#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

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
