#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "palgo.h"
#include "common.h"

void write_request(int fd, int days, std::vector<int> maxmg, std::vector<int> minw)
{
    // Write days
    write(fd, (u32 *)&days, sizeof(u32));

    // Write maxmg vector
    u32 maxmg_size = maxmg.size();
    write(fd, &maxmg_size, sizeof(maxmg_size));
    for (int elem : maxmg) {
        write(fd, (u32 *)&elem, sizeof(u32));
    }

    // Write minw vector
    u32 minw_size = minw.size();
    write(fd, &minw_size, sizeof(minw_size));
    for (int elem : minw) {
        write(fd, (u32 *)&elem, sizeof(u32));
    }
}

std::vector<Record> read_response(int fd)
{
    u32 size;
    read(fd, &size, sizeof(u32));

    std::vector<Record> exercises;
    for (int i = 0; i < size; i++) {
        Record record;
        read(fd, &record.g, sizeof(u32));
        read(fd, &record.m, sizeof(u32));
        read(fd, &record.s, sizeof(u32));
        exercises.push_back(record);
    }

    return exercises;
}

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in welc_addr;
    welc_addr.sin_family            = AF_INET;
    welc_addr.sin_addr.s_addr       = ntohl(INADDR_LOOPBACK);
    welc_addr.sin_port              = ntohs(8080);

    int rv = connect(fd, (struct sockaddr *)&welc_addr, sizeof(welc_addr));
    if (rv) {
        fprintf(stderr, "Failed to connect to server\n");
        exit(0);
    }

    int N = 3;
    std::vector<int> maxmg  = { 11, 30, 8,  20, 10, 8 };
    std::vector<int> minw   = { 22, 20, 20, 20, 23, 16 };
    write_request(fd, N, maxmg, minw);
    auto exercises = read_response(fd);

    for (auto &exercise : exercises) {
        std::cout
            << "(g"     << exercise.g
            << ", m"    << exercise.m
            << ", s"    << exercise.s << ")\n";
    }

    return 0;
}
