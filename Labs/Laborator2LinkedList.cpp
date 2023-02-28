#include <iostream>
#include <stdio.h>
#include <time.h>

struct Node {
    Node* next;
    int data;
};

void add_node_swap(Node*& head, int data)
{
    auto* node = new Node;
    node->next = head->next;
    node->data = head->data;

    head->next = node;
    head->data = data;

    head = node;
}

int add_node_iter(Node*& head, int data)
{
    int op_count = 1;

    auto* node = new Node;
    node->data = data;
    node->next = head;

    auto* it = head;

    while (it->next != head) {
        it = it->next;
        op_count++;
    }

    it->next = node;

    return op_count;
}

void print_list(Node* head)
{
    if (!head)
        return;

    std::cout << "last at: " << head << "\n";

    auto* it = head;

    do {
        printf("Node { .data=%2d .next=%p }\n", it->data, it->next);
        it = it->next;
    } while (it != head);
}

void free_list(Node*& head)
{
    if (!head)
        return;

    auto* it = head->next;

    while (it != head) {
        auto* to_delete = it;
        it = it->next;
        delete to_delete;
    }

    delete head;
    head = nullptr;
}

int main()
{
    clock_t start, end;
    int op_count_iter;

    auto* head = new Node;
    head->data = 5;
    head->next = head;

    start = clock();
    for (int i = 0; i < 10'000; ++i) {
        add_node_swap(head, i);
    }

    end = clock();
    free_list(head);

    double elapsed_time_swap = static_cast<double>(end - start) / CLOCKS_PER_SEC;

    head = new Node;
    head->data = 5;
    head->next = head;

    start = clock();
    op_count_iter = 0;
    for (int i = 0; i < 10'000; ++i) {
        op_count_iter += add_node_iter(head, i);
    }

    end = clock();
    free_list(head);

    double elapsed_time_iter = static_cast<double>(end - start) / CLOCKS_PER_SEC;

    std::cerr << "Elapsed time (swap): " << elapsed_time_swap << "\nElapsed time (iter): " << elapsed_time_iter << "\n";
    std::cerr << "Iterarea a executat " << op_count_iter << " operatii\n";

    return 0;
}
