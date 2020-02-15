int main(){
    vector<int> ind(N, 0);
    vector<int> X(N, 0);
    vector<int> Y(N, 0);
    ofstream total_data("p51.csv");
    ifstream ifs(allfilename[Nnumber]);
    int cnt = 0;

    //load data file
    while (!ifs.eof()){
        string s;
        cnt++;
        if (cnt < 7){
            getline(ifs, s);
        }else{
            string a, b, c;
            ifs >> a;
            if (a == "EOF")
                break;
            ifs >> b;
            ifs >> c;
            int in = cnt - 7;
            ind[in] = stoi(a);
            X[in] = stoi(b);
            Y[in] = stoi(c);
        }
    }

    //calculate distance
    vector<vector<int>> dis(N, vector<int>(N, 0));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++){
            dis[i][j] = (int)(sqrt((X[i] - X[j]) * (X[i] - X[j]) + (Y[i] - Y[j]) * (Y[i] - Y[j])) + 0.5);
        }
    int g = 0;
    vector<vector<int>> gen(0, vector<int>(2));
    for (int t = 0; t < 10; t++)
    {
        int L = 0;
        //NN-method
        vector<int> J = NN(dis, 0);
        J = twoOpt(dis, J);

        for (int i = 0; i < N; i++)
            L += dis[J[i]][J[i + 1]];
        if(t == 0)
            gen.push_back({0, L});
        srand(seed[t]);
        //ACO-algorithm
        //calclate first pheromone
        vector<vector<double>> phe(N, vector<double>(N, (double)M / L));
        int repeat = 0;
        int beL = L;
        while(repeat <200){
            vector<vector<double>> afterphe(N, vector<double>(N, 0));
            for (int k = 0; k < M;k++){
                int f = (double)N * rand() / (RAND_MAX + 1.0);
                vector<int> visit(N, 0);
                for (int i = 1; i < N;i++)
                    visit[i] = i;
                vector<int> J(N + 1, 0);
                J[0] = f, J[N] = f;
                visit.erase(visit.begin()+f);

                for (int i = 1; i < N;i++){
                    vector<double> P(N - i, 0);
                    double Psum = 0;
                    for (int j = 0; j < N - i;j++){
                        P[j] = phe[J[i - 1]][visit[j]] * pow((double)1/dis[J[i - 1]][visit[j]], b);
                        Psum += P[j];
                    }
                    
                    for (int j = 0; j < N - i;j++)
                        P[j] = P[j] / Psum;

                    double pro = (double)rand() / (RAND_MAX + 1.0);
                    int pix = 0;
                    double pp = P[0];
                    while(pp < pro){
                        pix++;
                        pp += P[pix];
                    }
                    J[i] = visit[pix];
                    visit.erase(visit.begin() + pix);
                }
                J = twoOpt(dis, J);
                double tL = 0;

                for (int i = 0; i < N; i++){
                    tL += (double)dis[J[i]][J[i + 1]];
                    //cout << (double)dis[J[i]][J[i + 1]] << " ";
                }
                //cout << endl;
                for (int i = 0; i < N; i++)
                    afterphe[J[i]][J[i + 1]] += 1.0 / tL;
                if(L > tL)
                    L = tL;
            }
            for (int i = 0; i < N;i++)
                for (int j = 0; j < N;j++)
                    phe[i][j] = (1.0 - ro) * phe[i][j] + afterphe[i][j];

            if(beL > L){
                if(t == 0){
                    gen.push_back({gen[g][0] + 1 +repeat,L});
                    g++;
                }
                beL = L;
                repeat = 0;
            }else{
                repeat++;
            }
        }
        cout << beL << endl;
    }
    int gens = gen.size();
    for (int i = 0; i < gens;i++){
        for (int j = 0; j < 2;j++){
            total_data << gen[i][j] << ",";
        }
        total_data << endl;
    }
    return 0;
}
