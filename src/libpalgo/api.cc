#include <vector>
#include <cstdlib>

#include "api.h"
#include "palgo.h"

static RecordList to_record_list(std::vector<Record> records_vec)
{
    int records_size = (int)records_vec.size();

    Record *records = (Record *)malloc(records_size * sizeof(Record));
    std::copy(records_vec.begin(), records_vec.end(), records);

    return (RecordList) {
        .records = records,
        .size    = records_size,
    };
}

RecordList records_wrapper(int N, int *maxmg, int *minw, int size)
{
    std::vector<int> maxmg_vec(maxmg, &maxmg[size]);
    std::vector<int> minw_vec(minw, &minw[size]);
    auto records_vec = palgo_records(N, maxmg_vec, minw_vec);
    return to_record_list(records_vec);
}

RecordList exercises_wrapper(int N, int *maxmg, int *minw, int size, int mins, int maxs)
{
    std::vector<int> maxmg_vec(maxmg, &maxmg[size]);
    std::vector<int> minw_vec(minw, &minw[size]);
    auto exercises_vec = palgo_exercises(N, maxmg_vec, minw_vec, mins, maxs);
    return to_record_list(exercises_vec);
}

void free_wrapper(RecordList exercises)
{
    free(exercises.records);
}
