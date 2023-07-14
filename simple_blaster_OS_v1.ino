// This script is a simple Blaster OS; By default is plays a blast sound and lights red; Pressing the mode change button
// switches to stun; In stun the stun sound plays and lights blue;  
// Ammo count starts at 50 and counts down with bars; At zero empty click plays until mode change button is pressed for reload
// Sounds are read from SD card loaded in DFPLayer mini



#include <Adafruit_NeoPixel.h>
// DFPlayer Mini library that seems to work with all of the DFPlayer Minis
#include <DFPlayerMini_Fast.h>
#include <Arduino.h>
#include <U8g2lib.h>
#if !defined(UBRR1H)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 1); // RX, TX
#endif

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif


//128x32 Ammo display on pins D9 & D10
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R3, /* clock=*/ 9, /* data=*/ 10, /* reset=*/ U8X8_PIN_NONE);   // Adafruit Feather ESP8266/32u4 Boards + FeatherWing OLED
//64x32 Scop display on pins A4 & A5 (native Arduino Every SDA SCL pins)
U8G2_SSD1306_64X32_1F_1_HW_I2C u8g3(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
// End of constructor list

// pin definitions
//Fire button connects to pin D8
int fire = 8;
//Mode change button connects to pin D3
int modechange = 3;

//Addressable LED data pin D11
#define PIN 11  //LED Data Pin
#define NUM_LEDS 9  //Number of LEDs that will be lit


// global variables
//These are the intial values that the script uses to setup counts for mode changes and fire button presses
int count1 = 1;
int count2 = 1;
int lastButtonState1 = 1;
int lastButtonState2 = 1;
long unsigned int lastPress1;
long unsigned int lastPress2;
volatile int buttonFlag1;
volatile int buttonFlag2;
int debounceTime = 20;
int ammo = 50;

DFPlayerMini_Fast myMP3;


//Define addresable LED variables
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // setup pin modes
  pinMode(modechange, INPUT_PULLUP);
  pinMode(fire, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(fire), ISR_button1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(modechange), ISR_button2, CHANGE);
  Serial.begin(9600); // dont forget to set your serial monitor speed to whatever is set here
  Serial.println("Running");
  strip.begin();
  strip.show();
  u8g2.begin();
  u8g3.begin();

  //DFPlayer Setup
  mySerial.begin(9600);
  myMP3.begin(mySerial, true);
  Serial.println("Setting volume to max");
  myMP3.volume(30); // Set volume 0 to 30 (max is 30)

  
  // Ammo Count Boxes
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_inb19_mn);
  u8g2.setCursor (2, 2);
  u8g2.print(ammo);
  
  //Draw Five Ammo Count Boxes
  u8g2.drawBox(2,110,30,17);
  u8g2.drawBox(2,91,30,17);
  u8g2.drawBox(2,72,30,17);
  u8g2.drawBox(2,53,30,17);
  u8g2.drawBox(2,34,30,17);
  u8g2.sendBuffer();

  //Play initial startup sound
  Serial.println("Play startup sound");
  //Triggers the startup sound to play
  //Format calls the directory on the SD card 01 and the third track; make sure files are named with 001, 002, 003, etc)
  myMP3.playFolder(01, 3);
}

void loop() {
  //Fire button check loop
  u8g3.firstPage(); 
  do {
    int a = random(1, 12);
    int b = random(1, 12);
    int c = random(1, 12);
    int d = random(1, 12);
    int e = random(70, 90);
   
    //Draw Scope
    u8g3.drawCircle(32, 15, 15);//Outer Ring
    u8g3.drawCircle(32, 15, 2);//Inner ring
    u8g3.drawLine(20, 15, 15, 15);//left hash
    u8g3.drawLine(44, 15, 49, 15);//right hash
    u8g3.drawLine(32, 30, 32, 25);//bottom hash
    u8g3.drawLine(32, 1, 32, 5);//top hash
    
    u8g3.setFont(u8g2_font_tom_thumb_4x6_tf);
    u8g3.setCursor (46, 6);
    u8g3.print("TI-23");
    u8g3.setCursor(52, 26);
    u8g3.print("RNG");
    u8g3.setCursor(52, 32);
    u8g3.print(e);
    u8g3.print("m");
    //Top Left Bars
    u8g3.drawLine(1, 1, a, 1);//bar 1
    u8g3.drawLine(1, 3, b, 3);//bar 2
    u8g3.drawLine(1, 5, c, 5);//bar 3
    u8g3.drawLine(1, 7, d, 7);//bar 4
    delay(50);
  } while ( u8g3.nextPage() );
  //Debug statements commented out
  //Serial.print("LastButton:");
  //Serial.println(lastButtonState1);
  if((millis() - lastPress1) > debounceTime && buttonFlag1)
  {
    lastPress1 = millis();   //update lastPress                                                     
    if(digitalRead(fire) == 0 && lastButtonState1 == 1)    //if button is pressed and was released last change
    {
      Serial.println("Fire:");
      count1 = count1 + 1;
      //Serial.println(count1);
      //Blast - subroutine to play blast sound and light
      if (count2 == 1 && ammo != 0){
       colorWipe(0xff,0x00,0x00, 2);
       colorWipe(0x00,0x00,0x00, 2); 
       myMP3.playFolder(01, 1);
      }
      //Stun - Subroutine if in stun mode
      if (count2 == 2 & ammo != 0){
       colorWipe(0x00,0x00,0xff, 2);
       colorWipe(0x00,0x00,0x00, 2); 
       myMP3.playFolder(01, 2);
      }
      //Empty - Subroutine if empty; Plays empty sound until mode button is pressed for reload
      if (ammo == 0){
       Serial.println("Empty");
       myMP3.playFolder(01, 5);
      }


      
      
      
      //Ammo Bar
      //Five bars - Displays five bars on ammo display based on ammo count above 40
      if ((ammo <=50) && (ammo >= 40))
      {
        ammo = ammo - 1;
        //Five ammo count boxes already drawn
        Serial.println("50-40");
        u8g2.setCursor (2, 2);
        u8g2.print(ammo);
        u8g2.sendBuffer();
      }

      //four bars -Displays four bars on ammo display based on ammo count above 30
      if ((ammo <= 39) && (ammo >= 30))
      {
        ammo = ammo - 1;
        //Draw Four Ammo Count Boxes
        Serial.println("39-30");
        u8g2.clearBuffer();
        u8g2.drawBox(2,110,30,17);
        u8g2.drawBox(2,91,30,17);
        u8g2.drawBox(2,72,30,17);
        u8g2.drawBox(2,53,30,17);
        Serial.println(ammo);
        u8g2.setCursor (2, 2);
        u8g2.print(ammo);
        u8g2.sendBuffer();
      }

      //Three bars Displays three bars on ammo display based on ammo count above 20
      if ((ammo <= 29) && (ammo >= 20)) {
        ammo = ammo - 1;
        //Draw Four Ammo Count Boxes
        Serial.println("29-20");
        u8g2.clearBuffer();
        u8g2.drawBox(2,110,30,17);
        u8g2.drawBox(2,91,30,17);
        u8g2.drawBox(2,72,30,17);
        Serial.println(ammo);
        u8g2.setCursor (2, 2);
        u8g2.print(ammo);
        u8g2.sendBuffer();
      }

      //Two bars Displays on bar on ammo display based on ammo count above 10
      if ((ammo <= 19) && (ammo >= 10)) {
        ammo = ammo - 1;
        //Draw Four Ammo Count Boxes
        Serial.println("19-10");
        u8g2.clearBuffer();
        u8g2.drawBox(2,110,30,17);
        u8g2.drawBox(2,91,30,17);
        Serial.println(ammo);
        u8g2.setCursor (2, 2);
        u8g2.print(ammo);
        u8g2.sendBuffer();
      }

      //One bars
      if ((ammo <= 9) && (ammo >= 1)) {
        ammo = ammo - 1;
        //Draw Four Ammo Count Boxes
        u8g2.clearBuffer();
        u8g2.drawBox(2,110,30,17);
        Serial.println(ammo);
        u8g2.setCursor (2, 2);
        u8g2.print(ammo);
        u8g2.sendBuffer();
      }

      //Zero bars
      if ((ammo == 0)) {
        //Draw Four Ammo Count Boxes
        u8g2.clearBuffer();
        Serial.println(ammo);
        u8g2.setCursor (2, 2);
        u8g2.print(ammo);
        u8g2.sendBuffer();
      }
  
      //lastButtonState1 = 0;    //record the lastButtonState
    }
    
    else if(digitalRead(fire) == 1 && lastButtonState1 == 0)    //if button is not pressed, and was pressed last change
    {
      lastButtonState1 = 1;    //record the lastButtonState
    }
    buttonFlag1 = 0;
  }
  //Modechange check loop - checks if mode change button has been pressed
  if((millis() - lastPress2) > debounceTime && buttonFlag2)
  {
    lastPress2 = millis();   //update lastPress                                                     
    if(digitalRead(modechange) == 0 && lastButtonState2 == 1)    //if button is pressed and was released last change
    {
      Serial.println("Mode:");
      //Count2 is used to determin mode 1=kill, 2=stun
      count2 = count2 + 1;
      Serial.println("Count Increase");
      if (count2 == 3) {
        count2 = 1;
      }
      //Play sound for mode change
      if (ammo != 0) {
        myMP3.playFolder(01, 4);
      }
      //Reload Sequence
      if (ammo == 0) {
        Serial.println("Reload");
        myMP3.playFolder(01, 3);
        ammo = 50;
        //Re-Draw Five Ammo Count Boxes for reload
        u8g2.drawBox(2,110,30,17);
        u8g2.drawBox(2,91,30,17);
        u8g2.drawBox(2,72,30,17);
        u8g2.drawBox(2,53,30,17);
        u8g2.drawBox(2,34,30,17);
        u8g2.setCursor (2, 2);
        u8g2.print(ammo);
        u8g2.sendBuffer();
        //Decreases count2 by 1 to prevent mode change on reload
        count2= count2 - 1;
        delay(1000);
      }
      //Serial.println(count2);
      
    
      //lastButtonState2 = 0;    //record the lastButtonState
    }
    
    else if(digitalRead(modechange) == 1 && lastButtonState2 == 0)    //if button is not pressed, and was pressed last change
    {
      lastButtonState2 = 1;    //record the lastButtonState
    }
    buttonFlag2 = 0;
  }
  
}

void ISR_button1()
{
  buttonFlag1 = 1;
}

void ISR_button2()
{
  buttonFlag2 = 1;
}


int SpeedDelay = .1;
void colorWipe(byte red, byte green, byte blue, int SpeedDelay) {
  for(uint16_t i=0; i<NUM_LEDS; i++) {
      setPixel(i, red, green, blue);
      showStrip();
      delay(SpeedDelay);
  }
}
void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;

 #endif
}
