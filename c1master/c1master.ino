/* Arduino Code powering NeoPixel strip */
/* Modified from Adafruit_NeoPixel/examples/strandtest */
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#define PIN 7 // Defines the pin in the arduino being used
#define STRIP_LENGTH 10 // Defines the number of pixels in the strip
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//  NEO_KHZ800 800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//  NEO_KHZ400 400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//  NEO_GRB Pixels are wired for GRB bitstream (most NeoPixel products)
//  NEO_RGB Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIP_LENGTH, PIN, NEO_GRB + NEO_KHZ800);
float percentage; // Percentage variable for progress bar

void setup() {
    percentage = 1.0;
    strip.setBrightness(25);
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
    Serial.begin(9600); // Initialize Serial to read Nessie API input from Nessie2.7.py
}

void loop() {
    if(Serial.available()){ // Check if any input has been recieved
        String readIn = Serial.readString(); // Store input string
        char carray[readIn.length() + 1]; // Init char array
        readIn.toCharArray(carray, sizeof(carray)); // Parse input string as C style string
        float activity = atof(carray); // Float reading in value from serial (Nessie API)
        // Ones place contains the switch value of the type of activity
        // Percentage of budget used in decimals
        int caseValue = (int)activity; // Parsed switch value (0 for spend, 1 for deposit, 2 for nearing threshold limit)
        if (caseValue == 0){
            withdrawal(500); // Activate the withdrawal animation
            percentage = activity; // Update the percentage
            Serial.print("withdrawalal: ");
            Serial.println(percentage); // Serial prints for debugging and notification purposes
        }
        if (caseValue == 1){
            deposit(500); // Activate the deposit animation
            Serial.println("Deposit"); // Serial print for debugging and notification purposes
        }
        if (caseValue == 2){
            while (true) {
                // Horrible way of making sure that the threshold animation cycles continuously
                // This *must* be modified to later to ensure the program continues to check for incoming activity from the API
                // (Hackathon time constraints can lead to interesting work arounds)
                threshold(100, 100);
                Serial.println("Threshold"); //Serial prints for debugging and notification purposes
            }
        }
    }
    progressBar(200); //The default animation is the progress bar, which should only be interupted during account activity
}

// Runs the animation for the progress bar which is displayed by default.
// It is modified from the rainbowCycle(uint8_t wait) method to include white pixels indiciating the percentage
// As well as added checks to interupt the method if serial input is available (keeping the notification interuptions timely)
void progressBar(uint8_t wait) {
    uint16_t i, j;
    int progress = percentage*STRIP_LENGTH; //Determines the number of NeoPixels that should be displayed as progress
    for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
        if (Serial.available()) return; // Interupts animation if there is a notification to be shown
        for(i=0; i< progress; i++) strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255)); // Sets the rainbow portion of the NeoPixel strip
        for(i=progress; i< STRIP_LENGTH; i++) strip.setPixelColor(i, 150, 150, 150); // Sets the white portion fo the NeoPixel strip
        strip.show(); // Displays the strip
        delay(wait); // Controls how quickly the LEDs on the strip move through each subsequent color
    }
}

// Original tester method created for clearly displaying what pixels are filled and unfiled.
// Used early in the Hackathon for debugging purposes and is not used in the main loop()
void percentageFill(float percentage, uint32_t main, uint32_t second, uint8_t wait) {
    int filled = (int)(percentage * strip.numPixels()); //Determines the number of NeoPixels that should be displayed as progress
    uint16_t i;
    for(i = 0; i<filled; i++) {
        strip.setPixelColor(i, main); // Sets the progress NeoPixels to the color variable 'main'
        strip.show(); // Shows the new update (but does not update the rest of the pixels)
        delay(wait); // This would need to be fixed before using this as an actual method by moving the strip.show() and the delay to after the 2nd loop
    }
    for(i; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, second); //sets the progress NeoPixels to the color variable 'main'
        strip.show();
        delay(wait);
    }
    //This would be a better place to put strip.show() and delay(wait) rather than within each loop if this were to be used as an actual display rather than debugging
}

// Withdrawalal and Deposit tester methods
// void withdrawalal(uint8_t wait) { colorWipe(strip.Color(100, 0, 0), wait); } // Red
// void deposit(uint8_t wait) { colorWipe(strip.Color(0, 50, 100), wait); } // Blue

// Final threshold animation
// Needs to be modified to have it run for a set amount of time to make sure the animation runs without needing to used while(true) in the main loop()
// preventing it from returning to the progress bar
void threshold(int color, uint8_t wait) {
    int origColor = color;
    while(color > 0) {
        // dim slowly
        for(int i = 0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, strip.Color(color, 0, 0));
        }
        strip.show();
        color -= 10;
        delay(wait);
    }
    // dim to white
    for(int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
    strip.show();
    delay(wait*2);
    // get brighter again
    while (color < origColor) {
        for(int i = 0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, strip.Color(color, 0, 0));
        }
        strip.show();
        color += 10;
        delay(wait);
    }
}

// Final withdrawal animation
void withdrawal(uint8_t wait) {
    // set the colors
    int color = 50;
    int divide = 5;
    int first = color * 1.7;
    int second = color * 2;
    int third = color * 2.5;
    for(int i = 0; i < strip.numPixels(); i+=5) {
        strip.setPixelColor(i+1, strip.Color(color, 0, 0));
        strip.setPixelColor(i+2, strip.Color(color, 0, 0));
        strip.setPixelColor(i+3, strip.Color(color, color/divide, 0));
        strip.setPixelColor(i+4, strip.Color(color, 0, 0));
        strip.setPixelColor(i+5, strip.Color(color, color/divide, 0));
        strip.setPixelColor(i, strip.Color(color, color/divide, 0));
    }
    strip.show();
    // make it wavy
    for(uint16_t i = 0; i<(strip.numPixels()/2); i+=2) {
        strip.setPixelColor(strip.numPixels()/2 + i + 2, strip.Color(first, 0, 0 ));
        strip.setPixelColor(strip.numPixels()/2 + i+1, strip.Color(second, 0, 0));
        strip.setPixelColor(strip.numPixels()/2 + i, strip.Color(third, third/divide,0));
        strip.setPixelColor(strip.numPixels()/2 - i, strip.Color(third, 0, 0));
        strip.setPixelColor(strip.numPixels()/2 - i-1, strip.Color(second, second/divide, 0));
        strip.setPixelColor(strip.numPixels()/2 - i - 2, strip.Color(first, first/divide, 0));
        strip.show();
        delay(wait);
    }
    for(uint16_t i = 0; i<(strip.numPixels()/2); i+=2) {
        strip.setPixelColor(i + 2, strip.Color(color, 0, 0));
        strip.setPixelColor(i+1, strip.Color(color, 0, 0));
        strip.setPixelColor(i, strip.Color(color, color/divide, 0));
        strip.setPixelColor(strip.numPixels() - i, strip.Color(color, 0, 0));
        strip.setPixelColor(strip.numPixels() - i-1, strip.Color(color, color/divide,0));
        strip.setPixelColor(strip.numPixels()- i - 2, strip.Color(color, color/divide, 0));
        strip.show();
        delay(wait);
    }
}

// Final deposit animation
void deposit(uint8_t wait) {
    // set the colors
    int color = 50;
    int first = color * 1.7;
    int second = color * 2;
    int third = color * 2.5;
    for(int i = 0; i < strip.numPixels(); i+=5) {
        strip.setPixelColor(i+1, strip.Color(0, 0, color));
        strip.setPixelColor(i+2, strip.Color(0, 0, color));
        strip.setPixelColor(i+3, strip.Color(0, color, color));
        strip.setPixelColor(i+4, strip.Color(0, 0, color));
        strip.setPixelColor(i+5, strip.Color(0, color, color));
        strip.setPixelColor(i, strip.Color(0, color, color));
    }
    strip.show();
    // make it wavy
    for(uint16_t i = 0; i<(strip.numPixels()/2); i+=2) {
        strip.setPixelColor(strip.numPixels()/2 + i + 2, strip.Color(0, 0, first));
        strip.setPixelColor(strip.numPixels()/2 + i+1, strip.Color(0, 0, second));
        strip.setPixelColor(strip.numPixels()/2 + i, strip.Color(0, third, third));
        strip.setPixelColor(strip.numPixels()/2 - i, strip.Color(0, 0, third));
        strip.setPixelColor(strip.numPixels()/2 - i-1, strip.Color(0, second, second));
        strip.setPixelColor(strip.numPixels()/2 - i - 2, strip.Color(0, first, first));
        strip.show();
        delay(wait);
    }
    for(uint16_t i = 0; i<(strip.numPixels()/2); i+=2) {
        strip.setPixelColor(i + 2, strip.Color(0, 0, color));
        strip.setPixelColor(i+1, strip.Color(0, 0, color));
        strip.setPixelColor(i, strip.Color(0, color, color));
        strip.setPixelColor(strip.numPixels() - i, strip.Color(0, 0, color));
        strip.setPixelColor(strip.numPixels() - i-1, strip.Color(0, color, color));
        strip.setPixelColor(strip.numPixels()- i - 2, strip.Color(0, color, color));
        strip.show();
        delay(wait);
    }
}

// Unused method from trying to add more motion to the white part of the progress bar
// Ultimately scrapped due to time constraints and color theory issues
uint32_t WhiteWheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85) {
        return strip.Color(255 - WheelPos * 3, 255, WheelPos * 3);
        } else if(WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(255, WheelPos * 3, 255 - WheelPos * 3);
        } else {
        WheelPos -= 170;
        return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 255);
    }
}
/* ------------------------------------- END OF CODE WRITTEN DURING HACK ------------------------------------ */
/* The following methods are from Adafruit's strandtest file in their NeoPixel library */
// Method for ensuring the LEDs are working properly (not modified from Adafruit Neopixel Library)
void strandtest(){
    // // Some example procedures showing how to display to the pixels:
    colorWipe(strip.Color(255, 0, 0), 50); // Red
    colorWipe(strip.Color(0, 255, 0), 50); // Green
    colorWipe(strip.Color(0, 0, 255), 50); // Blue
    // // Send a theater pixel chase in...
    theaterChase(strip.Color(127, 127, 127), 50); // White
    theaterChase(strip.Color(127,  0,  0), 50); // Red
    theaterChase(strip.Color( 0,  0, 127), 50); // Blue
    rainbow(20);
    rainbowCycle(20);
    theaterChaseRainbow(50);
    rainbowCycle(20);
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
    for (int j=0; j<10; j++) { //do 10 cycles of chasing
        for (int q=0; q < 3; q++) {
            for (int i=0; i < strip.numPixels(); i=i+3) {
                strip.setPixelColor(i+q, c); //turn every third pixel on
            }
            strip.show();
            delay(wait);
            for (int i=0; i < strip.numPixels(); i=i+3) {
                strip.setPixelColor(i+q, 0); //turn every third pixel off
            }
        }
    }
}
//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
    for (int j=0; j < 256; j++) { // cycle all 256 colors in the wheel
        for (int q=0; q < 3; q++) {
            for (int i=0; i < strip.numPixels(); i=i+3) {
                strip.setPixelColor(i+q, Wheel( (i+j) % 255)); //turn every third pixel on
            }
            strip.show();
            delay(wait);
            for (int i=0; i < strip.numPixels(); i=i+3) {
                strip.setPixelColor(i+q, 0); //turn every third pixel off
            }
        }
    }
}
