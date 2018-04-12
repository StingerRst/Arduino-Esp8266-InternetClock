void State_1(void) {
  Display[0] =17;
  Display[1] =14;
  Display[2] =19;
  Display[3] =19;
  ShowDisplay();  
 if (timeStatus() == timeSet){
    state=2;
  }
}
