/*
 * File: FlappyArduino.ino
 * -----------------------
 * Author: JT Cho
 * [jonathan.t.cho@gmail.com | joncho@seas.upenn.edu]
 * 
 * A minimalistic "Flappy Bird" type game for
 * the Arduino and a segmented display of 2x16 dimensions.
 * This was my first program that I made on an Arduino, using
 * the Intel Galileo.
 * I made this program in around an hour while taking a break from
 * my Fall 2014 PennApps hackathon project! :)
 */


#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd; //I2C port

const int pinButton = 3; //D3 port
const int difficulty = 100;
//Number of cols in display
int maxX = 16;
//Player coords
int x = 0;
int y = 1;
//Obstacle strings
String obstacles[2] = {"                ", "                "};

/*
 * Sets up the program.
 */
void setup() {
  Serial.begin(9600);
  pinMode(pinButton, INPUT);
  lcd.begin(maxX, 2);
  randomSeed(analogRead(0));
}

/*
 * Generate the random obstacle strings,
 * with frequency of obstacles determined
 * by the difficulty.
 */
void updateObstacles() {
  //
  obstacles[0] = obstacles[0].substring(1);
  obstacles[1] = obstacles[1].substring(1);
  
  int topChance = random(0, 100);
  int bottomChance = random(0, 100);
  if (topChance < difficulty) {
    obstacles[0] += "V";
    obstacles[1] += " ";
  }
  else {
    obstacles[0] += " ";
    if (bottomChance < difficulty)
      obstacles[1] += "n";
    else
      obstacles[1] += " ";
  }
}

/*
 * Main program loop.
 * Runs the game.
 */
void loop() {
  lcd.clear(); 
  if(digitalRead(pinButton)) {
     y = !y;
  }
  
  if (x < maxX/2)
    x++;
  
  //Player dies.
  if (obstacles[y].charAt(x) != ' ') {
    lcd.clear();
    lcd.print("!YOU LOSE!");
    int x = 0;
    int y = 1;
    obstacles[0] = obstacles[1] = "                ";
    for (int i = 0; i < 30; i++) {
      lcd.scrollDisplayLeft();
      delay(100);
    }
  }
  //Tick the game.
  else {
    lcd.print(obstacles[0]);
    lcd.setCursor(0, 1);
    lcd.print(obstacles[1]);
  
    lcd.setCursor(x, y);
    lcd.print('o');
    updateObstacles();
    delay(100);
  }
}
