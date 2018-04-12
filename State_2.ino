void State_2(void) {
//  Display =String(hour()/ 10)+String(hour()% 10)+String(minute()/ 10)+String(minute()% 10);  
  Display[0] =hour()/ 10;
  Display[1] =hour()% 10;
  Display[2] =minute()/ 10;
  Display[3] =minute()% 10;
  ShowDisplay();  
  unsigned long tmp=millis();  
  if (tmp - prev_D_timer > D_interval)  {  // Если таймер показа переполнился , то обнуляем
    prev_D_timer = tmp;  // запоминаем текущее время    
    if (timeStatus() == timeSet){
      state=3;
    }
    else {
      state=1;
    }
  }
}
