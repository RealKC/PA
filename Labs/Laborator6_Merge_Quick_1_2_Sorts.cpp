#include <chrono>
#include <iostream>
#include <math.h>
#include <stdlib.h>

// Results from my run:
//      Merge sort (ascending sequence) -> duration = 13489µs (13.489ms)
//      Quick sort #1 (ascending sequence) -> duration = 22191643µs (22191.6ms)
//      Quick sort #2 (ascending sequence) -> duration = 22517232µs (22517.2ms)
//      Merge sort (descending sequence) -> duration = 12631µs (12.631ms)
//      Quick sort #1 (descending sequence) -> duration = 21294635µs (21294.6ms)
//      Quick sort #2 (descending sequence) -> duration = 20130477µs (20130.5ms)
//      Merge sort (random sequence) -> duration = 26306µs (26.306ms)
//      Quick sort #1 (random sequence) -> duration = 25217µs (25.217ms)
//      Quick sort #2 (random sequence) -> duration = 23145µs (23.145ms)

static constexpr int ELEMENT_COUNT = 100'000;

void interclasare(int* a, int p, int q, int m, int* temp)
{
    auto i = p;
    auto j = m + 1;
    int k = -1;

    while ((i <= m) and (j <= q)) {
        ++k;
        if (a[i] <= a[j]) {
            temp[k] = a[i];
            ++i;
        } else {
            temp[k] = a[j];
            ++j;
        }
    }

    while (i <= m) {
        ++k;
        temp[k] = a[i];
        ++i;
    }

    while (j <= q) {
        ++k;
        temp[k] = a[j];
        ++j;
    }
}

void merge_sort(int* a, int p, int q)
{
    if (p < q) {
        int m = (p + q) / 2;

        merge_sort(a, p, m);
        merge_sort(a, m + 1, q);

        auto* temp = new int[q - p + 1];
        interclasare(a, p, q, m, temp);

        for (int i = p; i <= q; ++i) {
            a[i] = temp[i - p];
        }

        delete temp;
    }
}

int partition(int* a, int p, int q)
{
    auto x = a[p];
    auto i = p + 1;
    auto j = q;

    while (i <= j) {
        if (a[i] <= x) {
            ++i;
        }

        if (a[j] >= x) {
            --j;
        }

        if (i < j) {
            if (a[i] > x and x > a[j]) {
                std::swap(a[i], a[j]);
                ++i;
                --j;
            }
        }
    }

    auto pivot = i - 1;
    a[p] = a[pivot];
    a[pivot] = x;
    return pivot;
}

void quick_sort1(int* a, int p, int q)
{
    if (p < q) {
        auto pivot = partition(a, p, q);
        quick_sort1(a, p, pivot - 1);
        quick_sort1(a, pivot + 1, q);
    }
}

void quick_sort2(int* a, int p, int q)
{
    while (p < q) {
        auto pivot = partition(a, p, q);
        if (pivot - p > q - pivot) {
            quick_sort2(a, pivot + 1, q);
            q = pivot - 1;
        } else {
            quick_sort2(a, p, pivot - 1);
            p = pivot + 1;
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

void ascending_sequence(int* arr, int n)
{
    for (int i = 0; i < n; ++i) {
        arr[i] = i;
    }
}

void descending_sequence(int* arr, int n)
{
    for (int i = 0; i < n; ++i) {
        arr[i] = n - i;
    }
}

void random_sequence(int* arr, int n)
{
    for (int i = 0; i < n; ++i) {
        arr[i] = rand();
    }
}

int main()
{
    auto* v = new int[ELEMENT_COUNT];

    ascending_sequence(v, ELEMENT_COUNT);
    benchmark("Merge sort (ascending sequence)", [&] { merge_sort(v, 0, ELEMENT_COUNT - 1); });
    benchmark("Quick sort #1 (ascending sequence)", [&] { quick_sort1(v, 0, ELEMENT_COUNT - 1); });
    benchmark("Quick sort #2 (ascending sequence)", [&] { quick_sort2(v, 0, ELEMENT_COUNT - 1); });

    descending_sequence(v, ELEMENT_COUNT);
    benchmark("Merge sort (descending sequence)", [&] { merge_sort(v, 0, ELEMENT_COUNT - 1); });

    descending_sequence(v, ELEMENT_COUNT);
    benchmark("Quick sort #1 (descending sequence)", [&] { quick_sort1(v, 0, ELEMENT_COUNT - 1); });

    descending_sequence(v, ELEMENT_COUNT);
    benchmark("Quick sort #2 (descending sequence)", [&] { quick_sort2(v, 0, ELEMENT_COUNT - 1); });

    random_sequence(v, ELEMENT_COUNT);
    benchmark("Merge sort (random sequence)", [&] { merge_sort(v, 0, ELEMENT_COUNT - 1); });

    random_sequence(v, ELEMENT_COUNT);
    benchmark("Quick sort #1 (random sequence)", [&] { quick_sort1(v, 0, ELEMENT_COUNT - 1); });

    random_sequence(v, ELEMENT_COUNT);
    benchmark("Quick sort #2 (random sequence)", [&] { quick_sort2(v, 0, ELEMENT_COUNT - 1); });

    return 0;
}
