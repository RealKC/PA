#include <iostream>
#include <limits>
#include <stack>
#include <stdexcept>
#include <vector>

static constexpr auto infinity = std::numeric_limits<float>::infinity();
static constexpr auto node_count = 4;

class InvalidGraph : public std::runtime_error {
public:
    InvalidGraph()
        : std::runtime_error("(di)graful are circuite negative")
    {
    }
};

struct Graph {
    float** L;
    std::size_t n;
};

// 1. Scrieti o functie C/C++ care implementeaza algoritmul Floyd-Warshall
void floyd_warshall(Graph G, int** P)
{
    for (int i = 0; i < G.n; ++i) {
        for (int j = 0; j < G.n; ++j) {
            if ((i != j) and (G.L[i][j] != infinity)) {
                P[i][j] = i;
            } else {
                P[i][j] = -1;
            }
        }
    }

    for (int k = 0; k < G.n; ++k) {
        for (int i = 0; i < G.n; ++i) {
            for (int j = 0; j < G.n; ++j) {
                float sum;
                if ((G.L[i][k] == infinity) or (G.L[k][j] == infinity)) {
                    sum = infinity;
                } else {
                    sum = G.L[i][k] + G.L[k][j];
                }

                if (sum < G.L[i][j]) {
                    G.L[i][j] = sum;
                    P[i][j] = P[k][j];
                }

                if ((i == j) and (G.L[i][j] < 0)) {
                    throw InvalidGraph();
                }
            }
        }
    }
}

// 2. Dat fiind un graf G = (V, E), scrieti un program care sa afiseze drumurile
//    minime intre oricare doua varfuri
void show_minimum_road(int start, int end, int** P)
{
    int a = end;

    std::stack<int> stack;

    while (a != -1 and a != start) {
        stack.push(a);
        a = P[start][a];
    }

    std::cout << start;
    while (!stack.empty()) {
        std::cout << ", " << stack.top();
        stack.pop();
    }

    std::cout << std::endl;
}

int main()
{
    Graph G {
        // clang-format off
        .L = new float*[node_count] {
            new float[node_count] {        0,        1,        2, infinity }, // 1
            new float[node_count] { infinity,        0,       -1,        1 }, // 2
            new float[node_count] { infinity, infinity,        0,      0.5 }, // 3
            new float[node_count] { infinity, infinity, infinity,        0 }, // 4
        },
        // clang-format on
        .n = node_count,
    };

    auto** P = new int*[node_count];

    for (int i = 0; i < node_count; ++i) {
        P[i] = new int[node_count];
    }

    try {
        floyd_warshall(G, P);
        show_minimum_road(0, 3, P);
    } catch (InvalidGraph const& ex) {
        std::cout << ex.what() << std::endl;
    }

    return 0;
}
