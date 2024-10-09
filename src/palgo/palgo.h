#ifndef PALGO_H
#define PALGO_H

#include <vector>

struct Record {
    int g, m, s;
};

std::vector<Record> palgo_records(
        int N,
        std::vector<int> &maxmg,
        std::vector<int> &minw);

std::vector<Record> palgo_exercises(
        int N,
        std::vector<int> &maxmg,
        std::vector<int> &minw,
        int mins = 3,
        int maxs = 4);

struct RecordList {
    Record *records;
    int size;
};

extern "C" {
    RecordList exercises_wrapper(
            int N,
            int *maxmg, int *minw, int size,
            int mins, int maxs);

    void free_wrapper(RecordList exercises);
}

#endif
