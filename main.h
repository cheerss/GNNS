//
// Created by Wangwenxiao on 12/03/2017.
//

#ifndef GNNS_MAIN_H
#define GNNS_MAIN_H

#include <vector>
using namespace std;

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

#endif //GNNS_MAIN_H
