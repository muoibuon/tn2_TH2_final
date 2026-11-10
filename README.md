# Trần Ngô Tiến Đạt - 25112231 - Thực hành 2 - Thí nghiệm 2 
 Minh chứng: ảnh, videos, serial log và code ở trong các folders tương ứng của mỗi bài 
> Wokwi:
- bài 1 vd 1: https://wokwi.com/projects/480324937678396417
- bài 1 vd 2: https://wokwi.com/projects/480324761814331393
- bài 2 vd 1: https://wokwi.com/projects/480333257105617921
- bài 2 vd 2: https://wokwi.com/projects/480333437308691457
# TH2 · Bài 1 — delay() vs millis() vs timer phần cứng

LED1 → GPIO 2 · biến trở → GPIO 34. Đo khoảng thực tế giữa 2 lần nháy LED1 (đặt 500 ms) bằng `micros()`, 24 s CPU rảnh rồi 24 s CPU bận (+150 ms mỗi vòng `loop()`).

| Cách | Lệch TB khi rảnh | Lệch TB (max) khi bận | Bám nhịp khi CPU bận? |
|---|---:|---:|---|
| `delay()` | 2800 ms | 2860 (7150) ms | Không — `delay()` chặn cả chương trình, LED chỉ nháy 1 lần/8 s |
| `millis()` | 0,05 ms | 66 (100) ms | Trung bình đúng nhưng rung — chỉ kiểm tra giữa các vòng `loop()` |
| Timer phần cứng | 0,00 ms | 0,00 (0,00) ms | Có — ngắt phần cứng đúng giờ, không phụ thuộc `loop()` |

<img width="1950" height="1500" alt="image" src="https://github.com/user-attachments/assets/3c7caa88-c70d-4c25-869f-84cd33e71085" />

# TH2 · Bài 2 — So sánh các chế độ tiêu thụ điện ESP32

**Cách so sánh:** không đo được dòng nên quan sát gián tiếp qua hành vi. Một chương trình chạy lần lượt 4 chế độ (LED → GPIO 2), ghi dấu hiệu ra Serial: LED, WiFi AP `ESP32-TH2`, CPU có chạy không, thời gian thức dậy, biến RAM (`demThuong`) và biến RTC (`soLanThuc`, `RTC_DATA_ATTR`).

| Chế độ | Dòng điển hình (datasheet) | Dấu hiệu quan sát được (đo thực tế) |
|---|---:|---|
| Active + WiFi | 95–240 mA | LED sáng, điện thoại thấy WiFi `ESP32-TH2`, CPU chạy ~1 triệu vòng/s |
| Modem sleep | 30–68 mA | WiFi biến mất, LED vẫn nháy → CPU vẫn chạy |
| Light sleep | 0,8 mA | Serial im, LED **giữ sáng**; thức sau 2000,4 ms (trễ 0,4 ms), chạy tiếp chỗ cũ, `demThuong` tăng tiếp 1→5 |
| Deep sleep | 10 µA | LED **tắt**; thức = **khởi động lại** (`DEEPSLEEP_RESET`, mất 81 ms), `demThuong` về 0, `soLanThuc` vẫn giữ 1→2 |

**Cách phân biệt:** WiFi còn/mất tách Active với Modem sleep · CPU còn chạy (LED nháy, Serial in) tách Modem sleep với Light sleep · thức dậy chạy tiếp (RAM giữ) hay khởi động lại (RAM mất, chỉ RTC giữ) tách Light sleep với Deep sleep.
<img width="1950" height="720" alt="che_do_tieu_thu" src="https://github.com/user-attachments/assets/d849dc97-7f08-408b-956f-9a742c1c478a" />


**Giới hạn:** dòng ở bảng lấy từ datasheet, không đo. Đo trực tiếp khó vì: đồng hồ vạn năng không bắt được dòng thay đổi nhanh (đỉnh WiFi ~240 mA trong vài ms); board DevKit có chip USB‑UART, LDO và LED nguồn tự tiêu thụ ~10–20 mA, che mất mức µA của deep sleep; muốn đo đúng cần cảm biến dòng (INA219) hoặc cấp nguồn thẳng vào chip ESP32.

