#include <cstdio>
#include "FixedStack.h"

FixedStack::FixedStack(int max) {
  this->head = NULL;
  this->max_size = max;
  this->length = 0;
}

Node* FixedStack::push(uint8_t x, uint8_t y) {
  Node* node = new Node(x, y);

  if (!isEmpty()) {
    Node* temp = this->head;
    this->head = node;
    node->next = temp;

    if (length < max_size) {
      this->length++;
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
    this->length++;
    return NULL;
  }
}

bool FixedStack::isEmpty() {
  if (this->head)
    return false;
  return true;
}

void FixedStack::incrementMaxSize(uint8_t x, uint8_t y) {
  Node* temp = this->head;
  while (temp->next) {
    temp = temp->next;
  }
  temp->next = new Node(x, y);
  this->max_size++;
  this->length++;
}
