#include <Arduino.h>
#include <TimeLib.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266HTTPClient.h>
#include "TM1637.h"
#include <WiFiUdp.h>
// NTP-серверы:
static const char ntpServerName[] = "us.pool.ntp.org";
//static const char ntpServerName[] = "time.nist.gov";
//static const char ntpServerName[] = "time-a.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-b.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-c.timefreq.bldrdoc.gov";

const int timeZone = 5;     // центрально-европейское время
WiFiUDP Udp;
unsigned int localPort = 8888;  // локальный порт для прослушивания UDP-пакетов
int8_t Display[4];  // Переменная дял вывода на экран
DynamicJsonBuffer jsonBuffer;
String jsonConfig,ssidName,ssidPassword,ssidAPName,ssidAPPassword;
int state=1;  // Состояние автомата
float t=0;    // Переменная значений температуры 
byte t_err=1; // Ошибка измерения температуры
float T=0;    // Время
byte T_err=1; // Ошибка измерения времени
byte D=1;     // 0: показываем температуру 1:показываем время
const time_t timezone=5*60*60; // Часовой пояс

unsigned long D_interval = 5*1000;         // Таймер переключения экрана сек*миллисек
unsigned long t_interval = 10*60*1000;     // Таймер замера температуры часов*мин*сек*миллисек
unsigned long T_interval = 2*60*60*1000;     // Таймер сверки часов часов*мин*сек*миллисек
unsigned long prev_D_timer=0;      // Предидущее значение таймера переключения экрана
unsigned long prev_t_timer=0;      // Предидущее значение таймера температуры
unsigned long prev_T_timer=0;      // Предидущее значение таймера времени

ESP8266HTTPUpdateServer httpUpdater;
IPAddress apIP(192, 168, 4, 1);
ESP8266WebServer HTTP(80);

#define CLK D2//pins definitions for the module and can be changed to other ports       
#define DIO D3
TM1637 disp(CLK, DIO);

void setup() {
  ssidName = "Rst";
  ssidPassword = "SkodaRst";
  // put your setup code here, to run once:
  disp.init();//D4056A is the type of the module
  disp.set(BRIGHTEST);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  Serial.begin(9600);
  WIFIinit();//WiFi.begin
  Serial.print("Wifi status ");
  Serial.println(WiFi.status());
  Udp.begin(localPort);
}

void loop() {
  // put your main code here, to run repeatedly:
 // Disp();
  ReadTime();
  ReadTemp();
  switch (state){
    case 1: 
        State_1();  // Состояние Error
      break;
    case 2: 
        State_2(); // Состояние "Показ времени"
      break;
    case 3: 
        State_3();  // Состояние "Показ температуры"
      break;
    default:  
      Serial.println("Error status");
  }
  //ShowDisplay();
  delay (100);
}
