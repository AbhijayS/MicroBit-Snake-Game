#ifndef FSTK
#define FSTK

/*
  Definition: The Node class stores data for exactly ONE pixel
              Specifically, the X and Y coordinates of the pixel

  Usage:      A collection of Nodes creates the "Snake" body
              (stored inside the FixedStack Data Structure)
*/
class Node {

private:
  Node* next; // (Internal) Pointer to the next Node

public:
  uint8_t x_cord; // x-coordinate of the pixel
  uint8_t y_cord; // y-coordinate of the pixel

  // Initializing the data fields
  Node(uint8_t x, uint8_t y) {
    this->next = NULL;
    this->x_cord = x;
    this->y_cord = y;
  }

};

/*
  Definition: The FixedStack is derived from the Stack (Last In First Out or LIFO) Data Structure
              The FixedStack has a Fixed Max Size, meaning that items at the bottom of the stack will be removed as additional items are pushed onto the stack
              The Max Size can be incremented, but cannot be reduced

  Usage:      The Snake class (defined in the main.cpp file) uses a FixedStack to contruct the "body" of the "Snake"
*/
class FixedStack {

private:
  int max_size; // Max Size of the FixedStack
  int size; // Current size of the FixedStack (excludes NULL elements)

public:
  Node* head; // TO:DO change to void*
  int maxLength;
  int length;
  FixedStack(int size);
  Node* push(uint8_t x, uint8_t y);
  bool isEmpty();
  void incrementMaxSize(uint8_t x, uint8_t y);

};

#endif
