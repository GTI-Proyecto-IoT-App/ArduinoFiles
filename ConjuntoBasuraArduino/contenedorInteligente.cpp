// ---------------------------------------------------
//
// ContenedorInteligente.cpp
//Autor SERGI SIRVENT SEMPERE
//
// ---------------------------------------------------

//incluimos las siguientes bibliotecas

#include "contenedorInteligente.h"
#include <ArduinoJson.h>
#include<string>

// ---------------------------------------------------
// Constructor vacio
// ---------------------------------------------------

ContenedorInteligente::ContenedorInteligente()
{};

// ---------------------------------------------------
// Constructor con parametros
// ---------------------------------------------------

  ContenedorInteligente::ContenedorInteligente(int echoPinUltrasonico ,int triggerPinUltrasonico , int pinEntradaBascula,
  double profundidadContenedor,double distanciaContenedor , double pesoVacio , std::string tipoContenedor)
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
//    getters --> Z o R o string
// ---------------------------------------------------
/*
  int ContenedorInteligente::getPinUltra()
  {
    return (*this).pinEntradaUltrasonico;
  }
  int ContenedorInteligente::getPinBas(){return (*this).pinEntradaBascula;};
  double ContenedorInteligente::getProfundidad(){return (*this).profundidadContenedor;};
  double ContenedorInteligente::getDistancia(){return (*this).distanciaContenedor;};
  double ContenedorInteligente::getPesoVacio(){return (*this).pesoVacio;};
  std::string ContenedorInteligente::getTipo(){return (*this).tipoContenedor;};

*/


// ---------------------------------------------------
//    calcular --> JSON
// ---------------------------------------------------

StaticJsonDocument<100> ContenedorInteligente::calcular(){
  
  StaticJsonDocument<100> doc;
  
  doc["tipo"] = (*this).tipoContenedor;
  //doc["peso"] = 
  doc["volumen"] = (*this).calcularLlenado();

  

  return doc;
  
}

/**
 * ->calcularLlenado -> R
 * @author JuanCarlos y Angel 
 * Lee distancia del sensor sonico
 * calculado entro el pin de disparo y pin de recepcion
 * y calcula el porcentaje de llenado de la basura
 */
double ContenedorInteligente::calcularLlenado() {
  
  int valorCalibracion = 5;
  double duracion,porcentajeLlenadoBasura,distanciaCm;
  digitalWrite((*this).triggerPinUltrasonico, LOW); //nos aseguramos señal baja
  delayMicroseconds(4);
  digitalWrite((*this).triggerPinUltrasonico, HIGH); //generamos pulso de 10us
  delayMicroseconds(10);
  digitalWrite((*this).triggerPinUltrasonico, LOW);
  duracion = pulseIn((*this).echoPinUltrasonico, HIGH); //medimos el tiempo pulso
  distanciaCm = duracion * 10.0 / 292.0 / 2.0; //convertimos a distancia
  distanciaCm = distanciaCm - (*this).distanciaContenedor ;
  porcentajeLlenadoBasura = 100.0 - (distanciaCm/(*this).profundidadContenedor )*100.0;//conversión a %

  //calibracion
  if(porcentajeLlenadoBasura > (100-valorCalibracion)){
    porcentajeLlenadoBasura=100.0;
  }else if(porcentajeLlenadoBasura < 0){
    porcentajeLlenadoBasura=0.0;
  }
  
  return porcentajeLlenadoBasura;
}
