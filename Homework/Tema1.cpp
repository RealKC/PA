#include <iostream>
#include <list>
#include <queue>
#include <stdexcept>
#include <vector>

struct Digraph {
    struct Node {
        char tag;
        /// The successors are indices in the list of nodes
        std::list<int> successors;
    };

    std::vector<Node> nodes;
    std::vector<int> predecessor_counts;

    int node_count() const
    {
        return nodes.size();
    }
};

struct DigraphError : std::runtime_error {
    DigraphError(char const* msg)
        : runtime_error(msg)
    {
    }
};

std::vector<int> topological_sort(Digraph& D)
{
    // This vector contains the indices of the nodes in their topological order
    std::vector<int> L;
    std::queue<int> C;

    for (int u = 0; u < D.node_count(); ++u) {
        if (D.predecessor_counts[u] == 0) {
            C.push(u);
        }
    }

    for (int k = 0; k < D.node_count(); ++k) {
        if (C.empty()) {
            throw DigraphError("Graful conține cicluri");
        }

        int u = C.front();
        C.pop();

        L.push_back(u);

        auto successors_of_u = D.nodes[u].successors;
        while (!successors_of_u.empty()) {
            int v = successors_of_u.front();
            D.predecessor_counts[v]--;

            if (D.predecessor_counts[v] == 0) {
                C.push(v);
            }

            successors_of_u.pop_front();
        }
    }

    return L;
}

int main()
try {
    Digraph D {
        .nodes {
            { .tag = 'A', .successors { 1, 3 } },
            { .tag = 'B', .successors { 2 } },
            { .tag = 'C', .successors { 3, 4 } },
            { .tag = 'D', .successors { 4 } },
            { .tag = 'E', .successors {} },
            { .tag = 'F', .successors {} },
        },
        .predecessor_counts { 0, 1, 1, 2, 2, 0 }
    };

    auto topologically_sorted_elements = topological_sort(D);

    std::cout << "Sortarea topologică a grafului este: ";
    for (int i : topologically_sorted_elements) {
        std::cout << D.nodes[i].tag << ", ";
    }
    std::cout << std::endl;

    return 0;
} catch (DigraphError const& e) {
    std::cout << "Nu s-a putut realiza sortarea topologică: "
              << e.what() << std::endl;
    return 1;
}
