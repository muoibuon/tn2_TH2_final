#include "do_nhip.h"

struct NhipMillis {
  uint32_t timerMs, loopTick = 0;
  NhipMillis(uint32_t t) : timerMs(t) {}
  bool updateLoopTimer() {
    if (millis() - loopTick < timerMs) return false;
    loopTick += timerMs;
    if (millis() - loopTick >= timerMs) loopTick = millis();
    return true;
  }
};

NhipMillis nhipLed(500), nhipBienTro(2000), nhipHeartbeat(5000);
bool ledState = false;

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  Serial.println("# VD2 millis()");
  nhipLed.loopTick = nhipBienTro.loopTick = nhipHeartbeat.loopTick = millis();
}

void loop() {
  kiemTraPha();
  if (nhipLed.updateLoopTimer()) {
    ledState = !ledState;
    digitalWrite(LED1, ledState);
    tkLed.ghiNhan(micros());
  }
  if (nhipBienTro.updateLoopTimer()) {
    int tho = analogRead(ADC);
    tkBienTro.ghiNhan(micros());
    Serial.printf("bienTro=%d\n", tho);
  }
  if (nhipHeartbeat.updateLoopTimer()) {
    Serial.println("heartbeat");
    tkHeartbeat.ghiNhan(micros());
  }
  cpuBan();
}
