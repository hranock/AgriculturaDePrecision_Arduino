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

const uint64_t addr[6]={
  0x1709224800LL, //sin uso
  0x17092248ABLL, //nodo 0
  0x17092248CDLL, //nodo 1
  0x17092248A3LL, //nodo 2
  0x170922480FLL, //nodo 3
  0x1709224805LL  //nodo 4
};

RF24 radio(CE_PIN, CSN_PIN);

float datos[3];
int nNodos=2;

void openChannel(int i){
  radio.openReadingPipe(1, addr[1]);
  radio.startListening();
}

void setup(){
  Serial.begin(115200);
  radio.begin();
  //radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(100);  
}
 
void loop(){
  for(int i=1; i<nNodos; i++){
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
    }
 }
 Serial.println();
}
