#include "do_nhip.h"

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  Serial.println("# VD1 delay()");
}

void loop() {
  kiemTraPha();
  digitalWrite(LED1, HIGH); tkLed.ghiNhan(micros()); delay(500);
  digitalWrite(LED1, LOW);  tkLed.ghiNhan(micros()); delay(500);

  int tho = analogRead(ADC);
  tkBienTro.ghiNhan(micros());
  Serial.printf("bienTro=%d\n", tho);
  delay(2000);

  Serial.println("heartbeat");
  tkHeartbeat.ghiNhan(micros());
  delay(5000);

  cpuBan();
}
