#ifndef FSTK
#define FSTK

/*
  Definition: The Node class stores data for exactly ONE pixel
              Specifically, the X and Y coordinates of the pixel

  Usage:      A collection of Nodes creates the "Snake" body
              (stored inside the FixedStack Data Structure)
*/
class Node {

public:
  uint8_t x_cord; // x-coordinate of the pixel
  uint8_t y_cord; // y-coordinate of the pixel
  Node* next; // (Internal) Pointer to the next Node

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

  Usage:      The Snake class (defined in the "main.cpp" file) uses a FixedStack to contruct the "body" of the "Snake"
*/
class FixedStack {

private:
  int max_size; // Max Size of the FixedStack
  int stack_size; // Current size of the FixedStack (Excludes NULL elements)
  Node* head; // Head of the FixedStack

public:
  FixedStack(int size); // Initialize the FixedStack with a max size
  int size(); // Returns the current size of the FixedStack
  bool isEmpty(); // Returns true if the FixedStack is empty
  Node* peek(); //Rreturns the head Node of the FixedStack
  Node* push(Node* node); // Push a Node onto the FixedStack. Returns the last Node if overflows and NULL otherwise.
  void incrementMaxSize(Node* node); // Increase the max size of the FixedStack by 1 and push a new Node onto the FixedStack

};

#endif
