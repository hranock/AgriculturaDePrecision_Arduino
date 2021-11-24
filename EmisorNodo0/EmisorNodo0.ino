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

/* Direcciones
addr1 = 0x1709224800LL  
addr2 = 0x17092248ABLL 
addr3 = 0x17092248CDLL
addr4 = 0x17092248A3LL 
addr5 = 0x170922480FLL 
addr6 = 0x1709224805LL
*/

const uint64_t addr = 0x17092248ABLL;
RF24 radio(CE_PIN, CSN_PIN);

float datos[3];

void setup(){
  Serial.begin(115200); 
  radio.begin();
  //radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(100);
  radio.openWritingPipe(addr);
  radio.stopListening();
  Serial.println(F("\nIniciando..."));
  datos[0]=0;
}
 
void loop(){ 
  datos[1]=millis();
  datos[2]=3.14;
  if(radio.write(&datos, sizeof(datos))){
     Serial.print(millis());
  }
  delay(500);
}
