#include "MicroBit.h"
#include "FixedStack.h"
#define WIDTH 5
#define HEIGHT 5

MicroBit uBit;
enum DIRECTION {LEFT, RIGHT, UP, DOWN};

class Pixel {
public:
  int X;
  int Y;
  Pixel();
  void refresh();
  void display(MicroBitImage m, int brightness);
} Apple;

class Snake {
public:

  int X;
  int Y;
  int score;
  DIRECTION direction;
  FixedStack* body;

  Snake() {
    X = 0;
    Y = 0;
    score = 1;
    direction = RIGHT;
    body = new FixedStack(1);
    body->push(X, Y);
  }

  bool offScreen() {
    if (X < 0 || X > 4 || Y < 0 || Y > 4)
      return true;
    return false;
  }

  void move() {
    switch (direction) {
      case UP:    Y--;
                  break;
      case DOWN:  Y++;
                  break;
      case LEFT:  X--;
                  break;
      case RIGHT: X++;
                  break;
    }

    Node* tmp = body->push(X, Y);

    if (X == (Apple.X) && Y == (Apple.Y)) {
      body->incrementMaxSize(tmp->x, tmp->y);
      Apple.refresh();
      score++;
    }
  }

  void display(MicroBitImage m, int brightness) {
    Node* tmp = body->head;
    while (tmp) {
      m.setPixelValue(tmp->x, tmp->y, brightness);
      tmp = tmp->next;
    }
  }
  
  bool occupied(int x, int y) {
    Node* n = this->body->head;
    while (n) {
      if (n->x == x && n->y == y) {
        return true;
      }
      n = n->next;
    }
    return false;
  }
  
  bool selfDestruct() {
    if (this->body->length >= 5) {
      Node* n = this->body->head->next->next->next->next;
      while (n) {
        if (n->x == this->body->head->x && n->y == this->body->head->y) {
          return true;
        }
        n = n->next;
      }
    }
    return false;
  }
  
} Body;

Pixel::Pixel() {
  uBit.init();
  X = uBit.random(WIDTH);
  Y = uBit.random(HEIGHT);
}

void Pixel::refresh() {
  (this->X) = uBit.random(WIDTH);
  (this->Y) = uBit.random(HEIGHT);
  while (Body.occupied(this->X, this->Y)) {
    (this->X) = uBit.random(WIDTH);
    (this->Y) = uBit.random(HEIGHT);
  }
}

void Pixel::display(MicroBitImage m, int brightness) {
  m.setPixelValue(X, Y, brightness);
}

void onButtonA(MicroBitEvent e) {
  switch (Body.direction) {
    case UP:    Body.direction = LEFT;
                break;
    case DOWN:  Body.direction = RIGHT;
                break;
    case LEFT:  Body.direction = DOWN;
                break;
    case RIGHT: Body.direction = UP;
                break;
  }
}

void onButtonB(MicroBitEvent e) {
  switch (Body.direction) {
    case UP:    Body.direction = RIGHT;
                break;
    case DOWN:  Body.direction = LEFT;
                break;
    case LEFT:  Body.direction = UP;
                break;
    case RIGHT: Body.direction = DOWN;
                break;
  }
}

int main() {
  uBit.init();
  uBit.display.setDisplayMode(DISPLAY_MODE_GREYSCALE);

  const uint8_t game[] = {
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
  };

  uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_EVT_ANY, onButtonB);
  uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_EVT_ANY, onButtonA);

  MicroBitImage map(WIDTH, HEIGHT, game);
  map.setPixelValue(Body.X, Body.Y, 10);
  map.setPixelValue(Apple.X, Apple.Y, 255);
  uBit.sleep(1000);
  
  while (1) {
    map.clear();
    Body.move();

    if (Body.offScreen() || Body.selfDestruct())
      break;

    Body.display(map, 10);
    Apple.display(map, 255);
    uBit.display.print(map);
    
    uBit.sleep(500);
  }

  while (1) {
    uBit.display.scroll("GAME OVER!");
    uBit.display.scroll("SCORE ");
    uBit.display.scroll(Body.score);
  }
}
