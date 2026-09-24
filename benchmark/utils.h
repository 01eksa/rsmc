#ifndef RSMC_UTILS_H
#define RSMC_UTILS_H
#include <stdint.h>

#if __has_include(<unistd.h>)
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#ifndef _POSIX_MONOTONIC_CLOCK
#error "POSIX monotonic clock is not supported!"
#endif

static double sec_now(void)
{
    struct timespec ts;
    if (!clock_gettime(CLOCK_MONOTONIC, &ts))
        return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
    perror("clock_gettime");
    exit(1);
}

#elif defined(_WIN32)

#include <Windows.h>
#include <sysinfoapi.h>
static double sec_now(void)
{
    uint32_t ms = GetTickCount();
    return (double)ms / 1000.0;
}

#endif

#if defined(_MSC_VER)
#include <intrin.h>
#define popcnt64(x) __popcnt64(x)
#elif defined(__GNUC__) || defined(__clang__)
#define popcnt64(x) __builtin_popcountll(x)
#else
static inline int popcnt64(uint64_t x)
{
    x = x - ((x >> 1) & 0x5555555555555555ULL);
    x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
    x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
    return (x * 0x0101010101010101ULL) >> 56;
}
#endif

static uint32_t state = ('r' << 24) + ('s' << 16) + ('m' << 8) + 'c';

static uint32_t xorshift32()
{
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

#endif // RSMC_UTILS_H
