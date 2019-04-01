#include <cstdio>
#include "FixedStack.h"

FixedStack::FixedStack(int max) {
  this->head = NULL;
  this->max_size = max;
  this->size = 0;
}

int FixedStack::size() { return this->size; }

bool FixedStack::isEmpty() {
  if (this->head)
  return false;
  return true;
}

Node* FixedStack::peek() {
  return this->head;
}

Node* FixedStack::push(Node* node) {

  if (!isEmpty()) {
    Node* temp = this->head;
    this->head = node;
    node->next = temp;

    if (size < max_size) {
      this->size++;
      return NULL;
    }else{
        temp = this->head;
        while (temp->next->next) {
        temp = temp->next;
        }

        Node* tr = temp->next;
        temp->next = NULL;
        return tr;
    }
  }else {
    node->next = NULL;
    this->head = node;
    this->size++;
    return NULL;
  }
}


void FixedStack::incrementMaxSize(Node* node) {
  node->next = this->head;
  this->head = node;
  this->max_size++;
  this->size++;
}
