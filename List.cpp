#include "list.h"

// Constructor
template <typename T>
List<T>::List() : head(nullptr) {}

// Insertar al final de la lista
template <typename T>
void List<T>::insertAtEnd(T value) {
    Node* newNode = new Node(value);
    if (!head) {
        head = newNode;
    } else {
        Node* current = head;
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Imprimir la lista
template <typename T>
void List<T>::print() {
    Node* current = head;
    while (current) {
        cout << current->data << " ";
        current = current->next;
    }
    cout << endl;
}
