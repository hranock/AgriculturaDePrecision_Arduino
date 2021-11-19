/* 
 *  Codigo para comprobar la comunicaciones entre
 *  los dispostivos mediante el uso de NRF24 (EMISOR)
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
  radio.openWritingPipe(addr);
  radio.stopListening();
  Serial.println(F("\nIniciando..."));
}
 
void loop(){ 
  datos[0]=millis();
  datos[1]=3.14;
  if(radio.write(&datos, sizeof(datos))){
     Serial.print(millis());
     Serial.print(F("\tDatos: ")); 
     Serial.print(datos[0]); 
     Serial.print(F(", ")); 
     Serial.println(datos[1]); 
  }
  delay(500);
}
