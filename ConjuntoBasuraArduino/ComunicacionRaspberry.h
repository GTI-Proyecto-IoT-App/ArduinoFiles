//--------------------------------
//
// ComunicacionRaspberry.h
// Clase que gestiona la comunicacion a la raspberry 
// solo implementado por uart
//
//--------------------------------

#ifndef  COMUNICACION_RASPBERRY_H
#define COMUNICACION_RASPBERRY_H
#include "Arduino.h"
#include <string>

class ComunicacionRaspberry{

  private:
    const String IDENTIFAICADOR_TEXTO_UART = "$$$";// usado para saber que datos en la 
                                          //rapsberry son los buenos

  public:
    ComunicacionRaspberry(const int);// constructor para establecer conexion por uart
    void enviarPorUART(String);// enviar datos por uart
  
};
#endif
