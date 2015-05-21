#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
 
#define PIN 6
#define STRIP_LENGTH 60
 
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIP_LENGTH, PIN, NEO_GRB + NEO_KHZ800);

float percentage = 1.0; //percentage for progress bar
 
void setup() {
  strip.setBrightness(25);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(9600);
}
 
void loop() {
  //float percentage;
  if(Serial.available()){
    String readIn = Serial.readString();
    char carray[readIn.length() + 1];
    readIn.toCharArray(carray, sizeof(carray));
   
    float activity = atof(carray); //Float reading in value from serial (Nessie API)
                                   //Ones place contains the switch value of the type of activity
                                   //Percentage of budget used in decimals
    int caseValue = (int)activity; //Parsed switch value (0 for spend 1 for deposit)
    //println(activity);
    if (caseValue == 0){
      withdrawal(50);
      percentage = activity;
      Serial.print("Withdrawal: ");
      Serial.println(percentage);
      
    }
    if (caseValue == 1){
      deposit(200);
      Serial.println("Deposit");
    }

}
progressBar(1);
}
 
//method containing
void strandtest(){
//  // Some example procedures showing how to display to the pixels:
  colorWipe(strip.Color(255, 0, 0), 50); // Red
  colorWipe(strip.Color(0, 255, 0), 50); // Green
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
//  // Send a theater pixel chase in...
  theaterChase(strip.Color(127, 127, 127), 50); // White
  theaterChase(strip.Color(127,   0,   0), 50); // Red
  theaterChase(strip.Color(  0,   0, 127), 50); // Blue
 
  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);
  rainbowCycle(20);
}
 
//modified rainbow cycle as progress bar
void progressBar(uint8_t wait) {
  uint16_t i, j;
  int progress = percentage*STRIP_LENGTH;
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< progress; i++) { //sets the white 
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    for(i=progress; i< STRIP_LENGTH; i++) {
      strip.setPixelColor(i, 255, 255, 255);
    }
    strip.show();
    delay(wait);
  }
}
 
void percentageFill(float percentage, uint32_t main, uint32_t second, uint8_t wait) {
  int filled = (int)(percentage * strip.numPixels());
  uint16_t i;
  for(i = 0; i<filled; i++) {
    strip.setPixelColor(i, main);
    strip.show();
    delay(wait);
  }
  for(i; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, second);
    strip.show();
    delay(wait);  
  }
}
 
 
//withdrawal
void withdrawal(uint8_t wait) {
  colorWipe(strip.Color(255, 0, 0), 50); // Red
}
 
//deposit made
void deposit(uint8_t wait) {
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
   
}
 
//over budget
void over(uint8_t wait) {
 
}
 
 
 
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}
 
void rainbow(uint8_t wait) {
  uint16_t i, j;
 
  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
 
// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
 
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
 
 
 
//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}
 
//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}
 
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
 
//random animations made during hack
 
// Light up the strip starting from the middle
void middleFill(uint32_t main, uint32_t second, uint32_t third, uint32_t fourth, uint8_t wait) {
  int turn = 0;
  uint32_t color;
  while(turn < 2) {
    for(uint16_t i=0; i<(strip.numPixels()/2); i++) { // start from the middle, lighting an LED on each side
      if (turn) {
        color = third;
      } else {
        color = main;
      }
      strip.setPixelColor(strip.numPixels()/2 + i, color);
      strip.setPixelColor(strip.numPixels()/2 - i, color);
      strip.show();
      delay(wait);
    }
 
    for(uint16_t i=0; i<(strip.numPixels()/2); i++) { // reverse
      if (turn) {
        color = fourth;
      } else {
        color = second;
      }
      strip.setPixelColor(i, color);
      strip.setPixelColor(strip.numPixels() - i, color);
      strip.show();
      delay(wait);
    }
    turn++;
  }
}
 
void cycle(uint32_t main, uint32_t second, uint8_t wait, int cycles, int windowSize) {
  int start = 0;
  int steps = strip.numPixels() * cycles;
  int current, backward;
  while(start < steps) {
    current = start % strip.numPixels();
    strip.setPixelColor(current, main);
    backward = current - windowSize;
    if (backward < 0) {
     strip.setPixelColor(strip.numPixels()+backward, second);
    } else {
      strip.setPixelColor(backward%strip.numPixels(), second);
    }
    strip.show();
    delay(wait);
    start++;
  }
}
 
void doublecycle(uint32_t main, uint32_t second, uint8_t wait, int cycles, int windowSize) {
  int start = 0;
  int steps = strip.numPixels() * cycles;
  int current, current2, backward, backward2;
  while(start < steps) {
    current = start % strip.numPixels();
    current2 = (start + strip.numPixels()/2) % strip.numPixels();
    strip.setPixelColor(current, main);
    strip.setPixelColor(current2, main);
    backward = current - windowSize;
    backward2 = current2 - windowSize;
    if (backward < 0) {
     strip.setPixelColor(strip.numPixels()+backward, second);
    } else {
      strip.setPixelColor(backward%strip.numPixels(), second);
    }
    if (backward2 < 0) {
     strip.setPixelColor(strip.numPixels()+backward2, second);
    } else {
      strip.setPixelColor(backward2%strip.numPixels(), second);
    }
    strip.show();
    delay(wait);
    start++;
  }
}
