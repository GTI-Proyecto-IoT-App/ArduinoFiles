//--------------------------------
//
// ComunicacionRaspberry.cpp
// Clase que gestiona la comunicacion a la raspberry 
// solo implementado por uart
//
//--------------------------------
#include "ComunicacionRaspberry.h"
                   
//constructor parametrizado comunicacion uart
// inicializa el puerto serie, este valor debe ser el mismo que 
// el estableciso en la raspberry
ComunicacionRaspberry::ComunicacionRaspberry(const int baudiosUart)
{}

//
// Texto -> enviarPorUART()
// antes de enviar por el por uart formatea los datos
// para que en la raspberry se sepan que son los buenos
//
void ComunicacionRaspberry::enviarPorUART(String datos){
  datos = String((*this).IDENTIFAICADOR_TEXTO_UART + datos + (*this).IDENTIFAICADOR_TEXTO_UART);
  Serial.print(datos);// enviar datos por el puerto serie
}
