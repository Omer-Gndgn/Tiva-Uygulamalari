#include <stdint.h>
#include <stdbool.h>
#include <stdio.h> // 1. EKLEME: sprintf için gerekli
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "inc/tm4c123gh6pm.h"
#include "Lcd.h" // 2. EKLEME: LCD kütüphanesi
#include "inc/hw_memmap.h"

// Fonksiyon prototipleri
void saatilkdegerler(int sa_init, int dk_init, int sn_init);
void ilkayarlar();
void ftimerkesmesi();

// Global Değişkenler
int sa, dk, sn;

// 3. EKLEME: Ekran güncelleme bayrağı (volatile olmalı çünkü kesme içinde değişiyor)
volatile bool ekran_guncelle = false;

// 4. EKLEME: Yazıyı saklayacağımız geçici hafıza (Buffer)
char zaman_yazisi[16];

int main(void)
{
    LCD_Init();
    ilkayarlar();

    // LCD'yi başlat


    // Ekrana sabit bir başlık yazalım
    LCD_GotoXY(0, 0); // 1. Satır
    LCD_Puts("Dijital Saat:");

    while(1)
    {
        int a = SysCtlClockGet();
        // 5. EKLEME: Bayrak kontrolü
        if (ekran_guncelle == true) {

            // Bayrağı indir (ki sürekli yazmasın, sadece saniye değişince yazsın)
            ekran_guncelle = false;

            // --- Formatlama İşlemi ---
            // %02d: Sayıyı en az 2 basamaklı yaz, boşsa başına 0 koy.
            // Örnek: 9 -> "09", 12 -> "12"

            // --- MANUEL FORMATLAMA ---

                    // 1. SAAT (HH)
                    zaman_yazisi[0] = (sa / 10) + '0'; // Onlar basamağı (Örn: 1)
                    zaman_yazisi[1] = (sa % 10) + '0'; // Birler basamağı (Örn: 2)

                    // 2. İKİ NOKTA
                    zaman_yazisi[2] = ':';

                    // 3. DAKİKA (MM)
                    zaman_yazisi[3] = (dk / 10) + '0';
                    zaman_yazisi[4] = (dk % 10) + '0';

                    // 4. İKİ NOKTA
                    zaman_yazisi[5] = ':';

                    // 5. SANİYE (SS)
                    zaman_yazisi[6] = (sn / 10) + '0';
                    zaman_yazisi[7] = (sn % 10) + '0';

                    // 6. BİTİŞ KARAKTERİ (Çok Önemli!)
                    // String'in bittiğini LCD_Puts'a söylemek zorundayız.
                    zaman_yazisi[8] = '\0'; // NULL karakter (veya 0)

            // --- LCD Yazma İşlemi ---
            LCD_GotoXY(0, 1); // 2. Satırın ortasına git
            LCD_Puts(zaman_yazisi); // Hazırladığımız stringi bas
        }
    }
}

void ilkayarlar()
{
    saatilkdegerler(23, 59, 50); // Test için 12:59:55 başlatalım

    SysCtlClockSet(SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ|SYSCTL_USE_PLL|SYSCTL_SYSDIV_5); // 200/5

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1); // Kırmızı LED

    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, 40000000-1); // 1 Saniye; 40 milyon saat sayarsak 1 saniye geçer

    IntMasterEnable();
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntRegister(TIMER0_BASE, TIMER_A, ftimerkesmesi);
    TimerEnable(TIMER0_BASE, TIMER_A);
}

void ftimerkesmesi()
{
    // Kesme bayrağını temizle
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // LED'i toggle et (Çalıştığını görmek için)
    int a = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, ~a);

    // --- Saat Mantığı ---
    sn++;
    if (sn == 60)
    {
        sn = 0; dk++;
        if (dk == 60)
        {
            dk = 0; sa++;
            if (sa == 24)
            {
                sa=0;
            }
        }
    }

    // 6. EKLEME: Ana döngüye "Ekranı güncelle!" haberi ver
    ekran_guncelle = true;
}

void saatilkdegerler(int sa_init, int dk_init, int sn_init)
{
    sa = sa_init;
    dk = dk_init;
    sn = sn_init;
}
