#ifndef FSTK
#define FSTK

class Node {
public:
  int x;
  int y;
  Node* next;

  Node(int x0, int y0) {
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
  Node* push(int x, int y);
  bool isEmpty();
  void incrementMaxSize(int x, int y);
};
#endif
