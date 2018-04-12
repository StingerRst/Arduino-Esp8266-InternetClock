void State_3(void) {
  if (abs(int(t)/10)){ // Если есть десятки градусов
    if (t<0){
      Display[0] =16;  
    }
    else{
      Display[0] =17;
    }
    Display[1] =abs(int(t)/10);
  }
  else {
    Display[0] =17;
    if (t<0){
      Display[1] =16;  
    }
    else{
      Display[1] =17;
    }
  }
 
  Display[2] =abs(int(t)%10);
  Display[3] =abs(int(t*10)%10);
  ShowDisplay();  

  unsigned long tmp=millis();  
  if (tmp - prev_D_timer > D_interval)  {  // Если таймер показа переполнился , то обнуляем
    prev_D_timer = tmp;  // запоминаем текущее время    
    if (timeStatus() == timeSet){
      state=2;
    }
    else {
      state=1;
    }
  }
}
