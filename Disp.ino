void Disp(void) {
  unsigned long tmp=millis();
  if (tmp - prev_D_timer > D_interval){
    prev_D_timer=tmp;
    D=!D;
    //Serial.println ( "D="+ String(D));
  }
}
