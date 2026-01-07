## 🛠 Donanım Bağlantıları

### 1. LCD Ekran (2x16)
(Pin bağlantıları `Lcd.h` dosyasında tanımlıdır, genellikle PORTB kullanılır.)

### 2. NTC Sensör Devresi (Voltaj Bölücü)
Sıcaklık ölçümü için 10k NTC ve 4.7k direnç kullanılarak bir voltaj bölücü kurulmalıdır.

| Bileşen | Bağlantı | Açıklama |
| :--- | :--- | :--- |
| **NTC Termistör** | +3.3V --- ADC Pini | NTC'nin bir ucu VCC'ye, diğer ucu ADC pinine. |
| **Direnç (4.7kΩ)** | ADC Pini --- GND | Direncin bir ucu ADC pinine, diğer ucu toprağa. |
| **ADC Girişi** | **PE3 (AIN0)** | Tiva C üzerindeki analog okuma pini. |



### 3. Diğer Bileşenler
* **Kırmızı LED (PF1):** Sistemin çalıştığını gösteren "Kalp Atışı" (Heartbeat) göstergesi.

## ⚙️ Yazılım Mimarisi

Sistem iki ana parçadan oluşur:

1.  **Timer0 Kesmesi (ISR - Arka Plan):**
    * Her 1 saniyede bir tetiklenir.
    * Saat, dakika ve saniye değişkenlerini günceller.
    * `saniye_guncellendi` bayrağını **true** yapar.
    * Bu bölüm **çok hızlı** çalışır, ağır işlemler içermez.

2.  **Ana Döngü (Main Loop - Ön Plan):**
    * Sürekli olarak NTC sensöründen voltaj okur.
    * `log()` ve kayan nokta (float) işlemleriyle sıcaklığı hesaplar.
    * `saniye_guncellendi` bayrağını kontrol eder; eğer bayrak kalkmışsa saati ekrana yazar.

## 🧮 Kullanılan Formül (Steinhart-Hart)

NTC sensörler sıcaklığa bağlı olarak direnç değiştirir ancak bu değişim doğrusal değildir. Doğru sıcaklığı bulmak için şu kod bloğu kullanılmıştır:

```c
// Direnç Hesaplama
float ntc_direnci = (SERIES_RESISTOR * voltaj) / (VCC - voltaj);

// Steinhart-Hart Denklemi (Basitleştirilmiş Beta Modeli)
float steinhart;
steinhart = ntc_direnci / NTC_NOMINAL;     // (R/Ro)
steinhart = log(steinhart);                // ln(R/Ro)
steinhart /= BETA_VALUE;                   // 1/B * ln(R/Ro)
steinhart += 1.0 / (25.0 + 273.15);        // + (1/To)
steinhart = 1.0 / steinhart;               // Tersi alınır (Kelvin cinsinden)
sicaklik_c = steinhart - 273.15;           // Celsius'a çevrilir