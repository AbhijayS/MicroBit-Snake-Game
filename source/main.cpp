#include "MicroBit.h"
#include "FixedStack.h"

MicroBit uBit;
enum DIRECTION {LEFT, RIGHT, UP, DOWN};
const uint8_t WIDTH = 5;
const uint8_t HEIGHT = 5;
bool FINISHED;

class Pixel {
public:
  uint8_t X;
  uint8_t Y;
  Pixel();
  void refresh();
  void display(MicroBitImage m, uint8_t brightness);
} Apple;

class Snake {
public:

  signed char X;
  signed char Y;
  uint8_t score;
  DIRECTION direction;
  FixedStack* body;
  bool setDirection;

  Snake() {
    X = uBit.random(WIDTH-2);
    Y = uBit.random(HEIGHT);
    score = 1;
    setDirection = true;
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
      score++;
      if (score == 25)
        FINISHED = true;
      else
        Apple.refresh();
    }
    
    delete tmp;
    setDirection = true;
  }

  void display(MicroBitImage m, uint8_t brightness) {
    Node* tmp = body->head;
    while (tmp) {
      m.setPixelValue(tmp->x, tmp->y, brightness);
      tmp = tmp->next;
    }
  }

  bool occupied(uint8_t x, uint8_t y) {
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
  (this->X) = uBit.random(2)+3;
  (this->Y) = uBit.random(HEIGHT);
}

void Pixel::refresh() {
  do {
    (this->X) = uBit.random(WIDTH);
    (this->Y) = uBit.random(HEIGHT);
  } while(Body.occupied(this->X, this->Y));
}

void Pixel::display(MicroBitImage m, uint8_t brightness) {
  m.setPixelValue(X, Y, brightness);
}

void onButtonA(MicroBitEvent e) {
  if (Body.setDirection) {
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
    Body.setDirection = false;
  }
}

void onButtonB(MicroBitEvent e) {
  if (Body.setDirection) {
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
    Body.setDirection = false;
  }
}

int main() {
  uBit.init();
  FINISHED = false;
  uBit.display.setDisplayMode(DISPLAY_MODE_GREYSCALE);
  uBit.display.setBrightness(100);
  uBit.display.print("SNAKE 3 2 1", 500);

  const uint8_t game[] = {
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
  };
  MicroBitImage map(WIDTH, HEIGHT, game);

  uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_EVT_ANY, onButtonA);
  uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_EVT_ANY, onButtonB);

  map.setPixelValue(Body.X, Body.Y, 10);
  map.setPixelValue(Apple.X, Apple.Y, 255);
  uBit.display.print(map);
  uBit.sleep(1000);

  while (1) {
    map.clear();
    Body.move();

    if (Body.offScreen() || Body.selfDestruct() || FINISHED)
      break;

    Body.display(map, 10);
    Apple.display(map, 255);
    uBit.display.print(map);

    uBit.sleep(750);
  }

  uBit.display.clear();

  if (FINISHED) {
    MicroBitImage fill("255,255,255,255,255\n255,255,255,255,255\n255,255,255,255,255\n255,255,255,255,255\n255,255,255,255,255\n");
    uBit.display.print(fill);
    MicroBitImage f1("0,0,0,0,0\n0,0,0,0,0\n0,0,255,0,0\n0,0,0,0,0\n0,0,0,0,0\n");
    MicroBitImage f2("0,0,0,0,0\n0,255,255,255,0\n0,255,0,255,0\n0,255,255,255,0\n0,0,0,0,0\n");
    MicroBitImage f3("255,255,255,255,255\n255,0,0,0,255\n255,0,0,0,255\n255,0,0,0,255\n255,255,255,255,255\n");
  
    uBit.sleep(1000);
    while (1) {
      uBit.display.clear();
      
      uBit.display.print(f2);
      uBit.sleep(500);
      
      uBit.display.print(f1);
      uBit.sleep(500);
      
      uBit.display.clear();
      uBit.sleep(500);
      
      uBit.display.print(f1);
      uBit.sleep(500);
      
      uBit.display.print(f2);
      uBit.sleep(500);
      
      uBit.display.print(f3);
      uBit.sleep(500);
    }
  }else{
    uBit.display.scroll("GAME OVER", 70);
    uBit.display.scroll("SCORE:", 70);

    while (1) {
      uBit.display.print(Body.score);
      uBit.sleep(1000);
    }
  }
}
