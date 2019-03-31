/*
 RS232 Tester V0.1
 Display received serial data on LCD screen,
 save a received string in RAM and play back
 on request.

 Buttons connected on stated pins, in a D-Pad formation.

 Current button functions;
 UP - cycle baud rates
 DOWN - repeat last received string back to device
 LEFT - save the last received string in RAM for later playback
 RIGHT - play the saved string back to device

 More features coming...

 V0.1 released Apr 2019 by Dane Lewis
 https://www.danelewis.com/
 */

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

int upPin = 4; // up button
int dnPin = 5; // down button
int ltPin = 3; // left button
int rtPin = 2; // right button

int debounce = 1000;  // delay after each button press (cheap debounce)

int baudCurrent = 3; // holds the index of the currently selected baud rate
long baudRates[] = {1200, 2400, 4800, 9600, 19200, 38400l, 57600l, 115200l}; // list of baud rates

char array1[16]="RS232 Tester"; // spash screen top
char array2[16]="V0.01";  // splash screen bottom
int tim = 500 ; // delay time

char lastChar;  // buffer for reading strings from serial
String lastString;  // variable to hold last received serial string
String savedString; // holds the saved string for playback

LiquidCrystal_I2C lcd(0x3E,16,2); // set the LCD address to 0x3E for a 16 chars and 2 line display

void setup()
{
  pinMode(upPin, INPUT_PULLUP);
  pinMode(dnPin, INPUT_PULLUP);
  pinMode(ltPin, INPUT_PULLUP);
  pinMode(rtPin, INPUT_PULLUP);
  
  lcd.init(); // initialize the lcd
  lcd.backlight(); // turn the backlight on
  lcd.print(array1); // display splash top
  lcd.setCursor(0,1); // move cursor to bottom line
  lcd.print(array2);  // display spash bottom
  delay(2000);  // wait
  lcd.clear();
  lcd.print("Created By");  // show a second splash
  lcd.setCursor(0,1);
  lcd.print("Dane Lewis");
  delay(2000); // wait
  lcd.clear();
  lcd.print("Baud: "); // display baud rate on top line
  lcd.print(baudRates[baudCurrent]);

  Serial.begin(baudRates[baudCurrent]); // start the serial connection
}
/*********************************************************/
void loop() 
{
  // UP
  if(digitalRead(upPin) == LOW){
    // cycle the baud rate
    cycleBaud();
    delay(debounce);
  }
  // DOWN
  if(digitalRead(dnPin) == LOW){
    // repeat last string back to device
    Serial.print(lastString);
    delay(debounce);
  }
  // LEFT
  if(digitalRead(ltPin) == LOW){
    // save the last string in ram
    savedString = lastString;
    delay(debounce);
  }
  // RIGHT
  if(digitalRead(rtPin) == LOW){
    // play saved string back
    Serial.print(savedString);
    delay(debounce);
  }
  
  // if serial string received
  if(Serial.available()){
    // clear the bottom line
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lastString = "";
    delay(100);
    lcd.setCursor(0, 1);
    // print each character to the bottom line of the screen
    // store the string in last string variable
    while(Serial.available() > 0){
      lastChar = Serial.read();
      lcd.write(lastChar);
      lastString += lastChar;
    }
  }
}

// function to cycle the baud rate and restart the serial connection
void cycleBaud()
{
  lcd.clear();
  
  if((baudCurrent+1) >= (sizeof(baudRates)/sizeof(baudRates[0]))){
    baudCurrent = 0;
  } 
  else{
    baudCurrent++;
  }
  lcd.print("Baud: ");
  lcd.print(baudRates[baudCurrent]);
  Serial.end();
  Serial.begin(baudRates[baudCurrent]);
  
}
