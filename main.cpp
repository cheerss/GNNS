#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <utilitiy>
#include <fstream>
#include <cassert>
#include <pair>
#include "main.h"

using namespace std;

vector<vector<float> > base_vectors;
vector<vector<float> > query_vectors;
vector<vector<int> > graph;

int main(){
	vector<int> K_nearest;
	base_vectors = read_base();
	graph = build_graph(base_vectors);
	query_vectors = read_query(int count);
	search_K_nearest();
}

vector<vector<float> > read_base(){
	ifstream file;
	int dim;
	float num;
	vector<float> temp;
	vector<vector<float> > ans;
	file.open(BASE_PATH, ios::in | ios::binary);
	assert(file.is_open());

	for(int i = 0; i < BASE_NUM; i++){
		file.read((char*)&dim, 4);
		assert(dim == 128);
		for(int j = 0; j < dim; j++){
			file.read((char*)num, 4);
			temp.push_back(num);
		}
		ans.push_back(temp);
	}
	return ans;
}

vector<vector<int> > build_graph(vector<vector<float> > base_vectors){
	vector<vector<int> > ans;
	pair<int, float> dim[BASE_NUM][BASE_NUM];

	for(int i = 0; i < BASE_NUM; i++ ){
		for(int j = i; j < BASE_NUM; j++){
			
		}
	}
}