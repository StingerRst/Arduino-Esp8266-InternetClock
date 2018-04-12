void ReadTemp(void) {
  if (WiFi.isConnected()) {   // Если есть WIFI
      //Serial.println("ReadTemp");
      unsigned long tmp=millis();
      if ((tmp - prev_t_timer > t_interval)||(prev_t_timer ==  0)||t_err){
        prev_t_timer = tmp;  // запоминаем текущее время    
        HTTPClient http;
        http.begin("http://narodmon.ru/api/sensorsValues?sensors=15955&uuid=ea51ba08e3a93485516b42da3756cb9e&api_key=r7kAVUqAekbSd"); //HTTP

        const char * headerkeys[] = {"User-Agent","Set-Cookie","Cookie","Date","Content-Type","Connection"} ;
        size_t headerkeyssize = sizeof(headerkeys)/sizeof(char*);
        http.collectHeaders(headerkeys,headerkeyssize);
        Serial.printf("Header count: %d\r\n", http.headers());
        for (int i=0; i < http.headers(); i++) {
          Serial.printf("%s = %s\r\n", http.headerName(i).c_str(), http.header(i).c_str());
        }
                        
        Serial.printf("Cookie: %s\r\n", http.header("Date").c_str());
        Serial.printf("Set-Cookie: %s\r\n", http.header("Set-Cookie").c_str());
        int httpCode = http.GET();
        if(httpCode > 0) {
          if(httpCode == HTTP_CODE_OK) {
            String answer = http.getString();
            Serial.println(http.getString());
            JsonObject& json = jsonBuffer.parseObject(answer);
            if (int(json["sensors"][0]["time"])-int(json["sensors"][0]["changed"]) <= t_interval/1000) { // Если с измерения температу прошло 
                              //времени меньше, чем интервал , то учитываем температуру
              t=float(json["sensors"][0]["value"]);
              t_err=0;
            }
            else {  // Иначе ошибка измерения температуры
              t=0;
              t_err=1;
              Serial.println("t error");
            }
            Serial.println(String(hour())+":"+String(minute())+":"+ String(second()));
            Serial.println(t);
            jsonBuffer.clear();
          }          
          else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
          }

        }
        http.end();      
      }
  }
}
