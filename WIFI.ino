void WIFIinit() {
  // Если задана точка доступа - пытаемся подключиться
  if (ssidName!="") {
    WiFi.mode(WIFI_STA);
    byte tries = 30;
    WiFi.begin(ssidName.c_str(), ssidPassword.c_str());
    // Делаем проверку подключения до тех пор пока счетчик tries
    // не станет равен нулю или не получим подключение
    while (--tries && WiFi.status() != WL_CONNECTED)
    {
      Serial.print(".");
      delay(1000);
    }
  }
  if (WiFi.status() != WL_CONNECTED){
    // Если не удалось подключиться запускаем в режиме AP
    Serial.println("WiFi up AP");
    StartAPMode();
    //Настраиваем и запускаем HTTP интерфейс
    Serial.println("Start WebServer");
    HTTP_init();  
  }  
  else {
    // Иначе удалось подключиться отправляем сообщение
    // о подключении и выводим адрес IP
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

bool StartAPMode()
{ // Отключаем WIFI
  WiFi.disconnect();
  // Меняем режим на режим точки доступа
  WiFi.mode(WIFI_AP);
  // Задаем настройки сети
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(apIP, apIP, (255,255,255,0)) ? "Ready" : "Failed!");
  Serial.print("Setting soft-AP ... ");
  Serial.print("ssidAPName:");
  Serial.println(ssidAPName);
  Serial.print("ssidAPPassword:");
  Serial.println(ssidAPPassword);
  ssidAPName= (ssidAPName) ? ssidAPName :"ESP_" +(String) ESP.getChipId();
  // Если задан пароль,то поднимаем тояку доступа с паролем, иначе- без
  if (ssidAPPassword!=""){
    Serial.println(WiFi.softAP(ssidAPName.c_str(), ssidAPPassword.c_str()) ? "Ready" : "Failed!");
  }
  else {
    Serial.println(WiFi.softAP(ssidAPName.c_str()) ? "Ready" : "Failed!");
  }
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  return true;
}
