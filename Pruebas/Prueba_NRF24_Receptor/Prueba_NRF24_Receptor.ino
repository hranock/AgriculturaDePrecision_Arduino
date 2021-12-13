/* 
 *  Codigo para comprobar la comunicaciones entre
 *  los dispostivos mediante el uso de NRF24 (RECEPTOR)
 *  
 *  Agricultura de precisión 2021
 *  
 *  Hugo Rene Sanchez Rosas ~ 17092248
 *  Ing. TIC ~ ITSPA
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 9
#define CSN_PIN 10
 
const byte addr[5]={'0','1','A','D','P'};
RF24 radio(CE_PIN, CSN_PIN);

float datos[2];

void setup(){
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1, addr);
  radio.startListening();
}
 
void loop(){
  if(radio.available()){
    radio.read(&datos, sizeof(datos));
    Serial.print(millis());
    Serial.print(F("\tDato 0 = "));
    Serial.print(datos[0]);
    Serial.print(F(" ms, Dato 1 ="));
    Serial.println(datos[1]);
 }
}
