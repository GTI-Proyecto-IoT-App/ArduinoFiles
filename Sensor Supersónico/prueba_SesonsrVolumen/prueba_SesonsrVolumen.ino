#include <M5Stack.h>
const double cmTopeBasura =30.0;
const int valorCalibracion=5;

const int EchoPin = 17;
const int TriggerPin = 16;
 

void setup() {
  Serial.begin(115200);
  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT);
}
void loop() {
  Serial.print("Porcetaje llenado basura: ");
  Serial.print(distancia(TriggerPin, EchoPin));
  Serial.println("%");
  delay(1000);
  }
double distancia(int TriggerPin, int EchoPin) {
  double duracion, distanciaCm,porcentajeLlenadoBasura;
  digitalWrite(TriggerPin, LOW); //nos aseguramos señal baja
  delayMicroseconds(4);
  digitalWrite(TriggerPin, HIGH); //generamos pulso de 10us
  delayMicroseconds(10);
  digitalWrite(TriggerPin, LOW);
  duracion = pulseIn(EchoPin, HIGH); //medimos el tiempo pulso
  distanciaCm = duracion * 10.0 / 292.0 / 2.0; //convertimos a distancia

  porcentajeLlenadoBasura = 100.0 - ((distanciaCm/cmTopeBasura )*100.0);//conversión a %

  //calibracion
  if(porcentajeLlenadoBasura > (100-valorCalibracion)){
    porcentajeLlenadoBasura=100.0;
  }else if(porcentajeLlenadoBasura < 0){
    porcentajeLlenadoBasura=0.0;
  }
  return porcentajeLlenadoBasura;
}
