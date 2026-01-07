#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "Lcd.h" // Bu dosyada pin tanýmlarý (RS, E, D4, D5, D6, D7, LCDPORT, vb.) olmalýdýr.

// --- Lcd.h Dosyasýndan gelmesi gereken varsayýmsal tanýmlar ---
// Lütfen bu tanýmlarýn doðru olduðundan emin olun veya Lcd.h dosyanýza ekleyin!
#define LCDPORTENABLE   SYSCTL_PERIPH_GPIOB
#define LCDPORT         GPIO_PORTB_BASE

#define RS              GPIO_PIN_0 // Register Select (Command/Data)
#define E               GPIO_PIN_1 // Enable Pin (Latch)

#define D4              GPIO_PIN_4
#define D5              GPIO_PIN_5
#define D6              GPIO_PIN_6
#define D7              GPIO_PIN_7
#define DATA_PINS_MASK  (D4 | D5 | D6 | D7)
#define ALL_PINS        (RS | E | DATA_PINS_MASK)
// --- Varsayýmsal tanýmlarýn sonu ---


// Yardýmcý fonksiyon: 4-bit nibble gönderir ve E pinini doðru þekilde sallar.
void Lcd_SendNibble(uint8_t nibble) {
    uint32_t ui32Value = 0;

    // 1. Nibble'ý (4 bit) Tiva pin konumlarýna (D4-D7) kaydýrýn
    // D4-D7'nin PB4-PB7'de olduðunu varsayarsak, 4 bit sola kaydýrýlýr.
    ui32Value = (nibble & 0x0F) << 4;

    // 2. Sadece 4 veri pinine yeni deðeri yazýn. Diðer pinler (RS, E) deðiþmez.
    GPIOPinWrite(LCDPORT, DATA_PINS_MASK, ui32Value);

    // 3. E pinini sallayýn (Pulse)
    GPIOPinWrite(LCDPORT, E, E);      // E = 1 (Pin maskesi kullanýlarak pini YÜKSEK yap)
    SysCtlDelay(10);                  // Kýsa bir darbe süresi gecikmesi
    GPIOPinWrite(LCDPORT, E, 0x00);   // E = 0 (Pini ALÇAK yap)

    SysCtlDelay(50000);               // Komutun iþlenmesi için yeterli gecikme (yaklaþýk 1.25ms @ 80MHz)
}

// LCD'ye komut gönderir (RS=0)
void Lcd_Komut(unsigned char c) {
    // RS pinini ALÇAK (Command Mode) yap
    GPIOPinWrite(LCDPORT, RS, 0x00);

    // Yüksek Nibble'ý gönder (Bits 7-4)
    Lcd_SendNibble(c >> 4);

    // Alçak Nibble'ý gönder (Bits 3-0)
    Lcd_SendNibble(c & 0x0F);
}

// LCD'ye karakter verisi gönderir (RS=1)
void Lcd_Putch(unsigned char d) {
    // RS pinini YÜKSEK (Data Mode) yap
    GPIOPinWrite(LCDPORT, RS, RS);

    // Yüksek Nibble'ý gönder (Bits 7-4)
    Lcd_SendNibble(d >> 4);

    // Alçak Nibble'ý gönder (Bits 3-0)
    Lcd_SendNibble(d & 0x0F);
}

// LCD'yi temizler
void Lcd_Temizle(void){
    Lcd_Komut(0x01); // Display Clear komutu
    SysCtlDelay(500000); // Temizleme komutu uzun sürer (~1.52 ms). Daha uzun gecikme eklenmeli.
}

// Belirtilen konuma gider (Satýr x: 1 veya 2, Sütun y: 1-16)
void Lcd_Goto(char x, char y){
    if (x == 1) {
        Lcd_Komut(0x80 + ((y - 1) % 16)); // Satýr 1 baþlangýç adresi (0x80)
    } else {
        Lcd_Komut(0xC0 + ((y - 1) % 16)); // Satýr 2 baþlangýç adresi (0xC0)
    }
}

// LCD'ye bir dize yazdýrýr
void Lcd_Puts(char* s){
    while(*s) {
        Lcd_Putch(*s++);
    }
}

// LCD'yi baþlatýr ve yapýlandýrýr (4-bit mod için düzeltilmiþ)
void Lcd_init() {

    // 1. Çevresel Birimi (Peripheral) Etkinleþtirin
    SysCtlPeripheralEnable(LCDPORTENABLE);
    while(!SysCtlPeripheralReady(LCDPORTENABLE)); // Hazýr olana kadar bekleyin (Ýyi uygulama)

    // 2. Pinleri Çýkýþ Olarak Ayarlayýn
    GPIOPinTypeGPIOOutput(LCDPORT, ALL_PINS);

    SysCtlDelay(500000); // LCD'nin açýlýþ için bekleme süresi (~12.5 ms @ 80MHz). 15ms'den fazla olmalýdýr.

    // --- 3. LCD'yi 4-Bit Moduna Geçirme Dizisi (8-bit Handshake) ---
    GPIOPinWrite(LCDPORT, RS, 0x00); // RS = 0 (Tüm komutlar için)

    // Göndermek istediðimiz þey 0x30. Sadece yüksek nibble gönderilir, 4 bit kaydýrma gerekir.
    // 1. Adým: 0x3 (Fonksiyon Set) gönder
    GPIOPinWrite(LCDPORT, DATA_PINS_MASK, 0x30); // 0x3 << 4 = 0x30
    GPIOPinWrite(LCDPORT, E, E);
    SysCtlDelay(10);
    GPIOPinWrite(LCDPORT, E, 0x00);
    SysCtlDelay(50000);

    // 2. Adým: 0x3 (Fonksiyon Set) gönder
    GPIOPinWrite(LCDPORT, DATA_PINS_MASK, 0x30);
    GPIOPinWrite(LCDPORT, E, E);
    SysCtlDelay(10);
    GPIOPinWrite(LCDPORT, E, 0x00);
    SysCtlDelay(50000);

    // 3. Adým: 0x3 (Fonksiyon Set) gönder
    GPIOPinWrite(LCDPORT, DATA_PINS_MASK, 0x30);
    GPIOPinWrite(LCDPORT, E, E);
    SysCtlDelay(10);
    GPIOPinWrite(LCDPORT, E, 0x00);
    SysCtlDelay(50000);

    // 4. Adým: 4-bit moduna geçmek için 0x2 (Fonksiyon Set) gönder
    GPIOPinWrite(LCDPORT, DATA_PINS_MASK, 0x20); // 0x2 << 4 = 0x20
    GPIOPinWrite(LCDPORT, E, E);
    SysCtlDelay(10);
    GPIOPinWrite(LCDPORT, E, 0x00);
    SysCtlDelay(50000);

    // --- 4. 4-Bit Modunda Yapýlandýrma ---
    // Artýk Lcd_Komut fonksiyonu tam 8-bit komutlarý gönderebilir.
    Lcd_Komut(0x28); // Fonksiyon Ayarý: 4-bit, 2 satýr, 5x8 font
    Lcd_Komut(0x0C); // Display ON, Cursor OFF, Blink OFF
    Lcd_Komut(0x06); // Entry Mode Set: Ýmleç artýr, kaydýrma yok
    Lcd_Temizle();   // Ekraný temizle
    Lcd_Komut(0x80); // Ýmleci baþlangýca (0,0) getir (Home)
}
