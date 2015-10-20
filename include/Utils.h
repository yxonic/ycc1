// -*- mode: c++ -*-

#pragma once

#ifndef NDEBUG
#include <cstdio>
#  define ERROR(text, arg...) std::fprintf(stderr, "\x1b[31m[ERROR] "), \
        std::fprintf(stderr, text, ##arg),                              \
        std::fprintf(stderr, "\033[0m\n")
#  define WARN(text, arg...) std::fprintf(stderr, "\x1b[33m[WARN] "),   \
        std::fprintf(stderr, text, ##arg),                              \
        std::fprintf(stderr, "\033[0m\n")
#  define INFO(text, arg...) std::fprintf(stderr, "\x1b[34m[INFO] "),   \
        std::fprintf(stderr, text, ##arg),                              \
        std::fprintf(stderr, "\033[0m\n")
#else
#  define ERROR(text, arg...)
#  define WARN(text, ...)
#  define INFO(text, ...)
#endif
