#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>  // sprintf için gerekli
#include <stdlib.h> // atoi fonksiyonu için
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h" // Interrupt ID'leri için
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"     // Timer kütüphanesi
#include "driverlib/interrupt.h" // Kesme kütüphanesi
#include "Lcd.h" // LCD kütüphanenizin projenize ekli olduğundan emin olun

// --- Fonksiyon Prototipleri ---
void ilkayar(void);
void Timerkesmesi(void);      // Zamanlayıcı Kesme Fonksiyonu
void UART_Puts(char *s);      // String gönderme fonksiyonu

// --- Global Değişkenler ---
// UART Alım (Receive) Değişkenleri
char rx_buffer[32];
int rx_index = 0;
bool veri_tamam = false;

// Zaman ve Gönderim (Transmit) Değişkenleri
volatile uint32_t saat = 12;      // Başlangıç saati
volatile uint32_t dakika = 0;
volatile uint32_t saniye = 0;
volatile bool saniye_doldu = false; // Main döngüsüne haber vermek için bayrak
char tx_buffer[32];               // Gönderilecek veri paketi için

int main(void)
{
    ilkayar();
    LCD_Init(); // LCD başlatma

    LCD_Clear();
    LCD_GotoXY(0, 0);
    LCD_Puts("Sistem Hazir");


    // Başlangıç mesajı gönder
    UART_Puts("Tiva RTC Baslatildi...\n");

    while(1)
    {
        // ============================================================
        // 1. GÖREV: ZAMANLAYICI (TIMER) KONTROLÜ VE EKRAN GÜNCELLEME
        // ============================================================
        if(saniye_doldu)
        {
            saniye_doldu = false; // Bayrağı indir

            // Saati string formatına çevir (Örn: "12:05:30")
            sprintf(tx_buffer, "%02d:%02d:%02d", saat, dakika, saniye);

            // --- LCD GÜNCELLEME (DÜZELTME BURADA YAPILDI) ---
            LCD_GotoXY(0, 1);    // İmleci alt satıra getir
            LCD_Puts(tx_buffer); // Yeni saati ekrana yaz
            // ------------------------------------------------

            // Bilgisayara gönder (UART için satır atlama '\n' ekleyerek gönderiyoruz)
            UART_Puts(tx_buffer);
            UART_Puts("\n");      // UART terminalinde alt satıra geçmesi için
        }

        // ============================================================
        // 2. GÖREV: UART VERİ ALMA (PC'den Mesaj Gelirse)
        // ============================================================
        if(UARTCharsAvail(UART0_BASE))
        {
            char c = UARTCharGet(UART0_BASE);

            if(c == '\n' || c == '\r')
            {
                rx_buffer[rx_index] = '\0';
                rx_index = 0;
                veri_tamam = true;
            }
            else
            {
                if(rx_index < 30)
                {
                    rx_buffer[rx_index++] = c;
                }
            }
        }

        // ============================================================
        // 3. GÖREV: GELEN VERİYİ İŞLEME VE SAATİ GÜNCELLEME
        // ============================================================
        if(veri_tamam)
        {
            veri_tamam = false;

            char komut_tipi = rx_buffer[0];

            if(rx_buffer[1] == ':')
            {
                // -- ÜST SATIR (MESAJ YAZDIRMA) --
                // Format: M:MERHABA
                if(komut_tipi == 'M')
                {
                    LCD_GotoXY(0, 0);
                    LCD_Puts("                "); // Temizle
                    LCD_GotoXY(0, 0);
                    LCD_Puts(&rx_buffer[2]);
                }
                // -- ALT SATIR (SAAT AYARLAMA) --
                // Format: S:14:30 veya S:14:30:45
                else if(komut_tipi == 'S')
                {
                    // 1. LCD'ye Yaz (Anlık geri bildirim için)
                    LCD_GotoXY(0, 1);
                    LCD_Puts("                ");
                    LCD_GotoXY(0, 1);
                    LCD_Puts(&rx_buffer[2]);

                    // 2. Global Değişkenleri Güncelle
                    saat = atoi(&rx_buffer[2]);   // 2. karakterden başla (Saati al)
                    dakika = atoi(&rx_buffer[5]); // 5. karakterden başla (Dakikayı al)

                    // Eğer saniye de gönderildiyse al
                    if(strlen(rx_buffer) > 8)
                    {
                        saniye = atoi(&rx_buffer[8]);
                    }
                    else
                    {
                        saniye = 0; // Saniye yoksa sıfırla
                    }
                }
            }
        }
    }
}

// --- UART String Gönderme Yardımcısı ---
void UART_Puts(char *s)
{
    while(*s)
    {
        UARTCharPut(UART0_BASE, *s++);
    }
}

// --- TIMER KESME FONKSİYONU (Her 1 saniyede otomatik çalışır) ---
void Timerkesmesi(void)
{
    // Kesme bayrağını temizle (Zorunlu)
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // Saati ilerlet
    saniye++;
    if(saniye >= 60) {
        saniye = 0;
        dakika++;
        if(dakika >= 60) {
            dakika = 0;
            saat++;
            if(saat >= 24) saat = 0;
        }
    }

    // Main döngüsüne haber ver
    saniye_doldu = true;
}

void ilkayar(void)
{
    // 1. Clock Ayarı (40 MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // 2. Peripherallerin Açılması
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);  // LCD Portu (B Portu varsayıldı)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); // Timer

    // 3. UART Ayarları
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    UARTEnable(UART0_BASE);

    // 4. Timer Ayarları (1 Saniye)
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()); // 1 saniye için clock hızı kadar saymalı

    // 5. Kesme (Interrupt) Ayarları
    TimerIntRegister(TIMER0_BASE, TIMER_A, Timerkesmesi); // Kesme fonksiyonunu kaydet
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);      // Timer kesmesini aç

    IntMasterEnable(); // İşlemci genel kesmelerini aç
    TimerEnable(TIMER0_BASE, TIMER_A); // Timer'ı başlat
}
