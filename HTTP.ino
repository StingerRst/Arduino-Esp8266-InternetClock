void HTTP_init(void) {
Serial.println("HTTP_init");
  
Serial.println(WiFi.localIP().toString());
  // API для устройства
  HTTP.on("/restart", handle_Restart);  // Перезагрузка модуля по запросу вида /restart?device=ok
  HTTP.on("/upload", handle_FUpload);   // Загрузка файла   /upload 
  httpUpdater.setup(&HTTP);             // Обновление прошивки   /update 

  
  // Запускаем HTTP сервер
  HTTP.begin();

}
void handle_FUpload() {
const char* _serverIndex =
    R"(<html><body><form method='POST' action='edit' enctype='multipart/form-data'>
                  <input type='file' name='data'>
                  <input type='submit' value='Upload'>
               </form>
   </body></html>)";
    HTTP.send(200, F("text/html"), _serverIndex); 
}

// Перезагрузка модуля по запросу вида http://192.168.0.101/restart?device=ok
void handle_Restart() {
  String restart = HTTP.arg("device");          // Получаем значение device из запроса
  if (restart == "ok") {                         // Если значение равно Ок
    HTTP.send(200, "text / plain", "Reset OK"); // Oтправляем ответ Reset OK
    ESP.restart();                                // перезагружаем модуль
  }
  else {                                        // иначе
    HTTP.send(200, "text / plain", "No Reset"); // Oтправляем ответ No Reset
  }
}






