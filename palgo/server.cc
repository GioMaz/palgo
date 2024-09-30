#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>

#include "palgo.h"
#include "common.h"

// Request type
//
// +-----------+-----------------+-----
// | days: u32 | maxmg_size: u32 | ...
// +-----------+-----------------+-----
//
// -----------------+--------------+--------------+-----
// ... maxmg_0: u32 | maxmg_1: u32 | maxmg_n: u32 | ...
// -----------------+--------------+--------------+-----
//
// -------------------+-------------+-------------+-----
// ... minw_size: u32 | minw_1: u32 | minw_2: u32 | ...
// -------------------+-------------+-------------+-----
//
// ----------------+
// ... minw_n: u32 |
// ----------------+

void read_request(int fd, u32 &days,
        u32 *&maxmg, u32 &maxmg_size,
        u32 *&minw, u32 &minw_size)
{
    // Read days
    read(fd, &days, sizeof(days));

    // Read maxmg vector
    read(fd, &maxmg_size, sizeof(maxmg_size));
    maxmg = (u32 *)malloc(maxmg_size * sizeof(u32));
    for (int i = 0; i < maxmg_size; i++) {
        read(fd, &maxmg[i], sizeof(maxmg[i]));
    }

    // Read minw vector
    read(fd, &minw_size, sizeof(minw_size));
    minw = (u32 *)malloc(minw_size * sizeof(u32));
    for (int i = 0; i < minw_size; i++) {
        read(fd, &minw[i], sizeof(minw[i]));
    }
}

// Response type
//
// +-----------+----------+----------+----------+-----
// | size: u32 | g_0: u32 | m_0: u32 | s_0: u32 | ...
// +-----------+----------+----------+----------+-----
//
// -----+----------+----------+----------+------
//  ... | g_1: u32 | m_1: u32 | s_1: u32 | ...
// -----+----------+----------+----------+------
//
// -----+----------+----------+----------+
//  ... | g_n: u32 | m_n: u32 | s_n: u32 |
// -----+----------+----------+----------+

void write_response(int fd, std::vector<Record> &exercises)
{
    u32 exercises_size = exercises.size();
    write(fd, &exercises_size, sizeof(exercises_size));

    for (auto &exercise : exercises) {
        write(fd, (u32 *)&(exercise.g), sizeof(u32));
        write(fd, (u32 *)&(exercise.m), sizeof(u32));
        write(fd, (u32 *)&(exercise.s), sizeof(u32));
    }
}

void *handle_request(void *heap_fd)
{
    int fd = *((int *)heap_fd);
    printf("Initiated request handling for fd %d\n", fd);

    // Read input
    u32 days;
    u32 *maxmg, maxmg_size;
    u32 *minw, minw_size;
    read_request(fd, days,
            maxmg, maxmg_size,
            minw, minw_size);
    std::vector<int> maxmg_vec(maxmg, maxmg + maxmg_size);
    std::vector<int> minw_vec(minw, minw + minw_size);

    // Apply algorithm
    auto exercises = palgo_exercises2(days,
            maxmg_vec, minw_vec, 3, 4);

    // Write output
    write_response(fd, exercises);

    // Free memory
    free(heap_fd);
    free(maxmg);
    free(minw);

    // Close socket
    close(fd);

    printf("Terminating thread...\n");
    return NULL;
}

int main()
{
    int rv;

    int welc_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (welc_fd < 0) {
        printf("Failed socket()\n");
        exit(1);
    }
    // set_nonblocking(welc_fd);

    int opt = 1;
    setsockopt(welc_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in welc_addr;
    welc_addr.sin_family            = AF_INET;
    welc_addr.sin_addr.s_addr       = htonl(0);
    welc_addr.sin_port              = htons(8080);

    rv = bind(welc_fd, (struct sockaddr *)&welc_addr, sizeof(welc_addr));
    if (rv < 0) {
        printf("Failed bind()\n");
        exit(1);
    }

    rv = listen(welc_fd, 1000);
    if (rv < 0) {
        printf("Failed listen()\n");
        exit(1);
    }

    while (1) {
        struct sockaddr addr;
        socklen_t len;
        int fd = accept(welc_fd, &addr, &len);

        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        int *heap_fd = (int *)malloc(sizeof(int));
        *heap_fd = fd;
        pthread_create(&tid, &attr, handle_request, heap_fd);
    }
}
