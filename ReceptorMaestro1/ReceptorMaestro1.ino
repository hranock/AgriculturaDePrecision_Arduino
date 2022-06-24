/* 
 *  Codigo para comprobar la comunicaciones entre
 *  los dispostivos mediante el uso de NRF24 (RECEPTOR)
 *  
 *  Agricultura de precisión 2021-2022
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
#include <ESP8266HTTPClient.h>

#define CE_PIN 2
#define CSN_PIN 4

const uint64_t addr[6]={
  0x1709224800LL, //sin uso
  0x1709224801LL, //nodo 1
  0x1709224802LL, //nodo 2
  0x1709224803LL, //nodo 3
  0x1709224804LL, //nodo 4
  0x1709224805LL  //nodo 5
};

RF24 rf0(CE_PIN, CSN_PIN);

const int nNodos=3;
bool ok=false;

struct datosEnviados{
  byte ch1;   //id nodo
  byte ch2;   //AFC28
  byte ch3;   //BFC28
  byte ch4;   //Humeda
  byte ch5;   //DHT11
  byte ch6;   //LM35
};

datosEnviados datos;

const char* host="www.srvroh.com";
const int port=80;

void setup(){
  Serial.begin(115200);

  WiFiManager wm;
  bool sc = wm.autoConnect("AdP-Master","password");

  if(!sc) {
        Serial.println("Fallo conexion WiFi...");
        // ESP.restart();
  }
  
  rf0.begin();
  rf0.setAutoAck(false);
  rf0.setPALevel(RF24_PA_MAX);
  rf0.setDataRate(RF24_250KBPS);
  for(int i=1; i<=nNodos; i++){
    rf0.openReadingPipe(i, addr[i]);
  }
  rf0.startListening();
}
 
void loop(){
  looop();
}

void looop(){
  for(byte i=1; i<=nNodos; i++){
    if(rf0.available(&i)){
      rf0.read(&datos, sizeof(datosEnviados));
      //verDatosRaw();
      crearConexion();
    }
  }
}

//********** M  E  T  O  D  O  S **********
void verDatosRaw(){
  Serial.print(datos.ch1);
  Serial.print(F(" || "));
  Serial.print(datos.ch2);
  Serial.print(F(" || "));
  Serial.print(datos.ch3);
  Serial.print(F(" || "));
  Serial.print(datos.ch4);
  Serial.print(F(" || "));
  Serial.print(datos.ch5);
  Serial.print(F(" || "));
  Serial.println(datos.ch6);
}

void crearConexion(){
  WiFiClient wc;
  if(!wc.connect(host, port)){
    Serial.println(F("Error de conexión"));
    wc.stop();
    return;
  }else{
    HTTPClient http;
    
    http.begin(wc, "http://srvroh.com/adp_php/reportepost.php");
    http.addHeader("Content-Type", "application/json");
    
    String json="{\"id\":"+String(datos.ch1)+",\"hums1\":"+String(datos.ch2)+
            ",\"hums2\":"+String(datos.ch3)+",\"hum\":"+String(datos.ch4)+
            ",\"dht\":"+String(datos.ch5)+",\"lm\":"+String(datos.ch6)+"}";

    int httpRC = http.POST(json);
    http.end();
    wc.stop();
    
    Serial.print(json);
    Serial.print(F(" - HTTP Response code: "));
    Serial.println(httpRC);
  }
}
