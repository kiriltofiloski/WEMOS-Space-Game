#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int num = 0;
 
void setup() {
  Serial.begin(115200);
  
  // Start I2C Communication SDA = 5 and SCL = 4 on Wemos Lolin32 ESP32 with built-in SSD1306 OLED
  Wire.begin(5, 4);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000); // Pause for 2 seconds
 
  // Clear the buffer.
  display.clearDisplay();

  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(15, 15);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.println(F("Push to"));
  display.println(F("  start"));
  
  display.display();

  pinMode(25, INPUT);
  pinMode(26, INPUT);
  
}

int ship_x1 = 15;
int ship_x2 = 22;
int ship_x3 = 15;

int ship_y1 = 22;
int ship_y2 = 29;
int ship_y3 = 36;

int asteroid_x = 128;
int asteroid_y = 25;
int asteroid_rad = 4;

int score = 0;

int playing = 0;

void loop() {
  if(playing == 1){
    if(digitalRead(25) == HIGH){ // check for button inpu and update ship position accordingly
      updateShip(-5);
    }
    else if(digitalRead(26) == HIGH){
      updateShip(5);
    }
  
    updateAsteroid(); // update asteroid position
    
    display.clearDisplay();
    display.setTextSize(2); 
    display.drawTriangle(ship_x1,ship_y1,ship_x2,ship_y2,ship_x3,ship_y3,WHITE);
    display.drawCircle(asteroid_x,asteroid_y,asteroid_rad, WHITE);
    display.fillCircle(asteroid_x,asteroid_y,asteroid_rad, WHITE);
    display.setCursor(0,0);
    display.print(score);
    display.display();

    checkCollision(); //check if ship collided with asteroid

    delay(100);  
  }
  else{
    if(digitalRead(25) == HIGH || digitalRead(26) == HIGH){
      playing = 1;
    }
  }
}

void updateShip(int upOrDown){
  if ((upOrDown > 0 && ship_y3 < 64) || (upOrDown < 0 && ship_y1 > 0)){
    ship_y1 += upOrDown;
    ship_y2 += upOrDown;
    ship_y3 += upOrDown;
  }
}

void updateAsteroid(){
  if(asteroid_x > -5){ //if it hasn't left the screen
    asteroid_x -= 10;
  }
  else{ //create new asteroid
    asteroid_x = 130;
    asteroid_y = (rand() % (60 - 5 + 1)) + 5; //generate random y position
    score += 1;
  }
}

void checkCollision() {
  if(asteroid_x >= ship_x1 && asteroid_x <= ship_x2 && asteroid_y >= ship_y1 && asteroid_y <= ship_y3){
    display.setCursor(15,15);
    display.clearDisplay();
    display.setTextSize(2);
    display.println(F("You lose!"));
    display.display();
    playing = 0;

    ship_x1 = 15;
    ship_x2 = 22;
    ship_x3 = 15;

    ship_y1 = 22;
    ship_y2 = 29;
    ship_y3 = 36;

    asteroid_x = 128;
    asteroid_y = 25;
    asteroid_rad = 4;

    score = 0;
  }
}
