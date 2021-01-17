#include <WiFiManager.h>
#include <PubSubClient.h>
#include "HX711.h"

// parametros final de carrera
#define PinGPIOInterruptFinalCarrera 2
bool haCalculadoLlenado = false;

// parametros sensor supersonico
const double cmTopeBasura =30.0; // tamaño de la basura
const double cmDistanciaSensorDeBasura = 2.0; // tamaño de la basura

const uint8_t EchoPin = 17; // receptor sensor
const uint8_t TriggerPin = 16;//emisor sensor

WiFiManager wifiManager;
//-----------------WIFI-RouterPortatil-------------
//const char* WifiSSID = "TP-Link_6FEE";
//const char* wifiPass = "90821950";

//------------------WIFI_CASA-------------
//const char* WifiSSID = "MiFibra-EA9E";
//const char* wifiPass = "bggtPfp9";

// MQTT
const char* mqtt_server = "broker.hivemq.com";
const char* rootMqttClient = "proyectoGTI2A/dispositivo/";
WiFiClient espClient;
PubSubClient client(espClient);

HX711 bascula;
//bascula
const int DOUT=4;
const int CLK=5;



void setup() {

  // put your setup code here, to run once:
  Serial.begin(true,false,true);
  Serial.begin(115200);
 
  //set up wifi y mqtt
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  //config supersonico
  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT);

  // Config bascula
  bascula.begin(DOUT, CLK);
  bascula.read();
  //calibracion
  bascula.set_scale(-199764);
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
 //reset saved settings
 //wifiManager.resetSettings();
 //nombre del punto de acceso
 wifiManager.autoConnect("Angle_Project");
 //Si llega a este punto se ha conectado correctamente
 Serial.println("Se ha conectado correctamente");
}



//publica un mensaje al servidor mqtt indicandole el topic 
void publicarMqttAlTopic(String topic,String payload){
  
  char charBufPayload[payload.length() + 1];
  payload.toCharArray(charBufPayload,payload.length() + 1);

  char charBufTopic[String(rootMqttClient+topic).length() + 1];
  String(rootMqttClient+topic).toCharArray(charBufTopic,String(rootMqttClient+topic).length() + 1);
  Serial.println("Envio datos al mqtt");
  client.publish(charBufTopic, charBufPayload);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  if(!haCalculadoLlenado && !isBasuraAbierta()){
    
    String id = WiFi.macAddress()+"%basura";
    publicarMqttAlTopic(id+"/mesuras",calcularBasura(id));
    haCalculadoLlenado = true;
    delay(1000);
    //dormirM5Stack();
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
    
// will topic
  String id = WiFi.macAddress()+"%basura";
  String willTopic = String(id+"/WillTopic");
  char charBufTopic[String(rootMqttClient+willTopic).length() + 1];
  String(rootMqttClient+willTopic).toCharArray(charBufTopic,String(rootMqttClient+willTopic).length() + 1);
    if (client.connect("2750508476e341d6b8413239039ae8ac", charBufTopic, 1, true, "disconnected")) {
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
  Serial.println(res);
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
  //jsonMessage = String(jsonMessage+"{"+ calcularContenedor("papel")+ "},");
  //jsonMessage = String(jsonMessage+"{"+ calcularContenedor("organico")+ "},");
  jsonMessage = String(jsonMessage+"{"+ calcularContenedor("plastico")+ "}");
  jsonMessage = String(jsonMessage+"]}");
  
  return jsonMessage;
}


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
  digitalWrite(TriggerPin, LOW);
  delayMicroseconds(4);
  
  digitalWrite(TriggerPin, HIGH); //generamos pulso de 10us
  delayMicroseconds(10);
  digitalWrite(TriggerPin, LOW);
  duracion = pulseIn(EchoPin, HIGH); //medimos el tiempo pulso
  
  distanciaCm = duracion * 10.0 / 292.0 / 2.0; //convertimos a distancia
  distanciaCm = distanciaCm - cmDistanciaSensorDeBasura;
  porcentajeLlenadoBasura = 100.0 - (distanciaCm/cmTopeBasura)*100.0;//conversión a %
  //calibracion
  if(porcentajeLlenadoBasura > (100-valorCalibracion)){
    porcentajeLlenadoBasura=100.0;
  }else if(porcentajeLlenadoBasura < 0){
    porcentajeLlenadoBasura=0.0;
  }
  Serial.print("Porcentaje:");
  Serial.println(porcentajeLlenadoBasura);
  return porcentajeLlenadoBasura;
}


/**
 * Devuelve el valor actual de la bascula en KG
 */
double calcularPeso() {
  //le restamos el peso de la bascula (tara) que siempre sera el mismo
  return bascula.get_units(20)+41.59;
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
