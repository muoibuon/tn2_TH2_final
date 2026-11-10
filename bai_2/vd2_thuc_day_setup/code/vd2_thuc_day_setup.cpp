#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "esp_sleep.h"

#define LED_PIN 2
#define T_ACTIVE_MS 30000
#define T_MODEM_MS 10000
#define DEEP_S 10

RTC_DATA_ATTR int soLanThuc = 0;
int demThuong = 0;
WebServer server(80);
const char *nguyenNhan = "";
uint32_t tBatDau = 0;

const char *tenNguyenNhan(esp_sleep_wakeup_cause_t c) {
  if (c == ESP_SLEEP_WAKEUP_TIMER) return "timer (thuc tu deep sleep)";
  if (c == ESP_SLEEP_WAKEUP_UNDEFINED) return "cap nguon / reset";
  return "khac";
}

void trangChu() {
  demThuong++;
  String h = "<!doctype html><meta charset=utf-8><meta name=viewport content='width=device-width'>"
             "<meta http-equiv=refresh content=1><title>ESP32 TH2</title>"
             "<body style='font-family:sans-serif;padding:16px'><h2>ESP32 vua thuc day</h2>";
  h += "<p>Nguyen nhan thuc: <b>" + String(nguyenNhan) + "</b></p>";
  h += "<p>soLanThuc (RTC, giu qua deep sleep): <b>" + String(soLanThuc) + "</b></p>";
  h += "<p>demThuong (RAM, mat khi deep sleep): <b>" + String(demThuong) + "</b></p>";
  h += "<p>Thoi gian thuc: <b>" + String((millis() - tBatDau) / 1000) + " s</b> / " + String(T_ACTIVE_MS / 1000) + " s roi tat WiFi</p>";
  server.send(200, "text/html", h);
}

void setup() {
  tBatDau = millis();
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  esp_sleep_wakeup_cause_t c = esp_sleep_get_wakeup_cause();
  if (c == ESP_SLEEP_WAKEUP_UNDEFINED) soLanThuc = 0;
  soLanThuc++;
  nguyenNhan = tenNguyenNhan(c);
  Serial.printf("SETUP,nguyen_nhan=%s,soLanThuc=%d,demThuong=%d\n", nguyenNhan, soLanThuc, demThuong);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP32-TH2");
  server.on("/", trangChu);
  server.begin();
  Serial.printf("ACTIVE,wifi=AP ESP32-TH2,web=http://%s\n", WiFi.softAPIP().toString().c_str());
}

void loop() {
  uint32_t t = millis() - tBatDau;
  if (t < T_ACTIVE_MS) {
    server.handleClient();
    digitalWrite(LED_PIN, (t / 500) % 2);
  } else if (t < T_ACTIVE_MS + T_MODEM_MS) {
    if (WiFi.getMode() != WIFI_OFF) {
      server.stop();
      WiFi.softAPdisconnect(true);
      WiFi.mode(WIFI_OFF);
      Serial.printf("MODEM_SLEEP,wifi=tat,demThuong=%d\n", demThuong);
    }
    digitalWrite(LED_PIN, (t / 100) % 2);
  } else {
    Serial.printf("DEEP,vao,%d s\n", DEEP_S);
    Serial.flush();
    digitalWrite(LED_PIN, LOW);
    esp_sleep_enable_timer_wakeup(DEEP_S * 1000000ULL);
    esp_deep_sleep_start();
  }
}
