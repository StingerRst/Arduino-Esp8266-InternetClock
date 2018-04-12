void ShowDisplay(void) {
  //static int8_t oldDisplay[4] ;

//  disp.display(Display); // воводим на индикатор
  disp.point(false);
  disp.display(0,Display[0]); // воводим на индикатор
  if (state==2) {
    disp.point(true);
  }
  else {
    disp.point(false);
  }
  disp.display(1,Display[1]); // воводим на индикатор
  if (state==3) {
    disp.point(true);
  }
  else {
    disp.point(false);
  }
  disp.display(2,Display[2]); // воводим на индикатор
    disp.point(false);
  disp.display(3,Display[3]); // воводим на индикатор

}
