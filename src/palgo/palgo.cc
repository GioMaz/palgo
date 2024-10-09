#include <iostream>
#include <vector>
#include <queue>
#include <assert.h>

#include "palgo.h"

static bool bfs(std::vector<std::vector<int>> &g, int source, int sink, std::vector<int> &parent)
{
    int n = g.size();
    std::vector<bool> visited(n, false);
    std::queue<int> q;

    q.push(source);
    visited[source] = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < n; v++) {
            if (!visited[v] && g[u][v]) {
                q.push(v);
                visited[v] = true;
                parent[v] = u;
            }
        }
    }

    return visited[sink];
}

// Ford-Fulkerson implementation O((N+M)^2 * |f*|)
static int edmonds_karp(std::vector<std::vector<int>> &g, int source, int sink)
{
    int n           = g.size();
    auto parent     = std::vector<int>(n, -1);
    int max_flow    = 0;

    while (bfs(g, source, sink, parent)) {
        int path_flow = INT_MAX;
        int s = sink;
        while (s != source) {
            path_flow = std::min(path_flow, g[parent[s]][s]);
            s = parent[s];
        }

        max_flow += path_flow;

        int v = sink;
        while (v != source) {
            int u    = parent[v];
            g[u][v] -= path_flow;
            g[v][u] += path_flow;
            v        = parent[v];
        }
    }

    return max_flow;
}

static bool compare_record(Record r1, Record r2)
{
    return r1.g < r2.g;
}

std::vector<Record> palgo_records(int N, std::vector<int> &maxmg, std::vector<int> &minw)
{
    assert(maxmg.size() == minw.size());

    // Distribute series equally across days
    int maxg = 0;
    for (int i = 0; i < minw.size(); i++) {
        maxg += minw[i];
    }
    maxg = ceil((float)maxg / (float)N);

    int M = maxmg.size();

    int sets = 0;
    for (int i = 0; i < N; i++) {
        sets += minw[i];
    }
    assert(sets <= maxg * N);

    int n       = N + M + 2;
    std::vector<std::vector<int>> g(n, std::vector<int>(n, 0));

    // from S to every m_i
    for (int i = 0; i < M; i++) {
        g[0][1+i] = minw[i];
    }

    // From every m_i to every g_j
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            g[1+i][1+M+j] = maxmg[i];
        }
    }

    // From every g_j to T
    for (int j = 0; j < N; j++) {
        g[1+M+j][n-1] = maxg;
    }

    edmonds_karp(g, 0, n-1);

    std::vector<Record> records;

    // Check if each muscle has at least minw exercises (constraint 3)
    for (int i = 0; i < M; i++) {
        if (g[1+i][0] < minw[i]) {
            return records;
        }
    }

    // Get all the records with at least 1 serie
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            int serie = g[1+M+j][1+i];
            if (serie) {
                records.push_back((Record) {
                    .g = j,
                    .m = i,
                    .s = serie
                });
            }
        }
    }

    std::sort(records.begin(), records.end(), compare_record);

    return records;
}

static void flatten(std::vector<Record> &exercises)
{
    for (int i = 0; i < exercises.size(); i++) {
        for (int j = 0; j < exercises.size(); j++) {
            if (i != j) {
                float mean = ((float)(exercises[i].s + exercises[j].s)) / 2.0f;
                exercises[i].s = floor(mean);
                exercises[j].s = ceil(mean);
            }
        }
    }
}

static std::vector<Record> subdivide(Record record, int mins, int maxs)
{
    int nexer   = ceil(((float)record.s) / ((float)maxs));
    int total   = maxs * nexer;

    std::vector<Record> exercises(nexer, (Record) {
        .g = record.g,
        .m = record.m,
        .s = maxs,
    });

    int i = 0;
    while (total > record.s && i < nexer) {
        while (total > record.s && exercises[i].s > mins) {
            exercises[i].s--;
            total--;
        }
        i++;
    }

    if (total != record.s) {
        return { record };
    } else {
        return exercises;
    }
}

std::vector<Record> palgo_exercises(int N, std::vector<int> &maxmg, std::vector<int> &minw, int mins, int maxs)
{
    assert(maxmg.size() == minw.size());
    assert(mins <= maxs);
    assert(mins > 0);

    auto records = palgo_records(N, maxmg, minw);
    std::vector<Record> exercises;

    for (auto &record : records) {
        auto record_exercises = subdivide(record, mins, maxs);
        flatten(record_exercises);
        exercises.insert(exercises.end(),
                record_exercises.begin(),
                record_exercises.end());
    }

    return exercises;
}

RecordList exercises_wrapper(int N, int *maxmg, int *minw, int size,
                             int mins, int maxs)
{
    std::vector<int> maxmg_vec(maxmg, &maxmg[size]);
    std::vector<int> minw_vec(minw, &minw[size]);

    auto exercises_vec = palgo_exercises(N, maxmg_vec, minw_vec, mins, maxs);
    int exercises_size = (int)exercises_vec.size();

    Record *exercises = (Record *)malloc(exercises_size * sizeof(Record));
    std::copy(exercises_vec.begin(), exercises_vec.end(), exercises);

    return (RecordList) {
        .records = exercises,
        .size    = exercises_size,
    };
}

void free_wrapper(RecordList exercises)
{
    free(exercises.records);
}

// int main()
// {
//     //                          Dati initziali
//     //                          Petto,  Schiena,    Spalle, Gambe   Bicipiti,   Tricipiti
//     int N = 3;
//     std::vector<int> maxmg  = { 11,     30,         8,      20,     10,         8 };
//     std::vector<int> minw   = { 22,     20,         20,     20,     23,         16 };
//
//     auto records = palgo_exercises2(N, maxmg, minw, 3, 4);
//     if (records.size() == 0) {
//         std::cout << "The input data is not in a correct format\n";
//     }
//     for (auto &record: records) {
//         std::cout
//             << "(g"     << record.g
//             << ", m"    << record.m
//             << ", s"    << record.s << ")\n";
//     }
//
//     return 0;
// }
