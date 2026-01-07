# 🛠 Donanım Bağlantıları

| Bileşen | Tiva C Pini | Açıklama |
| :--- | :--- | :--- |
| **UART0 RX** | **PA0** | PC'den veri alma (USB üzerinden sanal port) |
| **UART0 TX** | **PA1** | PC'ye veri gönderme (USB üzerinden sanal port) |
| **LCD RS, EN, D4-D7** | **PORT B** | (Lcd.h dosyasına göre PB0-PB7 varsayılmıştır) |

## 📡 UART Komut Protokolü

Sistemi kontrol etmek için herhangi bir Seri Terminal programı (Putty, TeraTerm, Arduino Serial Monitor) kullanabilirsiniz.
**Baud Rate:** 9600 | **Data Bits:** 8 | **Stop Bit:** 1 | **Parity:** None

### 1. Saati Ayarlama Komutu
Saati değiştirmek için `S` komutu kullanılır.
* **Format:** `S:SAAT:DAKİKA` veya `S:SAAT:DAKİKA:SANİYE`
* **Örnek:** `S:14:30` (Saati 14:30:00 yapar)
* **Örnek:** `S:09:05:45` (Saati 09:05:45 yapar)

### 2. Ekrana Yazı Yazma Komutu
LCD'nin üst satırına mesaj yazdırmak için `M` komutu kullanılır.
* **Format:** `M:MESAJINIZ`
* **Örnek:** `M:Sistem Aktif` (Ekrana "Sistem Aktif" yazar)

> **Önemli Not:** Terminal programınızda **"Append Newline (\n)"** veya **"Both NL & CR"** seçeneğinin aktif olduğundan emin olun. Komutların işlenmesi için satır sonu karakteri gereklidir.

## 💻 Yazılım Mimarisi

Sistem **"Kesme Tabanlı (Interrupt-Driven)"** bir mimariye sahiptir:

1.  **Timer ISR (`Timerkesmesi`):**
    * Sadece zamanı sayar (sn++, dk++, sa++).
    * Bu işlem mikrosaniyeler sürer, ana döngüyü bloklamaz.
    * Ana döngüye "Saniye doldu" bayrağını (flag) kaldırır.

2.  **Main Loop (Ana Döngü):**
    * **Ekran Güncelleme:** Bayrak kalktıysa LCD'yi günceller ve saati UART'tan gönderir.
    * **Veri Alma:** UART tamponunda (buffer) veri var mı diye bakar.
    * **Veri İşleme:** Eğer `\n` karakteri geldiyse, tamponu okur, komutun `S` mi yoksa `M` mi olduğuna karar verir ve `atoi` fonksiyonu ile sayıya çevirip saati günceller.

## 📦 Kurulum

1.  Proje dosyalarını çalışma alanınıza (Workspace) ekleyin.
2.  `Lcd.c` ve `Lcd.h` dosyalarının projede olduğundan emin olun.
3.  Kodu derleyin ve yükleyin.
4.  PC'de Aygıt Yöneticisi'nden Tiva kartının **COM portunu** öğrenin.
5.  Terminal programını açıp 9600 baud rate ile bağlanın.
6.  `Tiva RTC Baslatildi...` mesajını görmelisiniz.

## ⚠️ Dikkat Edilmesi Gerekenler

* Kodda string işlemleri için `rx_buffer` boyutu 32 karakterle sınırlandırılmıştır. Çok uzun mesajlar göndermeyiniz.
* `atoi` fonksiyonu sayısal dönüşüm yapar. `S:AA:BB` gibi geçersiz formatlar gönderirseniz saat hatalı ayarlanabilir.