#pragma once

#ifdef DEBUG
#  define ERROR(text, arg...) fprintf(stderr, "[ERROR] "),      \
        fprintf(stderr, text, ##arg),                           \
        fprintf(stderr, "\n")
#  define WARN(text, arg...) fprintf(stderr, "[WARN] "),        \
        fprintf(stderr, text, ##arg),                           \
        fprintf(stderr, "\n")
#  define INFO(text, arg...) fprintf(stderr, "[INFO] "),        \
        fprintf(stderr, text, ##arg),                           \
        fprintf(stderr, "\n")
#else
#  define ERROR(text, arg...) fprintf(stderr, text, ##arg),     \
        fprintf(stderr, "\n")
#  define WARN(text, ...)
#  define INFO(text, ...)
#endif
