#ifndef MAIN_H
#define MAIN_H

#include <vector>

#define BASE_PATH "./siftsmall/siftsmall_base.fvecs"
#define TRUTH_PATH "./siftsmall/siftsmall_groundtruth.ivecs"
#define QUERY_PATH "./siftsmall/siftsmall_query.fvecs"

#define BASE_NUM 10000

vector<vector<float> > read_base();
vector<vector<int> > build_graph(vector<vector<float> >);
vector<vector<float> > read_query(int count);
vector<int> search_K_nearest();

#endif