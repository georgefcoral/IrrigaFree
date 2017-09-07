/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>
#include <DHT11.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

/*Cejas del ojo*/
byte cejainicio[8] = { B00000, B00011, B00111, B01100, B11000, B10000, B00000, B00000 };
byte cejaenmedio[8]= {B00000,B11111,B11111,B00000,B00000,B00000,B00000,B00000};
byte cejafinal [8]= {B00000,B11000,B11100,B00110,B00011,B00000,B00000};
/*Ojo */
byte ojo11[8]={B00001,B00011,B00110,B01100,B11000,B10000,B10000,B10000};
byte ojo12[8]={B11111,B00000,B0000,B00000,B00000,B00000,B00000,B01110};
byte ojo13[8]={B10000,B11000,B01100,B00110,B00011,B00001,B00001,B00001};

byte ojo21[8]={B10000,B10000,B10000,B10000,B10000,B10000,B10000,B10000};
byte ojo22[8]={B11111,B11111,B11111,B11111,B11111,B11111,B11111,B01110};
byte ojo23[8]={B00001,B00001,B00001,B00001,B00001,B00001,B00001,B00001};

byte ojo31[8]={B11000,B01100,B00110,B00011,B00001,B00000,B00000,B00000};
byte ojo32[8]={B00000,B00000,B00000,B00000,B11111,B00000,B00000,B00000};
byte ojo33[8]={B00011,B00110,B011000,B11000,B10000,B00000,B00000,B00000};


void setup() {
  lcd.begin(20, 4);
  lcd.createChar(0,cejainicio);
  lcd.createChar(1,cejaenmedio);
  lcd.createChar(2,cejafinal);

  lcd.createChar(3,ojo11);
  lcd.createChar(4,ojo12);
  lcd.createChar(5,ojo13);

  lcd.createChar(6,ojo21);
  lcd.createChar(7,ojo22);
  lcd.createChar(8,ojo23);

  lcd.createChar(9,ojo31);
  lcd.createChar(10,ojo32);
  lcd.createChar(11,ojo33);
lcd.home();
  //lcd.setCursor(1,1);
  lcd.write(byte(0));
  //lcd.write(byte(1));
 // lcd.write(byte(2));
  //lcd.setCursor(5,2);
  lcd.write(byte(3));
  lcd.write(byte(4));
  lcd.write(byte(5));
  //lcd.setCursor(5,3);
  lcd.write(byte(6));
  lcd.write(byte(7));
  lcd.write(byte(8));
  //lcd.setCursor(5,4);
  lcd.write(byte(9));
  lcd.write(byte(10));
  lcd.write(byte(11));
}

void loop() {
  
}

