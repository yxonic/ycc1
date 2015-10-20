// -*- mode: c++ -*-

#pragma once

#ifndef NDEBUG
#include <cstdio>
#  define ERROR(text, arg...) std::fprintf(                             \
        stderr, "\033[31m[ERROR] %s:%d: \033[0m", __FILE__, __LINE__),  \
        std::fprintf(stderr, text, ##arg),                              \
        std::fprintf(stderr, "\n")
#  define WARN(text, arg...) std::fprintf(                              \
        stderr, "\033[33m[WARN] %s:%d: \033[0m", __FILE__, __LINE__),   \
        std::fprintf(stderr, text, ##arg),                              \
        std::fprintf(stderr, "\n")
#  define INFO(text, arg...) std::fprintf(                              \
        stderr, "\033[34m[INFO] %s:%d: \033[0m", __FILE__, __LINE__),   \
        std::fprintf(stderr, text, ##arg),                              \
        std::fprintf(stderr, "\n")
#else
#  define ERROR(text, arg...)
#  define WARN(text, ...)
#  define INFO(text, ...)
#endif
