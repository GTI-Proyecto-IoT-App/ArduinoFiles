#include <M5Stack.h>
#include <WiFi.h>
#include "basuraInteligente.h"
#include <PubSubClient.h>
#include "HX711.h"

// paramtros final de carrera
#define PinGPIOInterruptFinalCarrera G2
bool haCalculadoLlenado = false;

// parametros sensor supersonico
const double cmTopeBasura =30.0; // tamaño de la basura
const double cmDistanciaSensorDeBasura = 2.0; // tamaño de la basura
//const int valorCalibracion=5; 
const uint8_t EchoPin = 17; // receptor sensor
const uint8_t TriggerPin = 16;//emisor sensor

const char* WifiSSID = "MiFibra-EA9E";//"TP-Link_6FEE";
const char* wifiPass = "bggtPfp9";

// MQTT
// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "mqtt.eclipse.org";
const char* rootMqttClient = "proyectoGTI2A/dispositivo/";
WiFiClient espClient;
PubSubClient client(espClient);

// bascula
const int DOUT=3;
const int CLK=5;

HX711 bascula;

//inicializacion de la basura
BasuraInteligente basura;

void setup() {

  // put your setup code here, to run once:
  M5.begin(true,false,true);
  Serial.begin(9600);

   // set up wifi y mqtt
  setup_wifi();
  client.setServer(mqtt_server, 1883);


  
  // set up BASURA

//  ContenedorInteligente listaContenedores[4];
//  listaContenedores[0] = ContenedorInteligente(17,16,12,30,20,0.5,"Vidrio"); 
//  listaContenedores[1] = ContenedorInteligente(EchoPin,TriggerPin,12,cmTopeBasura,20,0.5,"Plastico"); 
//  listaContenedores[2] = ContenedorInteligente(EchoPin,TriggerPin,12,cmTopeBasura,20,0.5,"Organico"); 
//  listaContenedores[3] = ContenedorInteligente(EchoPin,TriggerPin,12,cmTopeBasura,20,0.5,"Papel"); 

  //BasuraInteligente basura(id,&listaContenedores[0]);

  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT);

  // balanza
  Serial.println("Bascula");
    bascula.begin(DOUT, CLK);
    Serial.println("Bascula 2");
    bascula.read();
    Serial.println("Bascula 2");
    bascula.set_scale(439430.25); // Establecemos la escala
    Serial.println("Bascula 4");
    bascula.tare(20);  //El peso actual es considerado Tara.
  Serial.println("END Bascula");

  Serial.println("FINAL DE CARRERA");
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

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.printf("Connecting to %s ", WifiSSID);
  WiFi.begin(WifiSSID, wifiPass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/**
 * public un mensaje al servidor mqtt indicandole el topic
 */
void publicarMqttAlTopic(String topic,String payload){
  
  char charBufPayload[payload.length() + 1];
  payload.toCharArray(charBufPayload,payload.length() + 1);

  char charBufTopic[String(rootMqttClient+topic).length() + 1];
  String(rootMqttClient+topic).toCharArray(charBufTopic,String(rootMqttClient+topic).length() + 1);
  Serial.println("Envio datos al mqtt");
  Serial.println(charBufTopic);
  Serial.println(charBufPayload);
  client.publish(charBufTopic, charBufPayload);
}

void loop() {
  Serial.println("LOOP");
  // put your main code here, to run repeatedly:

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  if(!haCalculadoLlenado && !isBasuraAbierta()){

     // porcentajeLlenado = calcularLlenado(TriggerPin,EchoPin);
     String id = WiFi.macAddress()+"%basura";
      publicarMqttAlTopic(id+"/mesuras",calcularBasura(id));
      haCalculadoLlenado = true;
     //enviarInfoUart(basura.calcular());

     delay(1000);
     dormirM5Stack();
   }else{
    Serial.println("Esta calculando o esta abierta");
   }

  delay(500);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("2750508476e341d6b8413239039ae8ac")) {
      Serial.println("connected");
     
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


// ---------------------------------------------------
//    calcular --> JSON
// calcula uno de los cuatro contenedores
// ---------------------------------------------------

String calcularContenedor(String tipo){
  
  String res = "";
  
  
  res = String(res + "\"tipo\":"+ "\""+tipo+"\",");
  res = String(res + "\"volumen\":"+calcularLlenado()+",");
  res = String(res + "\"peso\":"+calcularPeso()); 

  return res;
  
};

/**
 * Calcular llenado y peso basura
 */
String calcularBasura(String id){

  String jsonMessage = "{\"id\":";

  

   jsonMessage = String(jsonMessage + "\"" + id + "\",");
    jsonMessage = String(jsonMessage+"\"medidas\": [");
    jsonMessage = String(jsonMessage+"{"+ calcularContenedor("vidrio")+ "},");
    jsonMessage = String(jsonMessage+"{"+ calcularContenedor("papel")+ "},");
    jsonMessage = String(jsonMessage+"{"+ calcularContenedor("organico")+ "},");
    jsonMessage = String(jsonMessage+"{"+ calcularContenedor("plastico")+ "}");
//  JsonObject  medidasObjVid = medidas.createNestedObject();
//  JsonObject  medidasObjVid = medidas.createNestedObject();
//  JsonObject  medidasObjVid = medidas.createNestedObject();
//  JsonObject  medidasObjVid = medidas.createNestedObject();
    jsonMessage = String(jsonMessage+"]}");
//   medidasObjVid["vidrio"] = calcularContenedor();
//   medidasObj["organico"] = calcularContenedor();
//   medidasObj["papel"] = calcularContenedor();
//   medidasObj["plastico"] = calcularContenedor();
   
   

  return jsonMessage;
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
  Serial.println("Distancia real");
   Serial.println(distanciaCm);
  distanciaCm = distanciaCm - cmDistanciaSensorDeBasura;//(*this).distanciaContenedor ;
   Serial.println("Distancia menos la del sensora a la basura");
  Serial.println(distanciaCm);
  porcentajeLlenadoBasura = 100.0 - (distanciaCm/cmTopeBasura/*(*this).profundidadContenedor*/ )*100.0;//conversión a %
   Serial.println("Porcentaje");
  Serial.println(porcentajeLlenadoBasura);

  //calibracion
  if(porcentajeLlenadoBasura > (100-valorCalibracion)){
    porcentajeLlenadoBasura=100.0;
  }else if(porcentajeLlenadoBasura < 0){
    porcentajeLlenadoBasura=0.0;
  }
  Serial.println(porcentajeLlenadoBasura);
  delay(1000);
  return porcentajeLlenadoBasura;
}


/**
 * Devuelve el valor actual de la bascula en KG
 */
double calcularPeso() {
  return bascula.get_units(20);
}

bool isBasuraAbierta(){
  // normalmente cerrado 
  return !digitalRead(PinGPIOInterruptFinalCarrera);
}

// CONFIGURAR PIN GPIO2 A LOW PARA DESPERTAR EL MICRO
void dormirM5Stack(){
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_2,0); //1 = High, 0 = Low Despertar cuando llegue 0
  esp_deep_sleep_start();
  
}
