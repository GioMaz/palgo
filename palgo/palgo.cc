#include <iostream>
#include <vector>
#include <queue>
#include <assert.h>

#include "palgo.h"

bool bfs(std::vector<std::vector<int>> &g, int source, int sink, std::vector<int> &parent)
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
int edmonds_karp(std::vector<std::vector<int>> &g, int source, int sink)
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

bool compare_record(Record r1, Record r2)
{
    return r1.g < r2.g;
}

std::vector<Record> palgo_maxg(int N, int M, std::vector<int> &maxmg, std::vector<int> &minw, int maxg)
{
    assert(maxmg.size() == M);
    assert(minw.size() == M);
    int sets = 0;
    for (int i = 0; i < N; i++) {
        sets += minw[i];
    }
    assert(sets <= maxg * N);

    int n       = N + M + 2;
    int source  = 0;
    int sink    = n-1;
    std::vector<std::vector<int>> g(n, std::vector<int>(n, 0));

    for (int i = 0; i < M; i++) {
        g[0][1+i] = minw[i];
    }

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            g[1+i][1+M+j] = maxmg[i];
        }
    }

    for (int j = 0; j < N; j++) {
        g[1+M+j][n-1] = maxg;
    }

    int max_flow = edmonds_karp(g, 0, n-1);

    std::vector<Record> records;

    // Check if each muscle has at least minw exercises (constraint 3)
    for (int i = 0; i < M; i++) {
        if (g[1+i][0] < minw[i]) {
            return records;
        }
    }

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

std::vector<Record> palgo(int N, int M, std::vector<int> &maxmg, std::vector<int> &minw)
{
    int maxg = 0;
    for (int i = 0; i < minw.size(); i++) {
        maxg += minw[i];
    }
    maxg = ceil((float)maxg / (float)N);
    return palgo_maxg(N, M, maxmg, minw, maxg);
}

int main()
{
    // Dati initziali
    int N = 3;
    int M = 6;
    //                           Petto,  Schiena,    Spalle, Gambe   Bicipiti,   Tricipiti
    std::vector<int> maxmg   = { 12,     30,         8,      12,     12,         8 };
    std::vector<int> minw    = { 16,     24,         24,     12,     24,         12 };

    auto records = palgo(N, M, maxmg, minw);
    if (records.size() == 0) {
        std::cout << "The input data is not in a correct format\n";
    }
    for (auto &record: records) {
        std::cout << "(giorno " << record.g
            << ", muscolo " << record.m
            << ", serie " << record.s << ")\n";
    }

    return 0;
}
