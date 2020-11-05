/*
CLASE QUE REPRESENTA  LA BASURA INTELIGENTE

AUTOR: SERGI SIRVENT SEMPERE

*/

#ifndef BASURAINTELIGENTE_H
#define BASURAINTELIGENTE_H


//incluimos las siguientes bibliotecas

#include <string>
#include "contenedorInteligente.h"
#include <M5Stack.h>

class BasuraInteligente{
  

  private:
    ContenedorInteligente *listaContenedores;
    
    
  public:

    BasuraInteligente();
    BasuraInteligente(ContenedorInteligente *listaContenedores);
   
    //int getPinFinalCarrera();
    StaticJsonDocument<100> calcular();
    //void dormirse();
    
};
#endif
