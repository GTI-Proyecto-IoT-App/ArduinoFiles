#include <M5Stack.h>

// paramtros final de carrera
#define PinGPIOInterruptFinalCarrera G2
bool haCalculadoLlenado = false;

// parametros sensor supersonico
const double cmTopeBasura =30.0; // tamaño de la basura
const int valorCalibracion=5; 
const int EchoPin = 17; // receptor sensor
const int TriggerPin = 16;//emisor sensor

void setup() {
  // put your setup code here, to run once:
  M5.begin(true,false,true);
   // set up sensos supersonico
  Serial.begin(115200);
  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT);

  // set up final de carrera
  pinMode(PinGPIOInterruptFinalCarrera, INPUT);
  attachInterrupt(digitalPinToInterrupt(PinGPIOInterruptFinalCarrera),final_carrera_activado,HIGH);

  if(isBasuraAbierta()){
    haCalculadoLlenado = true;
  }
    
}

/**
 * Callback interrupcion final de carrera
 * @author JuanCarlos y Angel 
 */
void final_carrera_activado(){
  haCalculadoLlenado = false; 
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!haCalculadoLlenado && !isBasuraAbierta()){
      calcularLlenado(TriggerPin,EchoPin);
      dormirM5Stack();
   }

  delay(500);
}

/**
 * N,N-> -> R
 * @author JuanCarlos y Angel 
 * Lee distancia del sensor sonico
 * calculado entro el pin de disparo y pin de recepcion
 * y calcula el porcentaje de llenado de la basura
 */
double calcularLlenado(int TriggerPin, int EchoPin) {
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
  Serial.println(porcentajeLlenadoBasura);
  return porcentajeLlenadoBasura;
}


bool isBasuraAbierta(){
  // normalmente cerrado 
  return !digitalRead(PinGPIOInterruptFinalCarrera);
}

// CONFIGURAR PIN GPIO2 A LOW PARA DESPERTAR EL MICRO
void dormirM5Stack(){
  Serial.println("DORMIR");
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_2,0); //1 = High, 0 = Low
  esp_deep_sleep_start();
  
}
