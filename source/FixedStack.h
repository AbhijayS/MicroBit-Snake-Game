#ifndef FSTK
#define FSTK

class Node {
public:
  uint8_t x;
  uint8_t y;
  Node* next;

  Node(uint8_t x0, uint8_t y0) {
    next = NULL;
    x = x0;
    y = y0;
  }
};

class FixedStack {
public:
  Node* head;
  int maxLength;
  int length;
  FixedStack(int size);
  Node* push(uint8_t x, uint8_t y);
  bool isEmpty();
  void incrementMaxSize(uint8_t x, uint8_t y);
};
#endif
