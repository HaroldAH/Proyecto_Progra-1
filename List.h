#ifndef LIST_H
#define LIST_H

#include <iostream>

using namespace std;

template <typename T>
class List {
private:
    struct Node {
        T data;
        Node* next;
        Node(T value) : data(value), next(nullptr) {}
    };

    Node* head;

public:
    List();
    void insertAtEnd(T value);
    void print();
};

#endif // LIST_H
