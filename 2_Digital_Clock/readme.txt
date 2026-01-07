## ⚙️ Nasıl Çalışır?

1.  **Zamanlama:** Sistem saati 40 MHz'e ayarlanmıştır. `Timer0`, her 40.000.000 çevrimde bir (yani tam 1 saniyede bir) kesme üretir.
2.  **Kesme Fonksiyonu (`ftimerkesmesi`):**
    * Saniyeyi artırır.
    * Saniye 60 olduğunda dakikayı, dakika 60 olduğunda saati günceller.
    * Ana döngüye "Ekranı güncelle" haberini vermek için `ekran_guncelle` bayrağını **true** yapar.
    * Çalıştığını göstermek için kart üzerindeki Kırmızı LED'i yakıp söndürür (Heartbeat).
3.  **Ana Döngü (`main`):**
    * Sürekli olarak `ekran_guncelle` bayrağını kontrol eder.
    * Bayrak **true** olduğunda, saat verilerini karakter dizisine (string) dönüştürür ve LCD'ye basar.
    * *Optimizasyon:* `sprintf` gibi ağır fonksiyonlar yerine matematiksel işlemlerle manuel string dönüşümü yapılarak performans artırılmıştır.

## 🛠 Donanım Bağlantıları

| Bileşen | Tiva C Pini | Açıklama |
| :--- | :--- | :--- |
| **Kırmızı LED** | **PF1** | Saniye başı yanıp söner (Heartbeat) |
| **LCD RS** | (Lcd.h'a bakınız) | Genelde PB0 |
| **LCD EN** | (Lcd.h'a bakınız) | Genelde PB1 |
| **LCD D4-D7** | (Lcd.h'a bakınız) | Genelde PB4-PB7 |

## 📦 Kullanılan Kütüphaneler

* **TivaWare DriverLib:** Donanım soyutlama katmanı (Timer, GPIO, Interrupt, SysCtl).
* **Lcd.h:** HD44780 LCD sürme kütüphanesi (Proje içine dahil edilmelidir).

## 💻 Kod Yapısı

```c
// Kritik değişkenler "volatile" olarak tanımlanmıştır
volatile bool ekran_guncelle = false;

// Zamanlayıcı Ayarı (1 Saniye)
TimerLoadSet(TIMER0_BASE, TIMER_A, 40000000-1);