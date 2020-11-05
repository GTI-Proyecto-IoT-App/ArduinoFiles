#include <M5Stack.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "basuraInteligente.h"

// paramtros final de carrera
#define PinGPIOInterruptFinalCarrera G2
bool haCalculadoLlenado = false;

// parametros sensor supersonico
const double cmTopeBasura =30.0; // tamaño de la basura
//const int valorCalibracion=5; 
const int EchoPin = 17; // receptor sensor
const int TriggerPin = 16;//emisor sensor



//inicializacion de la basura

BasuraInteligente basura;

void setup() {

  // put your setup code here, to run once:
  M5.begin(true,false,true);
  
  //set up sensos supersonico
  Serial.begin(115200);
  
  // set up BASURA

  ContenedorInteligente listaContenedores[4];
  listaContenedores[0] = ContenedorInteligente(EchoPin,TriggerPin,12,cmTopeBasura,20,0.5,"Vidrio"); 
  listaContenedores[1] = ContenedorInteligente(EchoPin,TriggerPin,12,cmTopeBasura,20,0.5,"Plastico"); 
  listaContenedores[2] = ContenedorInteligente(EchoPin,TriggerPin,12,cmTopeBasura,20,0.5,"Organico"); 
  listaContenedores[3] = ContenedorInteligente(EchoPin,TriggerPin,12,cmTopeBasura,20,0.5,"Papel"); 


  BasuraInteligente basura(&listaContenedores[0]);

  
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
  double porcentajeLlenado;
  
  // put your main code here, to run repeatedly:
  
  if(!haCalculadoLlenado && !isBasuraAbierta()){
      
     // porcentajeLlenado = calcularLlenado(TriggerPin,EchoPin);
      //enviarInfoUart(basura.calcular());
      dormirM5Stack();
   }

  delay(500);
}

bool isBasuraAbierta(){
  // normalmente cerrado 
  return !digitalRead(PinGPIOInterruptFinalCarrera);
}

// CONFIGURAR PIN GPIO2 A LOW PARA DESPERTAR EL MICRO
void dormirM5Stack(){
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_2,0); //1 = High, 0 = Low
  esp_deep_sleep_start();
  
}
