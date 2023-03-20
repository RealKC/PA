#include <chrono>
#include <iostream>
#include <math.h>
#include <stdlib.h>

// Results from my run:
//      Insertion sort (array sorted ascending) -> duration = 319µs (0.319ms)
//      Shell sort (array sorted ascending) -> duration = 4785µs (4.785ms)
//      Insertion sort (array sorted descending) -> duration = 28051622µs (28051.6ms)
//      Shell sort (array sorted descending) -> duration = 6854µs (6.854ms)
//      Insertion sort (random array) -> duration = 12548862µs (12548.9ms)
//      Shell sort (random array) -> duration = 27339µs (27.339ms)

static constexpr int ELEMENT_COUNT = 100'000;

void insertion_sort(int* a, int n)
{
    for (int k = 0; k < n; ++k) {
        auto i = k - 1;
        auto temp = a[k];

        while (i >= 0 and temp < a[i]) {
            a[i + 1] = a[i];
            --i;
        }

        if (i != k - 1) {
            a[i + 1] = temp;
        }
    }
}

void shell_sort(int* a, int n)
{
    for (int s = log2(n) - 2; s >= 0; --s) {
        int h = pow(2, s + 1) - 1;
        for (int i = h; i <= n - 1; ++i) {
            auto temp = a[i];
            auto j = i - h;
            while (j >= 0 and temp < a[j]) {
                a[j + h] = a[j];
                j -= h;
            }

            if (j + h != i) {
                a[j + h] = temp;
            }
        }
    }
}

template<typename Callback>
void benchmark(char const* description, Callback callback)
{
    using namespace std::chrono;

    auto start = high_resolution_clock::now();
    callback();
    auto end = high_resolution_clock::now();

    auto duration = end - start;

    auto us = duration_cast<microseconds>(duration);

    float const ms_fractional = static_cast<float>(us.count()) / 1000;
    std::cout << description << " -> duration = " << us.count() << "µs (" << ms_fractional << "ms)" << std::endl;
}

int main()
{
    auto* v = new int[ELEMENT_COUNT];
    for (int i = 0; i < ELEMENT_COUNT; ++i) {
        v[i] = i;
    }

    benchmark("Insertion sort (array sorted ascending)", [&] { insertion_sort(v, ELEMENT_COUNT); });
    benchmark("Shell sort (array sorted ascending)", [&] { shell_sort(v, ELEMENT_COUNT); });

    for (int i = 0; i < ELEMENT_COUNT; ++i) {
        v[i] = ELEMENT_COUNT - i;
    }

    benchmark("Insertion sort (array sorted descending)", [&] { insertion_sort(v, ELEMENT_COUNT); });

    for (int i = 0; i < ELEMENT_COUNT; ++i) {
        v[i] = ELEMENT_COUNT - i;
    }

    benchmark("Shell sort (array sorted descending)", [&] { shell_sort(v, ELEMENT_COUNT); });

    for (int i = 0; i < ELEMENT_COUNT; ++i) {
        v[i] = rand();
    }

    benchmark("Insertion sort (random array)", [&] { insertion_sort(v, ELEMENT_COUNT); });

    for (int i = 0; i < ELEMENT_COUNT; ++i) {
        v[i] = rand();
    }

    benchmark("Shell sort (random array)", [&] { shell_sort(v, ELEMENT_COUNT); });

    return 0;
}
