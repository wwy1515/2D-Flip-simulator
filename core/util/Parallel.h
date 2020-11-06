#pragma once
#include <omp.h>
#include <functional>
#include <vector>

inline void parallelFor(int start, int end, const std::function<void(int)>& func) {
    if (start > end)
    {
        return;
    }
    #pragma omp parallel for
    for (int i = start; i < end; ++i) {
        func(i);
    }

}
