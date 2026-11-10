#include "do_nhip.h"

hw_timer_t *timer = nullptr;
volatile bool ledState = false, coLed = false, coBienTro = false, coHeartbeat = false;
volatile uint32_t ledUs = 0, dem = 0;

void IRAM_ATTR onTimer() {
  ledState = !ledState;
  digitalWrite(LED1, ledState);
  ledUs = micros();
  coLed = true;
  dem++;
  if (dem % 4 == 0) coBienTro = true;
  if (dem % 10 == 0) coHeartbeat = true;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  Serial.println("# VD3 timer phan cung");
  timer = timerBegin(1000000);
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 500000, true, 0);
}

void loop() {
  kiemTraPha();
  if (coLed) {
    coLed = false;
    tkLed.ghiNhan(ledUs);
  }
  if (coBienTro) {
    coBienTro = false;
    int tho = analogRead(ADC);
    tkBienTro.ghiNhan(micros());
    Serial.printf("bienTro=%d\n", tho);
  }
  if (coHeartbeat) {
    coHeartbeat = false;
    Serial.println("heartbeat");
    tkHeartbeat.ghiNhan(micros());
  }
  cpuBan();
}
