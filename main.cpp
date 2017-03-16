#include <iostream>
#include <vector>
#include <fstream>
#include <cassert>
#include <set>
#include <random>
#include <ctime>
#include <utility>
#include <algorithm>
#include <string>

using namespace std;

#define FILE_PATH "siftsmall/"
#define DISTANCE_PATH "graph/distance"
#define INDEX_PATH "graph/index"
#define BASE_NUM 10000
#define K 100 //first K elements
#define E 100
#define k 1000 //knn graph
#define R 10
#define T 10
//#define BUILD

template <typename Type>
void print_vector(vector<vector<Type> > vec);
template <typename Type>
vector<vector<Type> > read_data(string, int);
float distance(vector<float>, vector<float>);
bool cmp(pair<int, float> a, pair<int, float> b);
void build_graph();
void read_graph();
vector<int> N(int Y, vector<vector <int> > G);
int argmin_p(int Y, vector<int> N, vector<float> Q, set<pair<int, float> > *S_U);
vector<int> search_K_nearest(vector<float> Q);// return
vector<int> search_K_nearest_(vector<float> Q);
vector<vector<float> > read_Q(int count);
vector<int> ground_truth(int Q, int count);//which query and return count results

vector<vector<float> > base_vectors; //coordinates of all points
vector<vector<int> > graph;

int main() {
    std::cout << "Hello, World!" << std::endl;
    base_vectors = read_data<float> ("siftsmall_base.fvecs", BASE_NUM);
    int nearest_count = 20;
    int query_count = 5;
//    print_vector(base_vectors);
#ifdef BUILD
    build_graph();
#else
    read_graph();
#endif
    vector<vector<float> > Q = read_Q(query_count);
    vector<int> truth;
    vector<int> predict;
    for(int i=0; i<query_count; i++) {
        predict = search_K_nearest(Q[i]);
        truth = ground_truth(i, nearest_count);
        for(int j=0; j<nearest_count; j++)
            cout << predict[j] << " ";
        cout << endl;
        for(int j=0; j<nearest_count; j++)
            cout << truth[j] << " ";
        cout << endl;
    }

    return 0;
}

template <typename Type>
void print_vector(vector<vector<Type> > vec){
    for(int i=0;i<2;i++){
        for(int j=0;j<128;j++) {
            cout << vec[i][j] << ' ';
            if(j % 10 == 0)
                cout << '\n';
        }
    }
    cout << endl;
}

template <typename Type>
vector<vector<Type> > read_data(string name, int count){
    ifstream f;
    int d;
    vector<vector<Type> > ans;

    string path = FILE_PATH + name;
    f.open(path, ios::in | ios::binary);
    assert(f.is_open());

    vector<Type> temp;
    for(int i=0; i<count; i++){
        f.read((char*)&d, 4);
        assert(d == 128);
        for(int j=0; j<d; j++){
            Type num;
            f.read((char*)&num, 4);
            temp.push_back(num);
        }
        ans.push_back(temp);
        temp.clear();
    }
    return ans;
}

float distance(vector<float> a, vector<float> b){
    assert(a.size() == b.size());
    float dis = 0;
    vector<float>::iterator aa = a.begin(), bb = b.begin();
    for(; aa != a.end(); aa++, bb++){
        dis += (*aa - *bb) * (*aa - *bb);
    }
    return dis;
}

bool cmp(pair<int, float> a, pair<int, float> b){
    return a.second < b.second;
}

pair<int, float> dis[BASE_NUM][BASE_NUM];
void build_graph(){
    ofstream index_file, distance_file;
    index_file.open(INDEX_PATH, ios::app);
    distance_file.open(DISTANCE_PATH, ios::app);
    assert(index_file.is_open() && distance_file.is_open());

    for(int i = 0; i < BASE_NUM; i++){
        cout << i << "/" << BASE_NUM << endl;
        for(int j = i ; j < BASE_NUM; j++){
            float temp = distance(base_vectors[i], base_vectors[j]);
            dis[i][j] = make_pair(j, temp);
            dis[j][i] = make_pair(i, temp);
//            cout << dis[i][j].first << " " <<dis[i][j].second << endl;
        }
        sort(dis[i], dis[i] + BASE_NUM, cmp);
        for(int j = 0; j < BASE_NUM; j++){
            index_file.write((char*)&(dis[i][j].first), 4);
            distance_file.write((char*)&(dis[i][j].second), 4);
        }
    }
    index_file.close();
    distance_file.close();
    return ;
}

void read_graph(){
    ifstream index_file, distance_file;
    index_file.open(INDEX_PATH, ios::in);
    distance_file.open(DISTANCE_PATH, ios::in);
    assert(index_file.is_open() && distance_file.is_open());

    vector<int> temp;
    for(int i = 0; i < BASE_NUM; i++){
        int index;
        for(int j = 0; j < k; j++) {
            index_file.read((char *) &index, 4);
            temp.push_back(index);
        }
        index_file.seekg(4 * (BASE_NUM-k), ios_base::cur);
        graph.push_back(temp);
        temp.clear();
    }
    index_file.close();
    distance_file.close();
    return ;
}

/*find the Y's E nearest neighbors in G*/
vector<int> N(int Y, vector<vector <int>> G){
    vector<int> ans;
    assert(k >= E);
    for(int i=1; i <= E; i++){ //because the value of index 0 is 0, which means the nearest point is itself
        ans.push_back(G[Y][i]);
    }
    return ans;
};

int argmin_p(int Y, vector<int> N, vector<float> Q, set<pair<int, float> > *S_U){
    float min = -1;
    int index = 0;
    for(auto iter = N.begin(); iter != N.end(); iter++){
        int t_index = *iter;
        vector<float> coor = base_vectors[t_index];
        float dis = distance(coor, Q);
        S_U -> insert(make_pair(t_index, dis));
        if(min == -1 || min > dis){
            min = dis;
            index = t_index;
        }
    }
    return index;
}

vector<int> search_K_nearest(vector<float> Q){
    set<pair<int, float> > *S_U = new set<pair<int, float> >;
    vector<int> ans;
    srand((unsigned)time(NULL));
    for (int r = 0; r < R; r++){
        int Y = rand() % BASE_NUM;
        for(int i = 0; i < T; i++) {
            vector<int> N_ = N(Y, graph);
            Y = argmin_p(Y, N_, Q, S_U); // modify the S_U at the same time
        }
    }
    vector<pair<int, float>> S_U_sort;
    copy(S_U->begin(), S_U->end(), back_inserter(S_U_sort));
    sort(S_U_sort.begin(), S_U_sort.end(), cmp);
    for(int i = 0; i < K ; i++){
        ans.push_back(S_U_sort[i].first);
    }
    delete(S_U);
    return ans;
}

vector<int> search_K_nearest_(vector<float> Q){
    vector<pair<int, float>> all;
    vector<int> ans;
    vector<float> temp;
    for(int i = 0; i < BASE_NUM; i++){
        temp = base_vectors[i];
        float dis = distance(temp, Q);
        all.push_back(make_pair(i, dis));
    }
    sort(all.begin(), all.end(), cmp);
    for(int i = 0; i < k; i++){
        ans.push_back(all[i].first);
    }
    return ans;
}

vector<vector<float> > read_Q(int count){
    ifstream file;
    vector<vector<float> > ans;
    string query = FILE_PATH + (string)("siftsmall_query.fvecs");
    file.open(query, ios::binary | ios::in);
    assert(file.is_open());
    int dim;
    float res;
    for(int i = 0; i < count ; i++){
        file.read((char*)&dim, 4);
        assert(dim == 128);
        vector<float> temp;
        for(int j = 0; j < dim; j++) {
            file.read((char *) &res, 4);
            temp.push_back(res);
        }
        ans.push_back(temp);
    }
    file.close();
    return ans;
}

vector<int> ground_truth(int Q, int count){
    ifstream file;
    vector<int> ans;
    string truth = FILE_PATH + (string)("siftsmall_groundtruth.ivecs");
    file.open(truth, ios::binary | ios::in);
    assert(file.is_open());
    int res, dim;

    file.seekg(4 * (100+1) * Q);
    file.read((char *) &dim, 4);
    assert(dim == 100);
    for(int i = 0; i < count; i++) {
        file.read((char *) &res, 4);
        //cout << res << endl;
        ans.push_back(res);
    }
    file.close();
    return ans;
}