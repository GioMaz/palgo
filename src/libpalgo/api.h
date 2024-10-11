#ifndef API_H
#define API_H

#include "palgo.h"

struct RecordList {
    Record *records; // MALLOC'D
    int size;
};

extern "C" {

    RecordList records_wrapper(
            int N,
            int *maxmg,
            int *minw,
            int size);

    RecordList exercises_wrapper(
            int N,
            int *maxmg, int *minw, int size,
            int mins, int maxs);

    void free_wrapper(RecordList exercises);

}

#endif
