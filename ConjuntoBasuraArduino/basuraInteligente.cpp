// ---------------------------------------------------
//
// BasuraInteligente.cpp
//Autor SERGI SIRVENT SEMPERE
//
// ---------------------------------------------------

#include "basuraInteligente.h"

//incluimos las siguientes bibiliotecas

#include <ArduinoJson.h>
#include <string>
#include <list>
#include <WiFi.h>

// ---------------------------------------------------
// Constructor vacio
// ---------------------------------------------------

BasuraInteligente::BasuraInteligente()
{};

// ---------------------------------------------------
// Constructor con parametros
// ---------------------------------------------------

  BasuraInteligente::BasuraInteligente(ContenedorInteligente *listaContenedores)
{
  (*this).listaContenedores = listaContenedores;
  //(*this).pinFinalCarrera = pinFinalCarrera;
  
 };

// ---------------------------------------------------
//    getters -->  Z
// ---------------------------------------------------

/*
int BasuraInteligente::getPinFinalCarrera()
{
  return (*this).pinFinalCarrera;
};
*/
// ---------------------------------------------------
//    calcular --> JSON
// ---------------------------------------------------

  StaticJsonDocument<100> BasuraInteligente::calcular(){
    
    StaticJsonDocument<100> doc;
    StaticJsonDocument<100> medidas;
    JsonArray array = medidas.to<JsonArray>();
    
    

     doc["id"] = (WiFi.macAddress()+"%basura");
     doc["fecha"] = "1212121212121";
     
     
     array.add((*this).listaContenedores[0].calcular()); 
     array.add((*this).listaContenedores[1].calcular());
     array.add((*this).listaContenedores[2].calcular());
     array.add((*this).listaContenedores[3].calcular());
     
     doc["medidas"]= array;
     
    std::string strJson;
    serializeJson(doc, strJson);
    
    return doc;
  };
