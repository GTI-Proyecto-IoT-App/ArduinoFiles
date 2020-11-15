#include <M5Stack.h>
#include <WiFi.h>
#include "basuraInteligente.h"

// para obtener la hora de un servidor web
#include "time.h"
const char* ntpServer = "hora.rediris.es";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

// paramtros final de carrera
#define PinGPIOInterruptFinalCarrera G2
bool haCalculadoLlenado = false;

// parametros sensor supersonico
const double cmTopeBasura =30.0; // tamaño de la basura
const double cmDistanciaSensorDeBasura = 2.0; // tamaño de la basura
//const int valorCalibracion=5; 
const uint8_t EchoPin = 17; // receptor sensor
const uint8_t TriggerPin = 16;//emisor sensor

const char* WifiSSID = "TP-LINK_6FEE";
const char* wifiPass = "90821950";

//inicializacion de la basura
BasuraInteligente basura;

void setup() {

  // put your setup code here, to run once:
  M5.begin(true,false,true);
  
  //set up sensos supersonico
  Serial.begin(115200);
  
  // set up BASURA

//  ContenedorInteligente listaContenedores[4];
//  listaContenedores[0] = ContenedorInteligente(17,16,12,30,20,0.5,"Vidrio"); 
//  listaContenedores[1] = ContenedorInteligente(EchoPin,TriggerPin,12,cmTopeBasura,20,0.5,"Plastico"); 
//  listaContenedores[2] = ContenedorInteligente(EchoPin,TriggerPin,12,cmTopeBasura,20,0.5,"Organico"); 
//  listaContenedores[3] = ContenedorInteligente(EchoPin,TriggerPin,12,cmTopeBasura,20,0.5,"Papel"); 

  //BasuraInteligente basura(id,&listaContenedores[0]);

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

     // porcentajeLlenado = calcularLlenado(TriggerPin,EchoPin);
    Serial.println(calcular());
     haCalculadoLlenado = true;
     //enviarInfoUart(basura.calcular());
     dormirM5Stack();
   }else{
    Serial.println("Esta calculando o esta abierta");
   }

  delay(500);
}

/**
 * Calcular llenado y peso basura
 */
StaticJsonDocument<100> calcularBasura(){
  
 StaticJsonDocument<100> doc;
  const size_t CAPACITY = JSON_ARRAY_SIZE(4);
  StaticJsonDocument<CAPACITY> medidas;
  
  JsonArray array = medidas.to<JsonArray>();
  

   doc["id"] = String(WiFi.macAddress()+"%basura");
   doc["fecha"] = "1212121212121";
   
   medidas.add((*this).listaContenedores[1].calcular()); 
   medidas.add((*this).listaContenedores[1].calcular());
   medidas.add((*this).listaContenedores[2].calcular());
   medidas.add((*this).listaContenedores[3].calcular());

   doc["medidas"]= medidas;
 
  String strJson;

  serializeJson(doc, strJson);
  
  return strJson;
};



/**
 * ->calcularLlenado -> R
 * @author JuanCarlos y Angel 
 * Lee distancia del sensor sonico
 * calculado entro el pin de disparo y pin de recepcion
 * y calcula el porcentaje de llenado de la basura
 */
double calcularLlenado() {
  
  int valorCalibracion = 5;

  double duracion,porcentajeLlenadoBasura,distanciaCm;
  //Serial.println((*this).triggerPinUltrasonico);
  digitalWrite(TriggerPin/*(*this).triggerPinUltrasonico*/, LOW); //nos aseguramos señal baja
  delayMicroseconds(4);

  digitalWrite(TriggerPin/*(*this).triggerPinUltrasonico*/, HIGH); //generamos pulso de 10us
  
  digitalWrite(TriggerPin/*(*this).triggerPinUltrasonico*/, LOW);

  duracion = pulseIn(EchoPin/*(*this).echoPinUltrasonico*/, HIGH); //medimos el tiempo pulso

  distanciaCm = duracion * 10.0 / 292.0 / 2.0; //convertimos a distancia

  distanciaCm = distanciaCm - cmDistanciaSensorDeBasura;//(*this).distanciaContenedor ;

  porcentajeLlenadoBasura = 100.0 - (distanciaCm/cmTopeBasura/*(*this).profundidadContenedor*/ )*100.0;//conversión a %


  //calibracion
  if(porcentajeLlenadoBasura > (100-valorCalibracion)){
    porcentajeLlenadoBasura=100.0;
  }else if(porcentajeLlenadoBasura < 0){
    porcentajeLlenadoBasura=0.0;
  }
  
  return porcentajeLlenadoBasura;
}

// ---------------------------------------------------
//    calcular --> JSON
// calcula uno de los cuatro contenedores
// ---------------------------------------------------

StaticJsonDocument<100> calcularContenedor(String tipo){
  
  StaticJsonDocument<100> doc1;
  
  
  doc1["tipo"] = tipo;//(*this).tipoContenedor;
  //doc["peso"] = 

  doc1["volumen"] = calcularLlenado();

  

  return doc1;
  
};

bool isBasuraAbierta(){
  // normalmente cerrado 
  return !digitalRead(PinGPIOInterruptFinalCarrera);
}

// CONFIGURAR PIN GPIO2 A LOW PARA DESPERTAR EL MICRO
void dormirM5Stack(){
  Serial.println("DORMIR");
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_2,0); //1 = High, 0 = Low Despertar cuando llegue 0
  esp_deep_sleep_start();
  
}
