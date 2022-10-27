//need to install fastled library

#include <FastLED.h>

#define PIN 7
#define NUM 10

#define ORANGE_Y CHSV(23, 255, 255)
#define emtpy CHSV(0, 0, 0)

CRGB leds [NUM];
void setup() {

FastLED.addLeds<WS2812,PIN,GRB>(leds,NUM);
}

void turn(char *direction_)
{

  if(strcmp(direction_, "right") == 0){
    for(int i = 0;i < NUM; i++){
      leds[i] = ORANGE_Y;
      FastLED.show();
      delay(50); 
    }

    for(int i = 0;i < NUM; i++){
      leds[i] = emtpy;
      FastLED.show();
      delay(50); 
    }
  }

  else if(strcmp(direction_, "left") == 0){
    for(int i = NUM;i > 0; i--){
      leds[i] = ORANGE_Y;
      FastLED.show();
      delay(50); 
    }

    for(int i = NUM;i > 0; i--){
      leds[i] = emtpy;
      FastLED.show();
      delay(50); 
    }
  }

  else if(strcmp(direction_, "back") == 0){
    for(int i=0;i<255;i++){
      fill_palette (leds, NUM, 0, 5, LavaColors_p, i, LINEARBLEND); //(名稱, 數量, 起始顏色序號ㄝ, 色差, palette name, 亮度, 色彩過度)
      FastLED.show();
    }
    
    for(int i=255;i>0;i--){
      fill_palette (leds, NUM, 0, 5, LavaColors_p, i, LINEARBLEND);
      FastLED.show();
    }
  }
}

void loop() {
  turn("left");

  turn("right");

  turn("back");
  }
