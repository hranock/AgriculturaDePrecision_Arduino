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
#include <RF24.h>

#define CSN_PIN 9
#define CE_PIN 10
 
byte addr[]={"ADP01"};

RF24 radio(CE_PIN, CSN_PIN);

float datos[2];

void setup(){
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, addr)
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}
 
void loop(){
 if(radio.available()){
  char texto[32] = "";
  radio.read(&texto, sizeof(texto));
  Serial.println(texto);
 }
 /*
 datos[0]=millis();
 datos[1]=3.14;
 bool ok = radio.write(datos, sizeof(datos));
  if(ok){
     Serial.print("Datos enviados: "); 
     Serial.print(datos[0]); 
     Serial.print(" , "); 
     Serial.println(datos[1]); 
  }else{
     Serial.println("error al enviar");
  }
  delay(2000);
  */
}
