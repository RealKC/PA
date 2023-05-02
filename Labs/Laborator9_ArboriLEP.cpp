#include <algorithm>
#include <iostream>
#include <vector>

static constexpr int n = 10;

struct Node {
    int val;
    Node* left { nullptr };
    Node* right { nullptr };
};

class Heap {
public:
    Heap(int* ponderi, int n)
    {
        m_elements.reserve(n);

        for (int i = 0; i < n; ++i) {
            auto* n = new Node;
            n->val = ponderi[i];
            m_elements.push_back(n);
        }

        std::make_heap(m_elements.begin(), m_elements.end(), comparator);
    }

    Node* pop()
    {
        std::pop_heap(m_elements.begin(), m_elements.end(), comparator);
        auto* value = m_elements.back();
        m_elements.pop_back();
        return value;
    }

    void push(Node* node)
    {
        m_elements.push_back(node);
        std::push_heap(m_elements.begin(), m_elements.end());
    }

    std::size_t length()
    {
        return m_elements.size();
    }

private:
    static bool comparator(Node* left, Node* right)
    {
        return left->val > right->val;
    }

    std::vector<Node*> m_elements;
};

void random_sequence(int* arr, int n)
{
    for (int i = 0; i < n; ++i) {
        arr[i] = rand() % 128;
    }
}

Node* lep(int* ponderi, int n)
{
    Heap B { ponderi, n };

    while (B.length() > 1) {
        auto* t1 = B.pop();
        auto* t2 = B.pop();

        auto* t = new Node;
        t->left = t1;
        t->right = t2;
        t->val = t1->val + t2->val;

        B.push(t);
    }

    return B.pop();
}

int compute_lep(Node* tree, int level = 0)
{
    if (tree == nullptr) {
        return 0;
    }

    if (tree->left || tree->right) {
        return compute_lep(tree->left, level + 1) + compute_lep(tree->right, level + 1);
    }

    return level * tree->val;
}

void print_tree(Node* tree, int level = 0)
{
    if (tree == nullptr) {
        return;
    }

    for (int i = 0; i < level; ++i) {
        std::cout << "    ";
    }
    std::cout << tree->val << std::endl;
    print_tree(tree->left, level + 1);
    print_tree(tree->right, level + 1);
}

int main()
{
    int ponderi[] = { 10, 20, 50, 30, 70 };
    // random_sequence(ponderi, std::size(ponderi));

    auto* weighted_tree = lep(ponderi, std::size(ponderi));
    print_tree(weighted_tree);

    std::cout << "LEP(t) value is " << compute_lep(weighted_tree) << std::endl;

    return 0;
}
