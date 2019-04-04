#include "MicroBit.h"
#include "FixedStack.h"


MicroBit uBit;
bool FINISHED; // True if player WINS the game
enum DIRECTION {LEFT, RIGHT, UP, DOWN}; // Game Controls
const uint8_t WIDTH = 5; // WIDTH of the LED matrix
const uint8_t HEIGHT = 5; // HEIGHT of the LED matrix
const uint8_t SNAKE_BRIGHTNESS = 25; // Snake's brightness on the screen
const uint8_t FRUIT_BRIGHTNESS = 255; // Fruit's brightness on the screen


class RandomPixel {
public:
  uint8_t X; // x-coordinate of the fruit
  uint8_t Y; // y-coordinate of the fruit

  RandomPixel();
  void refresh(); // Refresh the X and Y coordinates of the fruit to a random, unoccupied position
  void display(MicroBitImage m, uint8_t brightness); // Display the fruit on the LED Matrix
} Fruit;


class Snake {
public:
  signed char X; // x-coordinate of the Head of the Snake
  signed char Y; // y-coordinate of the Head of the Snake
  uint8_t score; // Current score of the game
  DIRECTION direction; // Current heading of the Snake
  FixedStack* body; // Snake's body implemented using the FixedStack data structure
  bool setDirection; // The player is allowed to set the Snake's direction only once per movement of the Snake

  Snake();
  bool offScreen(); // Returns true if the Snake goes outside the 5x5 LED Matrix
  void move(); // Increment the Snake's position in the direction it's headed
  void display(MicroBitImage m, uint8_t brightness); // Display the Snake's body on the LED Matrix
  bool occupied(uint8_t x, uint8_t y); // Returns true if the Snake's body is currently occupying the specified coordinates
  bool selfDestruct(); // Returns true if the Snake runs into itself
} SnakeBody;


/*
Snake function definitions
*****************************************
*/
Snake::Snake() {
  X = uBit.random(WIDTH-2);
  Y = uBit.random(HEIGHT);
  score = 1;
  setDirection = true;
  direction = RIGHT;
  body = new FixedStack(1);
  body->push(new Node(X, Y));
}

bool Snake::offScreen() {
  if (X < 0 || X > (WIDTH-1) || Y < 0 || Y > (HEIGHT-1))
  return true;
  return false;
}

void Snake::move() {
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

  if (X == (Fruit.X) && Y == (Fruit.Y)) {

    body->incrementMaxSize(new Node(X, Y));
    score++;
    if (score == 25) FINISHED = true;
    else Fruit.refresh();

  }else delete body->push(new Node(X, Y));

  setDirection = true;
}

void Snake::display(MicroBitImage m, uint8_t brightness) {
  Node* itr = body->peek();
  while (itr) {
    m.setPixelValue(itr->x_cord, itr->y_cord, brightness);
    itr = itr->next;
  }
}

bool Snake::occupied(uint8_t x, uint8_t y) {
  Node* itr = this->body->peek();
  while (itr) {
    if (itr->x_cord == x && itr->y_cord == y)
      return true;
    itr = itr->next;
  }
  return false;
}

bool Snake::selfDestruct() {
  if (this->body->size() >= 5) {
    Node* itr = this->body->peek()->next->next->next->next;
    while (itr) {
      if (itr->x_cord == this->body->peek()->x_cord && itr->y_cord == this->body->peek()->y_cord) {
        return true;
      }
      itr = itr->next;
    }
  }
  return false;
}
/*
*****************************************
*/


/*
RandomPixel function definitions
*****************************************
*/
RandomPixel::RandomPixel() {
  uBit.init();
  (this->X) = uBit.random(2)+3;
  (this->Y) = uBit.random(HEIGHT);
}

void RandomPixel::refresh() {
  do {
    (this->X) = uBit.random(WIDTH);
    (this->Y) = uBit.random(HEIGHT);
  } while(SnakeBody.occupied(this->X, this->Y));
}

void RandomPixel::display(MicroBitImage m, uint8_t brightness) {
  m.setPixelValue(X, Y, brightness);
}
/*
*****************************************
*/

/*
User Events
*****************************************
*/
void onButtonA(MicroBitEvent e) {
  if (SnakeBody.setDirection) {
    switch (SnakeBody.direction) {
      case UP:    SnakeBody.direction = LEFT;
      break;
      case DOWN:  SnakeBody.direction = RIGHT;
      break;
      case LEFT:  SnakeBody.direction = DOWN;
      break;
      case RIGHT: SnakeBody.direction = UP;
      break;
    }
    SnakeBody.setDirection = false;
  }
}

void onButtonB(MicroBitEvent e) {
  if (SnakeBody.setDirection) {
    switch (SnakeBody.direction) {
      case UP:    SnakeBody.direction = RIGHT;
      break;
      case DOWN:  SnakeBody.direction = LEFT;
      break;
      case LEFT:  SnakeBody.direction = UP;
      break;
      case RIGHT: SnakeBody.direction = DOWN;
      break;
    }
    SnakeBody.setDirection = false;
  }
}
/*
*****************************************
*/


int main() {
  uBit.init();
  FINISHED = false;
  uBit.display.setDisplayMode(DISPLAY_MODE_GREYSCALE);
  uBit.display.setBrightness(100);
  uBit.display.print("SNAKE", 500);
  uBit.display.print(3);
  uBit.sleep(800);
  uBit.display.print(2);
  uBit.sleep(800);
  uBit.display.print(1);
  uBit.sleep(800);

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

  map.setPixelValue(SnakeBody.X, SnakeBody.Y, SNAKE_BRIGHTNESS);
  map.setPixelValue(Fruit.X, Fruit.Y, FRUIT_BRIGHTNESS);
  uBit.display.print(map);

  uBit.sleep(1000);

  while (1) {
    map.clear();
    SnakeBody.move();

    if (SnakeBody.offScreen() || SnakeBody.selfDestruct() || FINISHED)
      break;

    SnakeBody.display(map, SNAKE_BRIGHTNESS);
    Fruit.display(map, FRUIT_BRIGHTNESS);
    uBit.display.print(map);

    uBit.sleep(750);
  }


  uBit.display.clear();


  /*
    Game over Animations
  */
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
    uBit.display.scroll("GAME OVER", 100);
    uBit.display.scroll("SCORE:", 100);

    while (1) {
      uBit.display.print(SnakeBody.score);
      uBit.sleep(1000);
    }
  }
}
