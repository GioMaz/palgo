#ifndef PALGO_H
#define PALGO_H

#include <vector>

struct Record {
    int g, m, s;
};

std::vector<Record> palgo(int N, std::vector<int> &maxmg, std::vector<int> &minw);
std::vector<Record> palgo_exercises1(int N, std::vector<int> &maxg, std::vector<int> &minw, int mins, int maxs);
std::vector<Record> palgo_exercises2(int N, std::vector<int> &maxg, std::vector<int> &minw, int mins, int maxs);

#endif
