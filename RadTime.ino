void ReadTime(void) {
  if (WiFi.isConnected()) {   // Если есть WIFI
      unsigned long tmp=millis();  
      if ((tmp - prev_T_timer > T_interval)||(prev_T_timer ==  0)||(timeStatus() != timeSet)){
        Serial.println("Time not set");
        prev_T_timer = tmp;  // запоминаем текущее время    
        time_t sTime=getNtpTime();
        if (sTime){
          setTime(sTime);
          Serial.println("time is set");
          T_err=0;
        }  
        if (timeStatus() == timeNotSet){
          T=0;
          T_err=1;
          Serial.println("T error");
        }
        Serial.println (now());
        Serial.println(String(hour())+":"+String(minute())+":"+ String(second()));
      }
  }
}

/*-------- Код для NTP ----------*/
 
const int NTP_PACKET_SIZE = 48;  //  NTP-время – в первых 48 байтах сообщения
byte packetBuffer[NTP_PACKET_SIZE];  //  буфер для хранения входящих и исходящих пакетов 

time_t getNtpTime()
{
  IPAddress ntpServerIP; // IP-адрес NTP-сервера
 
  while (Udp.parsePacket() > 0) ; // отбраковываем все пакеты, полученные ранее 
  Serial.println("Transmit NTP Request");  //  "Передача NTP-запроса" 
  // подключаемся к случайному серверу из списка:
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 2500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");  //  "Получение NTP-ответа"
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // считываем пакет в буфер 
      unsigned long secsSince1900;
      // конвертируем 4 байта (начиная с позиции 40) в длинное целое число: 
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");  //  "Нет NTP-ответа :(" 
  return 0; // если время получить не удалось, возвращаем «0» 
}
// отправляем NTP-запрос серверу времени по указанному адресу: 
void sendNTPpacket(IPAddress &address)
{
  // задаем все байты в буфере на «0»: 
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // инициализируем значения для создания NTP-запроса
  // (подробнее о пакетах смотрите по ссылке выше) 
  packetBuffer[0] = 0b11100011;   // LI (от «leap indicator», т.е. «индикатор перехода»), версия, режим работы 
  packetBuffer[1] = 0;     // слой (или тип часов) 
  packetBuffer[2] = 6;     // интервал запросов 
  packetBuffer[3] = 0xEC;  // точность 
  // 8 байтов с нулями, обозначающие базовую задержку и базовую дисперсию: 
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // После заполнения всех указанных полей
  // вы сможете отправлять пакет с запросом о временной метке:      
  Udp.beginPacket(address, 123); // NTP-запросы к порту 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
