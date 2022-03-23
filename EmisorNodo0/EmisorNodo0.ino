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
#define BTTX    7
#define BTRX    8
#define CE      9
#define CSN     10
#define MOSI    11
#define MISO    12
#define SCK     13
#define AFC282  14
#define BFC282  15
#define LM35    16

//Pines libres
#define XPIND3  3
#define XPIND4  4
#define XPINA3  A3
#define XPINA4  A4
#define XPINA5  A5
#define XPINA6  A6
#define XPINA7  A7

const uint64_t addr[6]={
  0x1709224800LL, //sin uso
  0x1709224801LL, //nodo 1
  0x1709224802LL, //nodo 2
  0x1709224803LL, //nodo 3
  0x1709224804LL, //nodo 4
  0x1709224805LL  //nodo 5
};

const byte idNodo=1;
RF24 rf0(CE, CSN);
DHT dht(PDHT11, DHT11);

int nLectura=100;
int tMinutos=10;
int h;
float t;
int tAct;
bool flg;

struct datosEnviados{
  byte ch1;   //id nodo
  byte ch2;   //AFC28
  byte ch3;   //BFC28
  byte ch4;   //Humeda
  byte ch5;   //DHT11
  byte ch6;   //LM35
};

datosEnviados datos;

SoftwareSerial BT(BTTX,BTRX);

void setup(){
  pinMode(PDHT11, INPUT);

  BT.begin(9600);
  Serial.begin(115200);
  dht.begin();
  
  rf0.begin();
  rf0.setAutoAck(false);
  rf0.setPALevel(RF24_PA_MAX);
  rf0.setDataRate(RF24_250KBPS);
  rf0.openWritingPipe(addr[idNodo]);
  rf0.stopListening();

  BT.print("Iniciando... nodo ");
  BT.println(idNodo);
  
  datos.ch1=idNodo;
  flg=true;
}
 
void loop(){
  tAct=esperaMinuto();
  if(tAct%tMinutos==0 && flg){  
    leerSensores();
    if(rf0.write(&datos, sizeof(datosEnviados))){
      verDatosRaw();
      verDatosRawBT();
    }else{
      BT.println(F("Error nrf24"));
    }
    flg=false;
  }
  if(tAct%tMinutos==1 && !flg){
    flg=true;
  }
}

//********** M  E  T  O  D  O  S **********
int leerFC28(uint8_t PIN){
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
  float t=z/n;
  return (5.5*t*100)/1024;
}

int esperaMinuto(){
  return (millis()/(60000))%60;
}

void leerSensores(){
  datos.ch2=leerFC28(AFC282);
  datos.ch3=leerFC28(BFC282);
  leerDHT();
  datos.ch6=leerLM35(50);
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

void verDatosRawBT(){
  BT.print(datos.ch1);
  BT.print(F(" || "));
  BT.print(datos.ch2);
  BT.print(F(" || "));
  BT.print(datos.ch3);
  BT.print(F(" || "));
  BT.print(datos.ch4);
  BT.print(F(" || "));
  BT.print(datos.ch5);
  BT.print(F(" || "));
  BT.println(datos.ch6);
}
