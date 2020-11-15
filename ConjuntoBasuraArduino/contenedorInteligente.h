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
    uint8_t echoPinUltrasonico;
    uint8_t triggerPinUltrasonico;//pin de lectura
    uint8_t pinEntradaBascula; //pin de lectura
    double profundidadContenedor; //altura del contenedor
    double distanciaContenedor; //distancia del sensor al contenedor
    double pesoVacio; //peso del contenedor sin basura
    String tipoContenedor; //indica el tipo
  
    // ---------------------------------------------------
    //    calcular llenado 
    // ---------------------------------------------------
  
    double calcularLlenado();//lee los sensores

//========================================PUBLICO
  public:

  ContenedorInteligente(); //constructor sin parametros

  ContenedorInteligente(uint8_t echoPinUltrasonico,
                           uint8_t triggerPinUltrasonico,     //constructor con parametros
                          uint8_t pinEntradaBascula,
                          double profundidadContenedor,
                          double distanciaContenedor,
                          double pesoVacio,
                          String  tipoContenedor);
                          

// ---------------------------------------------------
//    getters --> Z o R o string
// ---------------------------------------------------

  int getPinUltra();
  int getPinBas();
  double getProfundidad();
  double getDistancia();
  double getPesoVacio();
  String getTipo();     
                     




// ---------------------------------------------------
//    calcular --> JSON
// ---------------------------------------------------

  StaticJsonDocument<100> calcular();

};



#endif
