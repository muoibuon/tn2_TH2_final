#include <Arduino.h>
#include <sys/time.h>
#include "esp_sleep.h"

#define LED_PIN 2
#define LIGHT_LAN 3
#define LIGHT_S 2
#define DEEP_S 10

RTC_DATA_ATTR int soLanThuc = 0;
RTC_DATA_ATTR int64_t henThucUs = 0;
int demThuong = 0;

int64_t rtcUs() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (int64_t)tv.tv_sec * 1000000LL + tv.tv_usec;
}

void lamLightSleep() {
  demThuong++;
  Serial.flush();
  int64_t truoc = rtcUs();
  esp_sleep_enable_timer_wakeup(LIGHT_S * 1000000ULL);
  esp_light_sleep_start();
  float nguMs = (rtcUs() - truoc) / 1000.0f;
  Serial.printf("LIGHT,ngu_ms=%.1f,tre_thuc_ms=%.2f,soLanThuc=%d,demThuong=%d\n",
                nguMs, nguMs - LIGHT_S * 1000, soLanThuc, demThuong);
}

void lamDeepSleep() {
  soLanThuc++;
  demThuong++;
  Serial.printf("DEEP,vao,soLanThuc=%d,demThuong=%d\n", soLanThuc, demThuong);
  Serial.flush();
  digitalWrite(LED_PIN, LOW);
  henThucUs = rtcUs() + DEEP_S * 1000000LL;
  esp_sleep_enable_timer_wakeup(DEEP_S * 1000000ULL);
  esp_deep_sleep_start();
}

void setup() {
  int64_t vaoSetup = rtcUs();
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER && henThucUs) {
    Serial.printf("DEEP,thuc,khoi_dong_lai_ms=%.1f,soLanThuc=%d,demThuong=%d\n",
                  (vaoSetup - henThucUs) / 1000.0f, soLanThuc, demThuong);
  } else {
    Serial.println("BOOT,cap_nguon");
  }
}

void loop() {
  for (int i = 0; i < LIGHT_LAN; i++) lamLightSleep();
  lamDeepSleep();
}
