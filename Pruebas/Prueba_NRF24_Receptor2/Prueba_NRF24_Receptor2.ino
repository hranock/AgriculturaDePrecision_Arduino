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

#define CE_PIN 2
#define CSN_PIN 4

/* Direcciones
addr1 = 0x1709224800LL  
addr2 = 0x17092248ABLL 
addr3 = 0x17092248CDLL
addr4 = 0x17092248A3LL 
addr5 = 0x170922480FLL 
addr6 = 0x1709224805LL
*/

//const byte addr[5]={'0','1','A','D','P'};
const uint64_t addr = 0x1709224800LL;
RF24 radio(CE_PIN, CSN_PIN);

//float datos[3];
byte datos;

void setup(){
  Serial.begin(115200);
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  //radio.setChannel(100);  
  radio.openReadingPipe(1, addr);
  radio.startListening();
}
 
void loop(){
  if(radio.available()){
    radio.read(&datos, sizeof(datos));
    Serial.println(datos);
    /*
    Serial.print(millis());
    Serial.print(F("\tDato 0 = "));
    Serial.print(datos[0]);
    Serial.print(F(" Dato 1 = "));
    Serial.print(datos[1]);
    Serial.print(F(" ms, Dato 2 = "));
    Serial.println(datos[2]);
    */
 }
}
