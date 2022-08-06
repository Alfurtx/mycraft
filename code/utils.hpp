#if !defined(UTILS_HPP)
#define UTILS_HPP

#include <cstdio>

// NOTE(fonsi): custom assert implementation
#define NDEBUG
#include <cassert>
#undef assert
#define assert(E, M) (void)((E) || (_assert(#E, __FILE__, __LINE__, M),0))
inline void
_assert(const char* expr, const char* filename, const int line, char* message)
{
    if(!message) {
        fprintf(stderr, "[ERROR][%s:%d] %s\n", filename, line, expr);
    } else {
        fprintf(stderr, "[ERROR][%s:%d] %s\n", filename, line, message);
    }
}

#endif
