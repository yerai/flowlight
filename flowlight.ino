#include <Adafruit_NeoPixel.h>
#include <NewPing.h>

// NeoPixel Bottom
#define PIN 5 
#define NUM_LEDS 24 //Number of leds of the ring
Adafruit_NeoPixel strip_bottom = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800); //NeoPixel Strip Definition

// NeoPixel TOP
#define PIN_2 6 
Adafruit_NeoPixel strip_top = Adafruit_NeoPixel(NUM_LEDS, PIN_2, NEO_GRB + NEO_KHZ800); //NeoPixel Strip Definition

// Proximity sensor Color
#define TRIGGER_PIN 11
#define ECHO_PIN 12
#define MAX_DISTANCE 200

NewPing color(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
unsigned int distance = color.ping_cm();

// Proximity sensor Mode
#define TRIGGER_PIN_2 10
#define ECHO_PIN_2 9

NewPing mode(TRIGGER_PIN_2, ECHO_PIN_2, MAX_DISTANCE);
unsigned int movement = mode.ping_cm();


// Colors
int r[] = {255, 255, 255, 128,0,0,0,0,0,127,255,255};
int g[] = {0,128,255,255,255,255,255,128,0,0,0,0};
int b[] = {0,0,0,0,0,128,255,255,255,255,255,127};

// Current color
int pos = 0;

// Current Mode
int current_mode = 1;


// Mode 4
int snake_size = 10;
int snake_tail = 0;
int snake_head = 9;


void setup() {
  // Proximity sensor
  Serial.begin(9600);

  // NeoPixel
  strip_bottom.begin();
  strip_bottom.show();
  strip_top.begin();
  strip_top.show();

}

void loop() {

  Serial.println(current_mode);
  
  checkMode();
  
  if (current_mode == 1){ // Turned Off
    for (int i = 0; i <= NUM_LEDS-1; i++) {
        strip_bottom.setPixelColor(i, 0,0,0);
        strip_bottom.show();
        strip_top.setPixelColor(i, 0,0,0);
        strip_top.show();
     } 
  }

  if (current_mode == 2){ // Circle
    if (color.ping_cm() < 60){
     for (int i = 0; i <= NUM_LEDS-1; i++) {
        pos = getColor();
        strip_bottom.setPixelColor(i, r[pos], g[pos], b[pos]);
        strip_bottom.show();
        strip_top.setPixelColor(i, r[pos], g[pos], b[pos]);
        strip_top.show();
        delay(50);
      }
    }
  }

  if (current_mode == 3){ // Flame
    if (color.ping_cm() < 60){
      int right = 0;
      int left  = 23;
      
      for (int i = 0; i < 12; i++){
         pos = getColor();
         strip_bottom.setPixelColor(right, r[pos], g[pos], b[pos]);
         strip_bottom.setPixelColor(left, r[pos], g[pos], b[pos]);
         strip_bottom.show();
         strip_top.setPixelColor(right, r[pos], g[pos], b[pos]);
         strip_top.setPixelColor(left, r[pos], g[pos], b[pos]);
         strip_top.show();
         right ++;
         left --;
         delay(200);
      }

       for (int i = 0; i < 13; i++){
         strip_bottom.setPixelColor(right, 0, 0, 0);
         strip_bottom.setPixelColor(left, 0, 0, 0);
         strip_bottom.show();
         strip_top.setPixelColor(right, 0, 0, 0);
         strip_top.setPixelColor(left, 0, 0, 0);
         strip_top.show();
         right --;
         left ++;
         delay(200);
      }
    }
  }

  if (current_mode == 4){ // Random
      if (color.ping_cm() < 60){
       for (int i = 0; i <= NUM_LEDS-1; i++) {
          pos = getColor();
          strip_bottom.setPixelColor(random(0, 23), r[pos], g[pos], b[pos]);
          strip_bottom.show();
          pos = getColor();
          strip_top.setPixelColor(random(0, 23), r[pos], g[pos], b[pos]);
          strip_top.show();
          delay(1000);

          if( (mode.ping_cm() <= 10) && (mode.ping_cm() > 0)){
            break;
          }
        }
      }
    }

}

void checkMode(){
  
    if( (mode.ping_cm() <= 10) && (mode.ping_cm() > 0)){
      delay(1500);
      if( (mode.ping_cm() <= 10) && (mode.ping_cm() > 0)){
        current_mode++;
        if (current_mode > 5){
          current_mode = 1;
        }

        for(int i = 0; i< 10; i++){
          for ( int j = 0; j< NUM_LEDS-1; j++){
            strip_bottom.setPixelColor(j, 75,0,130);
            strip_bottom.show();
            strip_top.setPixelColor(j, 75,0,130);
            strip_top.show();
          }
           for ( int j = 0; j< NUM_LEDS-1; j++){
            strip_bottom.setPixelColor(j, 0,0,0);
            strip_bottom.show();
            strip_top.setPixelColor(j, 0,0,0);
            strip_top.show();
          }
          delay(80);
        }
      }
    }
  }

int getColor(){
  distance = color.ping_cm(); // Get distance
  int new_color = pos;
  if (distance <= 60){ // If there's no hand, keep same color
    if      (distance > 0 && distance < 6)   { new_color = 0; }
    else if (distance >= 6 && distance < 12)  { new_color = 1; }
    else if (distance >= 12 && distance < 18) { new_color = 2; }
    else if (distance >= 18 && distance < 24) { new_color = 3; }
    else if (distance >= 24 && distance < 30) { new_color = 4; }
    else if (distance >= 30 && distance < 36) { new_color = 5; }
    else if (distance >= 36 && distance < 42) { new_color = 6; }
    else if (distance >= 42 && distance < 48) { new_color = 7; }
    else if (distance >= 48 && distance < 54) { new_color = 8; }
    else                                      { /*Nothing */ }
  }
  return new_color;
}
