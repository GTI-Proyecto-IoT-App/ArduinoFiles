#include <M5Stack.h>

#define PinGPIOInterruptFinalCarrera 2

static volatile bool seHaCerrado = false;
bool haCalculadoLlenado = false;
bool deboEsperarAlCerrado;

/**
 * FINAL DE CARRERA 
 * Pin GPIO 5 INPUT normalmente cerrado 
 * 
 */

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  M5.begin(true,false,true);
  pinMode(PinGPIOInterruptFinalCarrera, INPUT);
  M5.Lcd.setTextSize(2);
  attachInterrupt(digitalPinToInterrupt(PinGPIOInterruptFinalCarrera),final_carrera_activado,HIGH);
  //rtc_gpio_deinit(PinGPIOInterruptFinalCarrera);//reconfigurar el pin por si se desperto y estaba configurado para despertado

  
  if(isBasuraAbierta()){
    haCalculadoLlenado = true;
  }

}

void final_carrera_activado(){
    Serial.println("INTERRUPCION");
    haCalculadoLlenado = false;
}


void loop() {
  Serial.println("LOOP");
  
  if(!haCalculadoLlenado){
      calcularVolumen();
      dormirM5Stack();
  }else{
      
   }


  delay(500);
}

void calcularVolumen(){
  Serial.println("CALCULANDO VOLUMEN");
  delay(2000);
  Serial.println("TERMINA CALCULAR");
  haCalculadoLlenado = true;
}

bool isBasuraAbierta(){
  return !digitalRead(PinGPIOInterruptFinalCarrera);
}

void dormirM5Stack(){
  Serial.println("DORMIR");
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_2,0); //1 = High, 0 = Low
  esp_deep_sleep_start();
  
}
