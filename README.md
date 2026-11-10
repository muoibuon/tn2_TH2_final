# TH2 · Bài 1 — delay() vs millis() vs timer phần cứng

LED1 → GPIO 2 · biến trở → GPIO 34. Đo khoảng thực tế giữa 2 lần nháy LED1 (đặt 500 ms) bằng `micros()`, 24 s CPU rảnh rồi 24 s CPU bận (+150 ms mỗi vòng `loop()`).

| Cách | Lệch TB khi rảnh | Lệch TB (max) khi bận | Bám nhịp khi CPU bận? |
|---|---:|---:|---|
| `delay()` | 2800 ms | 2860 (7150) ms | Không — `delay()` chặn cả chương trình, LED chỉ nháy 1 lần/8 s |
| `millis()` | 0,05 ms | 66 (100) ms | Trung bình đúng nhưng rung — chỉ kiểm tra giữa các vòng `loop()` |
| Timer phần cứng | 0,00 ms | 0,00 (0,00) ms | Có — ngắt phần cứng đúng giờ, không phụ thuộc `loop()` |

![So sánh](minh_chung/so_sanh_delay_millis_timer.png)

Code: `vd1_dung_delay/code` · `vd2_millis/code` · `vd3_timer/code`
