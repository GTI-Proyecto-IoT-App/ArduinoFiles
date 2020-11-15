// ---------------------------------------------------
//
// BasuraInteligente.cpp
//Autor SERGI SIRVENT SEMPERE
//
// ---------------------------------------------------

#include "basuraInteligente.h"


// ---------------------------------------------------
// Constructor vacio
// ---------------------------------------------------

BasuraInteligente::BasuraInteligente()
{};

// ---------------------------------------------------
// Constructor con parametros
// ---------------------------------------------------

  BasuraInteligente::BasuraInteligente(String id,ContenedorInteligente *listaContenedores)
{
  (*this).listaContenedores = listaContenedores;
  (*this).id = id;
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
//    calcular --> String
// ---------------------------------------------------

String BasuraInteligente::calcular(){
  
  StaticJsonDocument<100> doc;
  const size_t CAPACITY = JSON_ARRAY_SIZE(4);
  StaticJsonDocument<CAPACITY> medidas;
  
  JsonArray array = medidas.to<JsonArray>();
  

   doc["id"] = (*this).id;
   doc["fecha"] = "1212121212121";
   
   medidas.add((*this).listaContenedores[1].calcular()); 
   medidas.add((*this).listaContenedores[1].calcular());
   medidas.add((*this).listaContenedores[2].calcular());
   medidas.add((*this).listaContenedores[3].calcular());

   doc["medidas"]= medidas;
 
  String strJson;

  serializeJson(doc, strJson);
  
  return strJson;
};
