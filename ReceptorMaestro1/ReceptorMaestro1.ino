/* 
 *  Codigo para comprobar la comunicaciones entre
 *  los dispostivos mediante el uso de NRF24 (RECEPTOR)
 *  
 *  Agricultura de precisión 2021
 *  
 *  Hugo Rene Sanchez Rosas ~ 17092248
 *  Ing. TIC ~ ITSPA
 */

#include <nRF24L01.h>
#include <RF24.h>
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h >

#define CE_PIN 2
#define CSN_PIN 4

const uint64_t addr[6]={
  0x17092248FFLL, //sin uso
  0x1709224801LL, //nodo 1
  0x1709224802LL, //nodo 2
  0x1709224803LL, //nodo 3
  0x1709224804LL, //nodo 4
  0x1709224805LL  //nodo 5
};

RF24 rf0(CE_PIN, CSN_PIN);

int nNodos=1;
bool ok=false;

struct datosEnviados {
  byte ch1;   //id nodo
  byte ch2;   //Sensor AFC28
  byte ch3;   //Sensor BFC28
  byte ch4;   //Humedad
  byte ch5;   //DTH11
  byte ch6;   //LM35
};

datosEnviados datos;

void setup(){
  Serial.begin(115200);

  WiFiManager wm;
  bool sc = wm.autoConnect("AdP-Master","password");

  if(!sc) {
        Serial.println("Fallo conexion WiFi...");
        // ESP.restart();
  }else{
        //if you get here you have connected to the WiFi    
        Serial.println("Conectado...");
  }
  
  rf0.begin();
  rf0.setPALevel(RF24_PA_MAX);
  rf0.setDataRate(RF24_250KBPS);
  rf0.setChannel(99);  
}
 
void loop(){
  for(int i=0; i<nNodos; i++){
    openChannel(i);
    if(rf0.available()){
      rf0.read(&datos, sizeof(datosEnviados));
      verDatosRaw();
    }
  }
}

//********** M  E  T  O  D  O  S **********
void openChannel(int i){
  rf0.openReadingPipe(i+1, addr[i+1]);
  rf0.startListening();
}

void verDatosRaw(){
  Serial.print(F(" ID: "));
  Serial.print(datos.ch1);
  Serial.print(F(" || S1: "));
  Serial.print(datos.ch2);
  Serial.print(F(" || S2: "));
  Serial.print(datos.ch3);
  Serial.print(F(" || H: "));
  Serial.print(datos.ch4);
  Serial.print(F(" || T1: "));
  Serial.print(datos.ch5);
  Serial.print(F(" || T2: "));
  Serial.println(datos.ch6);
}
