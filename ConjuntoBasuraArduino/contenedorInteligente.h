/*
CLASE QUE REPRESENTA LOS CONTENEDORES INTELIGENTES DE LA BASURA INTELIGENTE

AUTOR: SERGI SIRVENT SEMPERE

*/




//incluimos las siguientes bibliotecas

#ifndef CONTENEDORINTELIGENTE_H
#define CONTENEDORINTELIGENTE_H


#include <string>
#include <ArduinoJson.h>




class ContenedorInteligente{
  

  private:

  int echoPinUltrasonico;
  int triggerPinUltrasonico;//pin de lectura
  int pinEntradaBascula; //pin de lectura
  double profundidadContenedor; //altura del contenedor
  double distanciaContenedor; //distancia del sensor al contenedor
  double pesoVacio; //peso del contenedor sin basura
  std::string tipoContenedor; //indica el tipo
  
  // ---------------------------------------------------
  //    calcular llenado 
  // ---------------------------------------------------

  double calcularLlenado();//lee los sensores

  public:

  ContenedorInteligente(); //constructor sin parametros

  ContenedorInteligente(int echoPinUltrasonico,
                           int triggerPinUltrasonico,     //constructor con parametros
                          int pinEntradaBascula,
                          double profundidadContenedor,
                          double distanciaContenedor,
                          double pesoVacio,
                          std::string  tipoContenedor);
                          

// ---------------------------------------------------
//    getters --> Z o R o string
// ---------------------------------------------------

  int getPinUltra();
  int getPinBas();
  double getProfundidad();
  double getDistancia();
  double getPesoVacio();
  std::string getTipo();     
                     




// ---------------------------------------------------
//    calcular --> JSON
// ---------------------------------------------------

  StaticJsonDocument<100> calcular();

};



#endif
