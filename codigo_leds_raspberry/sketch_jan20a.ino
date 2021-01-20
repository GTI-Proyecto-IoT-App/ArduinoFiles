
#include <stdlib.h>

// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include <Adafruit_NeoPixel.h>
#ifdef _AVR_
#include <avr/power.h>
#include math.h
#endif
// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN 2
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 7
// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int delayval = 300; // delay for half a second

int rFondo = 255;
int bFondo = 255;
int gFondo = 255;




void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (_AVR_ATtiny85_)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code
  pixels.begin(); // This initializes the NeoPixel library.

  
   Serial.begin(115200);
  
  
}
void loop() {

  if (Serial.available() > 0) {
      String command = Serial.readString(); 
      double llenado = atof(command.c_str());
      pintarLeds(llenado);
   }
}


void pintarLeds(double porcentajeLeds){

  if(porcentajeLeds>400){
    porcentajeLeds = 400;
  }

  int red,blue,green;
  
  if(porcentajeLeds >= 285){
    // esta casi al tope color rojo
    red = 255;
    blue = 0;
    green = 0;
  }else if(porcentajeLeds < 285 && porcentajeLeds >= 171){
    // a la mitad, amarillo
    red = 255;
    blue = 0;
    green = 255;
  }else{
    // poco lleno, verde
    red = 0;
    blue = 0;
    green = 255;
  }
  
  int ledsEncendidos = floor((porcentajeLeds * NUMPIXELS)/400);
  double ultimoLedBrillo = (porcentajeLeds * NUMPIXELS)/400; // el ultimo led se encendera en verde pero con menos brillo 0 -> nada de brillo, 1-> tope de brillo
  ultimoLedBrillo = ultimoLedBrillo - (int)ultimoLedBrillo; // sacamos el resto
  
  //pintar fondo
  for(int i = 0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(rFondo/3,gFondo/3,bFondo/3)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    //delay(delayval); // Delay for a period of time (in milliseconds).
  }

  //  pintar los leds
  int cont = 0;
  for(cont=0;cont<ledsEncendidos;cont++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(cont, pixels.Color(red,green,blue)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(delayval); // Delay for a period of time (in milliseconds).
  }

  

  if(ultimoLedBrillo > 0){
      double bright = 100*ultimoLedBrillo/255;
     pixels.setPixelColor(cont,pixels.Color(bright*red,bright*green,bright*blue));
     pixels.show();
  }
 
  
}
