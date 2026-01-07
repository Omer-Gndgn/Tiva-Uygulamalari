## 📋 Özellikler

* **4-Bit Modu:** LCD'yi sürmek için sadece 6 GPIO pini kullanır (Veri tasarrufu sağlar).
* **Kolay Kullanım:** `Lcd_init()`, `Lcd_Puts()`, `Lcd_Goto()` gibi basit fonksiyonlar içerir.
* **TivaWare Uyumlu:** Kodlar, TivaWare DriverLib kütüphaneleri üzerine inşa edilmiştir.
* **Güvenilir Başlatma:** LCD'nin kararlı çalışması için gerekli olan yazılımsal reset ve başlatma rutinlerini içerir.

## 🛠 Donanım Bağlantıları

Kütüphane varsayılan olarak **PORT B** üzerinden çalışacak şekilde ayarlanmıştır. Tiva LaunchPad ile LCD arasındaki bağlantılar aşağıdaki gibidir:

| LCD Pini | Fonksiyon | Tiva C Pini (TM4C123G) | Açıklama |
| :--- | :--- | :--- | :--- |
| **VSS** | GND | GND | Toprak |
| **VDD** | Power | +5V (VBUS) | Güç Beslemesi |
| **V0** | Contrast | Potansiyometre | Ekran Parlaklığı (Orta uç) |
| **RS** | Register Select | **PB0** | Komut/Veri Seçimi |
| **RW** | Read/Write | GND | Sadece yazma yapıldığı için Toprak |
| **E** | Enable | **PB1** | Yetkilendirme Pini |
| **D0-D3** | Data | Boş | Kullanılmıyor (4-bit mod) |
| **D4** | Data | **PB4** | Veri Pini 4 |
| **D5** | Data | **PB5** | Veri Pini 5 |
| **D6** | Data | **PB6** | Veri Pini 6 |
| **D7** | Data | **PB7** | Veri Pini 7 |
| **A (LED+)**| Backlight | +3.3V / +5V | Arka Işık (+) |
| **K (LED-)**| Backlight | GND | Arka Işık (-) |

> **Not:** Pin tanımlarını değiştirmek isterseniz `Lcd.h` dosyasındaki `#define` satırlarını düzenleyebilirsiniz.

## 📦 Gereksinimler

Bu projeyi derlemek için aşağıdaki yazılım ve kütüphanelere ihtiyacınız vardır:
* Keil uVision, Code Composer Studio (CCS) veya IAR Embedded Workbench.
* **TivaWare for C Series** (DriverLib dosyaları projenize eklenmiş olmalıdır).

## 🚀 Kurulum ve Kullanım

1.  `Lcd.c` ve `Lcd.h` dosyalarını proje klasörünüze kopyalayın.
2.  `main.c` dosyanıza `Lcd.h` dosyasını dahil edin (`#include "Lcd.h"`).
3.  TivaWare clock ayarlarını yaptıktan sonra `Lcd_init();` fonksiyonunu çağırın.

### Örnek Kod (main.c)

```c
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "Lcd.h"

int main(void) {
    // Sistem saatini 80 MHz'e ayarla (TivaWare sürümüne göre değişebilir)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // LCD'yi başlat
    Lcd_init();

    // Ekrana yazı yaz
    Lcd_Goto(1, 1);
    Lcd_Puts("Merhaba Dunya!");
    
    Lcd_Goto(2, 1);
    Lcd_Puts("Tiva C LCD Test");

    while(1) {
        // Sonsuz döngü
    }
}