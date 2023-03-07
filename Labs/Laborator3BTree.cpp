#include <stdio.h>

static constexpr int SHRINKING_FACTOR = 2;

struct BTreeNode {
    enum class Type {
        Leaf,
        Inner,
    };

    int key_count;
    Type type;
    int keys[2 * SHRINKING_FACTOR - 1];
    BTreeNode* children[2 * SHRINKING_FACTOR];
};

void split_node(BTreeNode* u, int i, BTreeNode* v)
{
    auto* w = new BTreeNode;
    w->type = v->type;
    w->key_count = SHRINKING_FACTOR - 1;
    for (int j = 0; j <= SHRINKING_FACTOR - 2; ++j) {
        w->keys[j] = v->keys[SHRINKING_FACTOR + j];
    }

    if (v->type == BTreeNode::Type::Inner) {
        for (int j = 0; j <= SHRINKING_FACTOR - 1; ++j) {
            w->children[j] = v->children[j + SHRINKING_FACTOR];
        }
    }

    v->key_count = SHRINKING_FACTOR - 1;

    for (int j = u->key_count; j >= i + 1; --j) {
        u->children[j + 1] = u->children[j];
    }

    u->children[i + 1] = w;

    for (int j = u->key_count - 1; j >= i; --j) {
        u->keys[j + 1] = u->keys[j];
    }

    u->keys[i] = v->keys[SHRINKING_FACTOR - 1];
    u->key_count++;
}

void insert_in_incomplete_node(BTreeNode* v, int k)
{
    int i = v->key_count - 1;
    if (v->type == BTreeNode::Type::Leaf) {
        while (i >= 0 and k < v->keys[i]) {
            v->keys[i + 1] = v->keys[i];
            --i;
        }
        v->keys[i + 1] = k;
        ++v->key_count;
    } else {
        while (i >= 0 and k < v->keys[i]) {
            --i;
        }
        ++i;
        if (v->children[i]->key_count == (2 * SHRINKING_FACTOR - 1)) {
            split_node(v, i, v->children[i]);
            if (k > v->keys[i])
                ++i;
        }
        insert_in_incomplete_node(v->children[i], k);
    }
}

void insert_btree(BTreeNode*& t, int k)
{
    auto* v = t;

    if (v->key_count == (2 * SHRINKING_FACTOR - 1)) {
        auto* u = new BTreeNode;
        t = u;
        u->type = BTreeNode::Type::Inner;
        u->key_count = 0;
        u->children[0] = v;
        split_node(u, 0, v);
        insert_in_incomplete_node(u, k);
    } else {
        insert_in_incomplete_node(v, k);
    }
}

void search_btree(BTreeNode const* t, int k)
{
    int i = 0;
    while (i < t->key_count and k > t->keys[i]) {
        ++i;
    }

    if (i < t->key_count and k == t->keys[i]) {
        printf("Found '%d' at index %d in node %p\n", k, i, t);
        return;
    }

    if (t->type == BTreeNode::Type::Leaf) {
        printf("Did not find '%d' in tree\n", k);
    } else {
        search_btree(t->children[i], k);
    }
}

int main()
{
    auto* tree = new BTreeNode;
    tree->key_count = 0;
    tree->type = BTreeNode::Type::Leaf;

    insert_btree(tree, 3);
    insert_btree(tree, 4);
    insert_btree(tree, 5);
    insert_btree(tree, 6);

    search_btree(tree, 3);
    search_btree(tree, 6);
    search_btree(tree, 100);

    insert_btree(tree, -100);
    insert_btree(tree, 100);

    return 0;
}
