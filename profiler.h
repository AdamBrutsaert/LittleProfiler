/*
** EPITECH PROJECT, 2022
** profiler.h
** File description:
** profiler.h
*/

#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#else
    #define _POSIX_C_SOURCE 200809L
#endif

#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    char const *name;
    char const *file;
    uint64_t line;
    int64_t ns;
    int64_t s;
} __profiler_t;

static __profiler_t *__PROFILERS = NULL;
static uint64_t __PROFILER_CAPACITY = 0;
static uint64_t __PROFILER_LENGTH = 0;
static uint64_t __PROFILER_ERRORS = 0;

#if defined(_WIN32) || defined(_WIN64)

static BOOL g_first_time = 1;
static LARGE_INTEGER g_counts_per_sec;

static inline int clock_gettime(int, struct timespec *ct)
{
    LARGE_INTEGER count;

    if (g_first_time)
    {
        g_first_time = 0;

        if (0 == QueryPerformanceFrequency(&g_counts_per_sec))
        {
            g_counts_per_sec.QuadPart = 0;
        }
    }

    if ((NULL == ct) || (g_counts_per_sec.QuadPart <= 0) ||
            (0 == QueryPerformanceCounter(&count)))
    {
        return -1;
    }

    ct->tv_sec = count.QuadPart / g_counts_per_sec.QuadPart;
    ct->tv_nsec = ((count.QuadPart % g_counts_per_sec.QuadPart) * BILLION) / g_counts_per_sec.QuadPart;

    return 0;
}

#endif

static inline void profiler_push(char const *name, char const *file,
    uint64_t line)
{
    struct timespec spec;
    __profiler_t *tmp;

    if (__PROFILER_LENGTH >= __PROFILER_CAPACITY) {
        tmp = (__profiler_t *)realloc(__PROFILERS, (__PROFILER_CAPACITY * 2
            + !__PROFILER_CAPACITY) * sizeof(__profiler_t));
        if (!tmp) {
            fputs("Error: how is it possible? Couldn't reallocate!\n", stderr);
            __PROFILER_ERRORS++;
            return;
        }
        __PROFILERS = tmp;
        __PROFILER_CAPACITY = __PROFILER_CAPACITY * 2 + !__PROFILER_CAPACITY;
    }
    clock_gettime(CLOCK_REALTIME, &spec);
    __PROFILERS[__PROFILER_LENGTH].name = name;
    __PROFILERS[__PROFILER_LENGTH].file = file;
    __PROFILERS[__PROFILER_LENGTH].line = line;
    __PROFILERS[__PROFILER_LENGTH].ns = spec.tv_nsec;
    __PROFILERS[__PROFILER_LENGTH].s = spec.tv_sec;
    __PROFILER_LENGTH++;
}

static inline void profiler_pop(void)
{
    struct timespec spec;
    int64_t elapsed;

    clock_gettime(CLOCK_REALTIME, &spec);
    if (__PROFILER_ERRORS) {
        __PROFILER_ERRORS--;
        return;
    }
    if (!__PROFILER_LENGTH)
        return;
    __PROFILER_LENGTH--;
    elapsed = spec.tv_sec * 1000000000 + spec.tv_nsec
        - (__PROFILERS[__PROFILER_LENGTH].s * 1000000000
            + __PROFILERS[__PROFILER_LENGTH].ns);
    fprintf(stderr, "[%s:%lu] %s: Took %gs, %gms (%ldns)\n",
        __PROFILERS[__PROFILER_LENGTH].file,
        __PROFILERS[__PROFILER_LENGTH].line,
        __PROFILERS[__PROFILER_LENGTH].name, elapsed / 1000000000.0f,
            elapsed / 1000000.0f, elapsed);
    if (!__PROFILER_LENGTH)
        free((void *)__PROFILERS);
}

#define PROFILE_BEGIN(name) profiler_push(name, __FILE__, __LINE__)
#define PROFILE_FUNC() PROFILE_BEGIN(__func__)
#define PROFILE_END() profiler_pop()
