// biblioteka I2C
#include <Wire.h>
//biblioteka żyroskop i akcelerometr
#include <LSM6.h>

// zdefiniowanie wejść analogowych
static int flexSensorPin_1 = A0; 
static int flexSensorPin_2 = A1;
static int flexSensorPin_3 = A2;
static int flexSensorPin_4 = A3;
static int flexSensorPin_5 = A5;

// zdefiniowanie akceletometru
LSM6 imu;
char reportAcc[200];

int pozycjaY;
int pozycjaX;

// inicjalizacja wejść analogowych jako czujniku ugięcia
int flexSensorReadingsPinki;
int flexSensorReadingsRing;
int flexSensorReadingsMiddle;
int flexSensorReadingsIndex;
int flexSensorReadingsThumb;  

// uruchmienie zmiennych lokalnych 
// które reprezentują stan logiczny czujników ugięcia 
bool flexSensorReadingsPinkiLogic;
bool flexSensorReadingsRingLogic;
bool flexSensorReadingsMiddleLogic;
bool flexSensorReadingsIndexLogic;
bool flexSensorReadingsThumbLogic;

void setup(){
// otwarcie komunikacji szeregowej USB
Serial.begin(9600);
// otwarnie komunikacji I2C
Wire.begin();

// jesli nie znajdziemy urządzenia LMS6 to wysyłamy komunikat
if (!imu.init())
{
Serial.println("Failed to detect and initialize IMU!");
while (1);
}
imu.enableDefault(); 

}

void loop(){
// uruchomienie funkcji odczytu z czujników
// w powtarzającej się pętli
readFlexSensors();
readAcce();
// wysłanie danych do komputera
Serial.println();
// odczekanie przed kolejnym wysłaniem
delay(100);
}

void readFlexSensors(){

// inicjalizacja wejść analogowych jako czujniku ugięcia
flexSensorReadingsPinki = analogRead(flexSensorPin_1);
flexSensorReadingsRing = analogRead(flexSensorPin_2);
flexSensorReadingsMiddle = analogRead(flexSensorPin_3);
flexSensorReadingsIndex = analogRead(flexSensorPin_4);
flexSensorReadingsThumb = analogRead(flexSensorPin_5);  

// przetworzenie stanu analogowego czujnika na stan logiczny 1/0
if(flexSensorReadingsPinki<390){
flexSensorReadingsPinkiLogic = 1;
}else{
flexSensorReadingsPinkiLogic = 0;
}

if(flexSensorReadingsRing<350){
flexSensorReadingsRingLogic = 1;
}else{
flexSensorReadingsRingLogic = 0;
}

if(flexSensorReadingsMiddle<350){
flexSensorReadingsMiddleLogic = 1;
}else{
flexSensorReadingsMiddleLogic = 0;
}

if(flexSensorReadingsIndex<350){
flexSensorReadingsIndexLogic = 1;
}else{
flexSensorReadingsIndexLogic = 0;
}

if(flexSensorReadingsThumb<780){
flexSensorReadingsThumbLogic = 1;
}else{
flexSensorReadingsThumbLogic = 0;
}

// podanie odczytanych stanów na odpowiednie miejsca 
// w ramce danych przesyłanych do komputera
Serial.print(flexSensorReadingsPinkiLogic);
Serial.print(";");
Serial.print(flexSensorReadingsRingLogic);
Serial.print(";");
Serial.print(flexSensorReadingsMiddleLogic);
Serial.print(";");
Serial.print(flexSensorReadingsIndexLogic);
Serial.print(";");
Serial.print(flexSensorReadingsThumbLogic);
Serial.print(";");
}

// odczyt danych z akcelerometru przez magistralę I2C
void readAcce(){
// uruchomienie odczytu z urządzenia LSM6
imu.read();
// odczytanie odpowiednich zmiennych z zadanrgo urządzenia
pozycjaX = map(imu.a.x, -16500, 16500, 99, 0);
pozycjaY = map(imu.a.y, -16500, 16500, 99, 0);

// zabezpieczenia
if (pozycjaX > 35 && pozycjaX < 65){
  pozycjaX = 50;
}
if (pozycjaY > 35 && pozycjaY < 65){
  pozycjaY = 50;
}

if (pozycjaX > 99){
  pozycjaX = 99;
}else if (pozycjaX < 0){
  pozycjaX = 0;
}

if (pozycjaY > 99){
  pozycjaY = 99;
}else if (pozycjaY < 0){
  pozycjaY = 0;
}

// podanie zmiennych do ramki danych
Serial.print(pozycjaX);
Serial.print(";");
Serial.print(pozycjaY);
}
