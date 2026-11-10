#pragma once
#include <Arduino.h>

#define LED1 2
#define ADC 34
#define CPU_BAN_MS 150
#define PHA_MS 24000

struct DoNhip {
  const char *ten;
  uint32_t chuKyMs;
  uint32_t truocUs = 0, n = 0;
  float tongLech = 0, lechMax = 0;

  DoNhip(const char *t, uint32_t c) : ten(t), chuKyMs(c) {}

  void ghiNhan(uint32_t nowUs) {
    if (truocUs) {
      float dt = (nowUs - truocUs) / 1000.0f;
      float lech = fabsf(dt - chuKyMs);
      tongLech += lech;
      if (lech > lechMax) lechMax = lech;
      n++;
      Serial.printf("EV,%s,%lu,%.2f,%.2f\n", ten, (unsigned long)(nowUs / 1000), dt, lech);
    }
    truocUs = nowUs;
  }

  void inTongKet() {
    Serial.printf("TK,%s,chu_ky=%lu,n=%lu,lech_tb=%.2f,lech_max=%.2f,tong_lech=%.1f\n",
                  ten, chuKyMs, n, n ? tongLech / n : 0, lechMax, tongLech);
    truocUs = 0; n = 0; tongLech = 0; lechMax = 0;
  }
};

DoNhip tkLed("led", 500), tkBienTro("bienTro", 2000), tkHeartbeat("heartbeat", 5000);
int pha = 0;

void kiemTraPha() {
  if (pha < 2 && millis() >= (uint32_t)(pha + 1) * PHA_MS) {
    Serial.printf("PHA,%d,%s\n", pha, pha ? "CPU_BAN" : "CPU_RANH");
    tkLed.inTongKet(); tkBienTro.inTongKet(); tkHeartbeat.inTongKet();
    pha++;
  }
}

void cpuBan() {
  if (pha != 1) return;
  uint32_t t = millis();
  while (millis() - t < CPU_BAN_MS) {}
}
