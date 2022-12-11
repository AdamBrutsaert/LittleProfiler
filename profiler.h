/*
** EPITECH PROJECT, 2022
** profiler.h
** File description:
** profiler.h
*/

#pragma once

#define _POSIX_C_SOURCE 200809L
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    char const *name;
    uint64_t begin;
} __profiler_t;

static __profiler_t *__PROFILERS = NULL;
static uint64_t __PROFILER_CAPACITY = 0;
static uint64_t __PROFILER_LENGTH = 0;
static uint64_t __PROFILER_ERRORS = 0;

static inline void profiler_push(char const *name, char const *file,
    uint64_t line)
{
    struct timespec spec;
    __profiler_t *tmp;

    if (__PROFILER_LENGTH >= __PROFILER_CAPACITY) {
        tmp = realloc(__PROFILERS, (__PROFILER_CAPACITY * 2
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
    __PROFILERS[__PROFILER_LENGTH].begin = spec.tv_nsec;
    __PROFILER_LENGTH++;
}

static inline void profiler_pop(void)
{
    struct timespec spec;
    uint64_t elapsed;

    clock_gettime(CLOCK_REALTIME, &spec);
    if (__PROFILER_ERRORS) {
        __PROFILER_ERRORS--;
        return;
    }
    if (!__PROFILER_LENGTH)
        return;
    __PROFILER_LENGTH--;
    elapsed = spec.tv_nsec - __PROFILERS[__PROFILER_LENGTH].begin;
    fprintf(stderr, "[%s] Took %gs, %gms (%luns)\n",
        __PROFILERS[__PROFILER_LENGTH].name, elapsed / 1000000000.0f,
            elapsed / 1000000.0f, elapsed);
    if (!__PROFILER_LENGTH)
        free(__PROFILERS);
}

#define PROFILE_BEGIN(name) profiler_push(name, __FILE__, __LINE__)
#define PROFILE_FUNC() PROFILE_BEGIN(__func__)
#define PROFILE_END() profiler_pop()
