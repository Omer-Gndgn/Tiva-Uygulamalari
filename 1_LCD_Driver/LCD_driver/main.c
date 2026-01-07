#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
// Kendi yazdığımız kütüphaneyi dahil ediyoruz
#include "lcd.h"

int main(void) {

    // ÇOK ÖNEMLİ: Kütüphanedeki gecikme (delay) fonksiyonlarının
    // çalışması için Tiva C'nin saat hızını ayarlamanız GEREKİR.
    // Örn: 80Mhz için:
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // LCD kütüphanemizin başlatma fonksiyonunu çağırıyoruz
    LCD_Init();

    // 1. satıra git ve yaz
    LCD_GotoXY(0, 0);
    LCD_Puts("Tiva C LCD");

    // 2. satıra git ve yaz
    LCD_GotoXY(0, 1);
    LCD_Puts("Test Basarili!");

    while(1)
    {
        // Döngü
    }
}
