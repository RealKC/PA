#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

using node_t = int;

static constexpr auto infinity = std::numeric_limits<node_t>::max();
static constexpr auto node_count = 6;

int pop_count(bool* population, int n)
{
    int count = 0;

    for (int i = 0; i < n; ++i) {
        count += population[i];
    }
    return count;
}

void dijkstra(int n, node_t** L, node_t i0, int D[], node_t P[])
{
    for (int i = 0; i < n; ++i) {
        P[i] = i0;
        D[i] = L[i0][i];
    }

    bool* selected = new bool[n];
    std::fill_n(selected, n, false);

    selected[i0] = true;

    while (pop_count(selected, n) < n) {
        node_t i = -1, val_min = 0;

        for (node_t k = 0; k < n; ++k) {
            if (val_min < D[k] && !selected[k]) {
                val_min = D[k];
                i = k;
            }
        }

        std::cout << "trying node " << i << std::endl;

        selected[i] = true;

        for (node_t j = 0; j < n; ++j) {
            if (L[i][j] == infinity) {
                continue;
            }

            int sum = 0;
            if (D[i] == infinity || L[i][j] == infinity) {
                sum = infinity;
            } else {
                sum = D[i] + L[i][j];
            }

            if (D[j] > sum) {
                D[j] = sum;
                P[j] = i;
            }
        }
    }
}

void print_array(int const* arr, int length, char const* info)
{
    if (info) {
        std::cout << info << ": ";
    }

    bool first = true;

    for (int i = 0; i < length; ++i) {
        if (first) {
            if (arr[i] == infinity) {
                std::cout << "∞";
            } else {
                std::cout << arr[i];
            }
            first = false;
        } else {
            if (arr[i] == infinity) {
                std::cout << ", ∞";
            } else {
                std::cout << ", " << arr[i];
            }
        }
    }

    std::cout << std::endl;
}

int main()
{
    // clang-format off
    int** l = new node_t* [node_count] {
        new node_t[node_count] {        0,        1,        2,        5, infinity, infinity }, // 0
        new node_t[node_count] { infinity,        0,        1, infinity,        3, infinity }, // 1
        new node_t[node_count] { infinity, infinity,        0,        1,        1, infinity }, // 2
        new node_t[node_count] { infinity, infinity, infinity,        0, infinity, infinity }, // 3
        new node_t[node_count] { infinity, infinity, infinity,        3,        0, infinity }, // 4
        new node_t[node_count] { infinity, infinity, infinity, infinity,        1,        0 }, // 5
    };
    // clang-format on

    int d[node_count];
    node_t p[node_count];

    dijkstra(node_count, l, 0, d, p);
    print_array(d, node_count, "lungimea drumurilor");
    print_array(p, node_count, "penultimul nod");

    return 0;
}
