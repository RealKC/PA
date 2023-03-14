#include <iostream>
#include <string_view>

class Trie {
public:
    Trie()
        : m_tree(new Node)
    {
    }

    ~Trie()
    {
        free_tree(m_tree);
    }

    void insert(std::string_view s) { insert_in_trie(m_tree, s); }

    bool contains(std::string_view s)
    {
        auto* node = search_in_trie(m_tree, s);

        if (node == nullptr) {
            return false;
        }

        return node->ends_word;
    }

private:
    static constexpr size_t letter_to_index(char ch) { return ch - 'a'; }

    struct Node {
        bool ends_word { false };
        Node* succ[26] { nullptr };
    };

    static Node* search_in_trie(Node* tree, std::string_view s)
    {
        size_t i = 0;
        auto* p = tree;

        while (p != nullptr and i < s.length()) {
            p = p->succ[letter_to_index(s[i])];
            ++i;
        }

        return p;
    }

    static void insert_in_trie(Node* tree, std::string_view s)
    {
        size_t i = 0;
        auto* p = tree;

        while (i < s.length()) {
            auto idx = letter_to_index(s[i]);
            if (p->succ[idx] == nullptr) {
                p->succ[idx] = new Node;
            }
            p = p->succ[idx];
            ++i;
        }

        p->ends_word = true;
    }

    static void free_tree(Node* tree)
    {
        if (tree == nullptr) {
            return;
        }

        for (size_t i = 0; i < 26; ++i) {
            if (tree->succ[i]) {
                free_tree(tree->succ[i]);
            }
        }

        delete tree;
    }

    Node* m_tree { nullptr };
};

int main()
{
    Trie trie;

    trie.insert("ada");
    trie.insert("adauga");
    trie.insert("add");

    std::cout << std::boolalpha;

    std::cout << "Is 'ada' in the trie: " << trie.contains("ada") << std::endl;
    std::cout << "Is 'add' in the trie: " << trie.contains("add") << std::endl;
    std::cout << "Is 'adau' in the trie: " << trie.contains("adau") << std::endl;
    std::cout << "Is 'tree' in the trie: " << trie.contains("tree") << std::endl;

    return 0;
}
