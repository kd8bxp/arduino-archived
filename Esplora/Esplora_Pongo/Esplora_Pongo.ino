//Pongo by WaDoMa ;-)
//Extended version of the original example
// found at:
//https://github.com/WaDoMa/Arduino_Esplora_Pongo

#include <Esplora.h>
#include <TFT.h>            // Arduino LCD library
#include <SPI.h>

#define MAX_NUM_BRICKS 40

struct screen
{
  int resolution_h,resolution_v;
};

struct brick
{
  int16_t x,y;
  bool alive;
};

struct ball
{
  int x,y;
  int previous_x,previous_y;
  double delta_x,delta_y;
  int width,height;
  int col_r,col_g,col_b;
};

struct paddle
{
  int x,y;
  int previous_x,previous_y;
  int width,height;
  int col_r,col_g,col_b;
};

screen scr={};
ball ball1={};
paddle paddle1={};
brick bricks[MAX_NUM_BRICKS];
  
void setup()
{
  EsploraTFT.begin();   // initialize the display
  EsploraTFT.background(0, 0, 0);  // set the background the black
  scr = {EsploraTFT.width(),EsploraTFT.height()};  //determine and store resolution of screen
  ball1 = {0,0,0,0,1.1,1.5,5,5,255,255,255};  //initialize ball position to upper left corner, ball trajectory, dimensions and color
  paddle1 = {scr.resolution_h/2,scr.resolution_v/2,0,0,20,5,255,255,255};  //initialize paddle position to the middle of the screen, dimensions and color
  int i=0;
  for(int16_t x=0;x < scr.resolution_h; x=x+20 )
  {
    bricks[i].x = x;
    bricks[i].y = 20;
    bricks[i].alive=true;
    i++;
  }
}

void loop()
{
  int ballmovementintervall = 0;  //Initializes a variable, which stores the waiting time in Arduino-"millis" before moving the ball to its next position. This is de facto the game speed.
  paddle1.x = map(Esplora.readJoystickX(), 512, -512, 0, scr.resolution_h) - paddle1.width / 2;
  paddle1.y = map(Esplora.readJoystickY(), -512, 512, 0, scr.resolution_v) - paddle1.height / 2;
  EsploraTFT.fill(0, 0, 0);  // set the fill color to black 
  if (paddle1.previous_x != paddle1.x || paddle1.previous_y != paddle1.y)  //erases the previous position of the paddle if different from present
  {
    EsploraTFT.rect(paddle1.previous_x, paddle1.previous_y, paddle1.width, paddle1.height);  //blacken old paddle
  }
  EsploraTFT.fill(paddle1.col_r, paddle1.col_b, paddle1.col_g);  //set fill color to paddle color
  EsploraTFT.rect(paddle1.x, paddle1.y, paddle1.width, paddle1.height);  //draw the paddle on screen and save the current position as the previous
  paddle1.previous_x = paddle1.x;
  paddle1.previous_y = paddle1.y;
  
  ballmovementintervall = map(Esplora.readSlider(), 0, 1023, 0, 80) + 1;   // Reads and maps the slider position to determine the speed of the ball. Here, "80" results into the minimum speed and "0" into the maximum speed.
  //Serial.print(millis());  //output millis for debugging
  //Serial.print("\t");
  //Serial.print("\n");
  if (millis() % ballmovementintervall < 2)  //conditon gets periodically TRUE as times goes by 
  {
    moveBall();
  }
  drawBricks();
}

// this function determines the ball's position on screen
void moveBall()
{
  if (ball1.x > scr.resolution_h || ball1.x < 0)  // if the ball goes offscreen, reverse the direction:
  {
    ball1.delta_x = -ball1.delta_x;
  }
  if (ball1.y > scr.resolution_v || ball1.y < 0)
  {
    ball1.delta_y = -ball1.delta_y;
  }

  // check if the ball and the paddle are occupying the same space on screen
  if (inRect(ball1.x, ball1.y, paddle1.x, paddle1.y, paddle1.width, paddle1.height))
  {
    double paddlehitoffcenter = (paddle1.x + paddle1.width / 2.0f) - ball1.x;
    double deflection = paddlehitoffcenter / (paddle1.width / 2.0f);
//    Serial.print(deflection);
//    Serial.print("\t");
    ball1.delta_x = deflection*2.0f;
    ball1.delta_y = -2.0f;
  }

  ball1.x += ball1.delta_x;  // update the ball's position
  ball1.y += ball1.delta_y;

  for(int16_t i = 0;i < MAX_NUM_BRICKS;i++)
  {
    if( ( bricks[i].alive == true) && inRect(ball1.x, ball1.y, bricks[i].x, bricks[i].y, 10, 2) )
    { 
       bricks[i].alive = false;
       EsploraTFT.fill(0, 0, 0);
       EsploraTFT.rect(bricks[i].x, bricks[i].y, 10, 2);
    }
  }
  EsploraTFT.fill(0, 0, 0);// erase the ball's previous position
  if (ball1.previous_x != ball1.x || ball1.previous_y != ball1.y)
  {
    EsploraTFT.rect(ball1.previous_x, ball1.previous_y, ball1.width, ball1.height);
  }
  EsploraTFT.fill(255, 255, 255);  // draw the ball's current position
  EsploraTFT.rect(ball1.x, ball1.y,ball1.width, ball1.height);

  ball1.previous_x = ball1.x;  //store present to previous position
  ball1.previous_y = ball1.y;
  
  /*Serial.print(ball1.x);  //output ball position for debugging
  Serial.print("\t");
  Serial.print(ball1.y);
  Serial.print("\t");
  Serial.print(ball1.delta_x);
  Serial.print("\t");
  Serial.println(ball1.delta_y); */
  
}

// this function checks the position of the ball to see if it intersects with the paddle
boolean inRect(int x, int y, int rectX, int rectY, int rectWidth, int rectHeight)
{
 if ((x >= rectX && x <= (rectX + rectWidth)) && (y >= rectY && y <= (rectY + rectHeight)))
  {
    return true;
  }
  else
  {
    return false;
  }
}

void drawBricks()
{
    for(int16_t i=0;i < MAX_NUM_BRICKS;i++)
    {
      if(bricks[i].alive == true)
      {
          EsploraTFT.rect(bricks[i].x, bricks[i].y, 10, 2);
      }
    }
}


