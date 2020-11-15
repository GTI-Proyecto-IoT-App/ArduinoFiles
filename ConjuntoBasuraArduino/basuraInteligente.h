/*
CLASE QUE REPRESENTA  LA BASURA INTELIGENTE

AUTOR: SERGI SIRVENT SEMPERE

*/

#ifndef BASURAINTELIGENTE_H
#define BASURAINTELIGENTE_H


//incluimos las siguientes bibliotecas

#include <string>
#include "contenedorInteligente.h"

class BasuraInteligente{
  

  private:
    ContenedorInteligente *listaContenedores;
    String id;
    
  public:

    BasuraInteligente();
    BasuraInteligente(String id,ContenedorInteligente *listaContenedores);
   
    //int getPinFinalCarrera();
    String calcular();
    //void dormirse();
    
};
#endif
