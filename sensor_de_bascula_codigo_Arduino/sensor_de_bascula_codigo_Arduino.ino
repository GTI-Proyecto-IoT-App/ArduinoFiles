#include "M5Stack.h"
#include "HX711.h"
const int DOUT=3;
const int CLK=5;



HX711 balanza;

void setup() {
  Serial.begin(9600);
  balanza.begin(DOUT, CLK);
  Serial.print("Lectura del valor del ADC:  ");
  Serial.println(balanza.read());
  Serial.println("No ponga ningun  objeto sobre la balanza");
  Serial.println("Destarando...");
  Serial.println("...");
  balanza.set_scale(394334); // Establecemos la escala
  balanza.tare(20);  //El peso actual es considerado Tara.
  
  Serial.println("Listo para pesar");  
}

void loop() {
  Serial.print("Peso: ");
  Serial.print(balanza.get_units(20),3);
  Serial.println(" kg");
  delay(500);
}//t394734 == 1kg 
