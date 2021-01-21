// ---------------------------------------------------
//
// ContenedorInteligente.cpp
//Autor SERGI SIRVENT SEMPERE
//
// ---------------------------------------------------

//incluimos las siguientes bibliotecas
#include "contenedorInteligente.h"
#include <ArduinoJson.h>
//#include <SoftwareSerial.h>
// ---------------------------------------------------
// Constructor vacio
// ---------------------------------------------------

ContenedorInteligente::ContenedorInteligente()
{};

// ---------------------------------------------------
// Constructor con parametros
// ---------------------------------------------------

  ContenedorInteligente::ContenedorInteligente(uint8_t echoPinUltrasonico ,uint8_t triggerPinUltrasonico , uint8_t pinEntradaBascula,
  double profundidadContenedor,double distanciaContenedor , double pesoVacio , String tipoContenedor)
  {
    (*this).echoPinUltrasonico = echoPinUltrasonico;
    (*this).triggerPinUltrasonico = triggerPinUltrasonico;
    (*this).pinEntradaBascula = pinEntradaBascula;
    (*this).profundidadContenedor = profundidadContenedor;
    (*this).distanciaContenedor = distanciaContenedor;
    (*this).pesoVacio = pesoVacio;
    (*this).tipoContenedor = tipoContenedor;

    pinMode((*this).triggerPinUltrasonico, OUTPUT);
    pinMode((*this).echoPinUltrasonico, INPUT);

  };



// ---------------------------------------------------
//    calcular --> JSON
// ---------------------------------------------------

StaticJsonDocument<100> ContenedorInteligente::calcular(){
  
  StaticJsonDocument<100> doc1;
  
  //Serial.println(calcularLlenado());

  //Serial.println("vidrio");
  
  doc1["tipo"] = "vidrio";//(*this).tipoContenedor;
  //doc["peso"] = 

  doc1["volumen"] = calcularLlenado();

  

  return doc1;
  
};

/**
 * ->calcularLlenado -> R
 * @author JuanCarlos y Angel 
 * Lee distancia del sensor sonico
 * calculado entro el pin de disparo y pin de recepcion
 * y calcula el porcentaje de llenado de la basura
 */
double ContenedorInteligente::calcularLlenado() {
  
  int valorCalibracion = 5;

  Serial.println("LL-1-------------");
  double duracion,porcentajeLlenadoBasura,distanciaCm;
  //Serial.println((*this).triggerPinUltrasonico);
  Serial.println("LL-1-------------");
  digitalWrite(16/*(*this).triggerPinUltrasonico*/, LOW); //nos aseguramos señal baja
  delayMicroseconds(4);

  digitalWrite(16/*(*this).triggerPinUltrasonico*/, HIGH); //generamos pulso de 10us
  
  digitalWrite(16/*(*this).triggerPinUltrasonico*/, LOW);

  duracion = pulseIn(17/*(*this).echoPinUltrasonico*/, HIGH); //medimos el tiempo pulso

  distanciaCm = duracion * 10.0 / 292.0 / 2.0; //convertimos a distancia

  distanciaCm = distanciaCm - 20;//(*this).distanciaContenedor ;

  porcentajeLlenadoBasura = 100.0 - (distanciaCm/20/*(*this).profundidadContenedor*/ )*100.0;//conversión a %


  //calibracion
  if(porcentajeLlenadoBasura > (100-valorCalibracion)){
    porcentajeLlenadoBasura=100.0;
  }else if(porcentajeLlenadoBasura < 0){
    porcentajeLlenadoBasura=0.0;
  }
  
  return porcentajeLlenadoBasura;
};
