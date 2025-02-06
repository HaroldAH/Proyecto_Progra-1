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

    using NodePtr = Node*;
    List() : head(nullptr) {}

    Node* getHead() const {
        return head;
    }
    void insertAtEnd(T value) {
        Node* newNode = new Node(value);
        if (!head) {
            head = newNode;
        }
        else {
            Node* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    void deletePosition(int position) {
        if (!head || position < 1) return;

        Node* current = head;
        Node* prev = nullptr;
        int index = 1;


        if (position == 1) {
            head = head->next;
            delete current;
            return;
        }


        while (current != nullptr && index < position) {
            prev = current;
            current = current->next;
            index++;
        }


        if (current != nullptr) {
            prev->next = current->next;
            delete current;
        }
    }


    void print() {
        Node* current = head;
        while (current) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }

    T& getAt(int position) {
        if (position < 1) throw out_of_range("Posicion fuera de rango");
        Node* current = head;
        int index = 1;
        while (current && index < position) {
            current = current->next;
            index++;
        }
        if (!current) throw out_of_range("Posicion fuera de rango");
        return current->data;
    }

};

#endif 
