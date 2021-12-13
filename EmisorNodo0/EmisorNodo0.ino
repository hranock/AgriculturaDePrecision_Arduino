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
#include <DHT.h>
#include <SoftwareSerial.h>

#define PDHT11  2
#define AFC281  5
#define BFC281  6
#define BTRX    7
#define BTTX    8
#define CE      9
#define CSN     10
#define MOSI    11
#define MISO    12
#define SCK     13
#define AFC282  A0
#define BFC282  A1
#define LM35    A2

//Pines libres
#define XPIND3  3
#define XPIND4  4
#define XPINA3  A3
#define XPINA4  A4
#define XPINA5  A5
#define XPINA6  A6
#define XPINA7  A7

/* Direcciones
addr0 = 0x17092248FFLL  
addr1 = 0x1709224801LL 
addr2 = 0x1709224802LL
addr3 = 0x1709224803LL 
addr4 = 0x1709224804LL 
addr5 = 0x1709224805LL
*/

const uint64_t addr = 0x1709224801LL;
RF24 rf0(CE, CSN);
DHT dht(PDHT11, DHT11);
SoftwareSerial BT(BTRX,BTTX);

const byte nLectura=100;
byte h;
float t;

struct datosEnviados {
  byte ch1;   //id nodo
  byte ch2;   //Sensor AFC28
  byte ch3;   //Sensor BFC28
  byte ch4;  //Humedad
  float ch5;  //DTH11
  float ch6;  //LM35
};

datosEnviados datos;

void setup(){
  pinMode(PDHT11, INPUT);
  pinMode(AFC281, INPUT);
  pinMode(BFC281, INPUT);
  
  Serial.begin(115200);
  BT.begin(9600);
  dht.begin();
  
  rf0.begin();
  //rf0.setAutoAck(false);
  rf0.setPALevel(RF24_PA_MAX);
  rf0.setDataRate(RF24_250KBPS);
  rf0.setChannel(100);
  rf0.openWritingPipe(addr);
  rf0.stopListening();
  datos.ch1=1;
}
 
void loop(){ 
  datos.ch2=leerFC28(AFC282);
  datos.ch3=leerFC28(BFC282);
  leerDHT();
  datos.ch6=leerLM35(50);
  
  if(rf0.write(&datos, sizeof(datosEnviados))){
    verDatosRaw();
  }
  delay(500);
}

//********** M  E  T  O  D  O  S **********
int leerFC28(unsigned char* PIN){
  long z=0;
  for(int i=0; i<nLectura; i++){
    z+=analogRead(PIN);
    delay(10);
  }
  z=(int)(z/nLectura);
  return map(z, 0, 1023, 0, 254);
}

void leerDHT(){
  datos.ch4=dht.readHumidity();
  datos.ch5=dht.readTemperature();
  if (isnan(datos.ch4) || isnan(datos.ch5)){
    datos.ch4=0;
    datos.ch5=0;
  }
}

float leerLM35(int n){
  long z=0;
  for (int i=0; i<n; i++){
    z+=analogRead(LM35);
    delay(10);
  }
  int t = (int) (z/n);
  return (5.5*t*100.0)/1024.0;
}

void reiniciarDatos(){
  datos.ch1=-1;
  datos.ch2=0;
  datos.ch3=0;
  datos.ch4=0;
  datos.ch5=0;
  datos.ch6=0;
}

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
