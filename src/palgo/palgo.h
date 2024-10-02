#ifndef PALGO_H
#define PALGO_H

#include <iostream>
#include <vector>

bool bfs(std::vector<std::vector<int>> &g, int source, int sink, std::vector<int> &parent);
int edmonds_karp(std::vector<std::vector<int>> &g, int source, int sink);

struct Record {
    int g, m, s;
};

bool compare_record(Record r1, Record r2);

std::vector<Record> palgo_maxg(int N, std::vector<int> &maxmg, std::vector<int> &minw, int maxg);
std::vector<Record> palgo(int N, std::vector<int> &maxmg, std::vector<int> &minw);
std::vector<Record> palgo_exercises1(int N, std::vector<int> &maxg, std::vector<int> &minw, int mins, int maxs);
std::vector<Record> palgo_exercises2(int N, std::vector<int> &maxg, std::vector<int> &minw, int mins, int maxs);

#endif
