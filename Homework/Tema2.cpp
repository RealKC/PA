#include <algorithm>
#include <cmath>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <unordered_map>
#include <vector>

struct Node {
    int weight;
    // -1 for interior nodes
    // the char code for leaves
    int value { -1 };
    Node* left { nullptr };
    Node* right { nullptr };
};

struct Character {
    int frequency;
    char ch;
};

class Heap {
public:
    Heap(std::vector<Character> const& characters)
    {
        m_elements.reserve(characters.size());

        for (auto& character : characters) {
            auto* n = new Node;
            n->weight = character.frequency;
            n->value = character.ch;
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
        return left->weight > right->weight;
    }

    std::vector<Node*> m_elements;
};

std::vector<Character> compute_frequencies(std::string_view s)
{
    std::unordered_map<char, int> frequencies;

    for (auto& ch : s) {
        frequencies[ch]++;
    }

    std::vector<Character> characters;
    characters.reserve(frequencies.size());

    for (auto& [k, v] : frequencies) {
        characters.push_back(Character {
            .frequency = v,
            .ch = k,
        });
    }

    return characters;
}

Node* lep(std::vector<Character> const& characters)
{
    Heap B { characters };

    while (B.length() > 1) {
        auto* t1 = B.pop();
        auto* t2 = B.pop();

        auto* t = new Node;
        t->left = t1;
        t->right = t2;
        t->weight = t1->weight + t2->weight;
        t->value = -1;

        B.push(t);
    }

    return B.pop();
}

std::optional<std::string> get_compressed_representation(char ch, Node* tree, std::string code = "")
{
    if (tree == nullptr) {
        throw std::runtime_error("Invalid call (tree should not be nullptr)");
    }

    if (tree->value != -1) {
        if (tree->value == ch) {
            return code;
        } else {
            return std::nullopt;
        }
    }

    if (auto repr = get_compressed_representation(ch, tree->left, code + "0"); repr.has_value()) {
        return repr;
    }

    if (auto repr = get_compressed_representation(ch, tree->right, code + "1"); repr.has_value()) {
        return repr;
    }

    return std::nullopt;
}

std::string make_compressed(std::string_view s, Node* tree)
{
    std::string ret;
    ret.reserve(s.length());

    for (char ch : s) {
        ret += *get_compressed_representation(ch, tree);
    }

    return ret;
}

int main()
{
    std::string s;

    std::cout << "Input a string: ";
    std::string line;
    while (std::getline(std::cin, line)) {
        s += line;
    }

    auto frequencies = compute_frequencies(s);
    auto* weighted_tree = lep(frequencies);
    auto compressed = make_compressed(s, weighted_tree);

    double original_length = s.length() * 8.0;
    double compressed_length = std::ceil(compressed.length());
    auto ratio = compressed_length / original_length * 100;

    std::cout << "The compressed string is " << ratio << "% of the original string.\n";

    return 0;
}
