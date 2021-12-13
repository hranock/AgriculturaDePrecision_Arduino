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

RF24 radio(CE_PIN, CSN_PIN);

float datos[3];
int nNodos=2;
bool ok=false;

void openChannel(int i){
  radio.openReadingPipe(i+1, addr[i+1]);
  radio.startListening();
}

void setup(){
  Serial.begin(115200);

  WiFiManager wm;
  bool sc = wm.autoConnect("AdP-Master","password");

  if(!sc) {
        Serial.println("Failed to connect");
        // ESP.restart();
  }else{
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
  }
  
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(100);  
}
 
void loop(){
  for(int i=0; i<nNodos; i++){
    openChannel(i);
    if(radio.available()){
      radio.read(&datos, sizeof(datos));
      Serial.print(millis());
      Serial.print(F("\tDato 0 = "));
      Serial.print(datos[0]);
      Serial.print(F(" Dato 1 = "));
      Serial.print(datos[1]);
      Serial.print(F(" ms, Dato 2 = "));
      Serial.println(datos[2]);
      ok=true;
    }else{
      ok=false;
    }
    if(i==nNodos-1 && ok){
      Serial.println();
      delay(100);  
    }
  }
}
