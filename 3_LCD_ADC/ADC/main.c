/*
 * main.c
 * DÜZELTİLMİŞ VERSİYON
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h> // sprintf için
#include <math.h>  // logaritma için
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h" // Register tanımları için
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/gpio.c"
#include "driverlib/adc.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "Lcd.h"

void saatilkdegerler(int hour,int minute, int second);
void ilkayarlar();
void sicaklik_olc_ve_yaz(); // İsmini değiştirdim, sadece ölçüm yapmıyor yazıyor da
void ekrana_saat_yaz();     // Yeni fonksiyon
void ftimerkesmesi();

char zaman_str[16];
char sicaklik_str[16];

// Volatile: Kesme içinde değişen değişkenler için şarttır
volatile int sa, dk, sn;
volatile bool saniye_guncellendi = false; // Ana döngüye haber vermek için bayrak

// --- NTC SENSÖR AYARLARI ---
#define SERIES_RESISTOR     4700.0
#define NTC_NOMINAL         5000.0
#define BETA_VALUE          3950.0
#define VCC                 3.3

volatile uint32_t adc_degeri;
volatile float sicaklik_c;

int main(void)
{
    ilkayarlar();

    while(1)
    {
        // 1. Sıcaklığı Ölç ve Yaz
        sicaklik_olc_ve_yaz();

        // 2. Eğer saniye değiştiyse Saati Yaz (Böylece sürekli ekranı meşgul etmeyiz)
        if(saniye_guncellendi)
        {
            ekrana_saat_yaz();
            saniye_guncellendi = false; // Bayrağı indir
        }

        // İşlemciyi biraz rahatlat
        SysCtlDelay(SysCtlClockGet() / 10);
    }
}

void ilkayarlar()
{
    // 1. SAAT AYARI EN BAŞTA YAPILMALI
    SysCtlClockSet(SYSCTL_XTAL_16MHZ|SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN);

    // 2. Şimdi LCD ve diğer ayarlar
    LCD_Init();
    LCD_GotoXY(0, 0);
    LCD_Puts("Saat ve Sicaklik"); // Başlık

    saatilkdegerler(19, 15, 00);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // ADC ve Sensör Ayarları
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);

    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
    ADCIntClear(ADC0_BASE, 3);

    // Timer Ayarları
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()-1);

    IntMasterEnable();
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntRegister(TIMER0_BASE, TIMER_A, ftimerkesmesi);
    TimerEnable(TIMER0_BASE,TIMER_A);
}

void ftimerkesmesi()
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // LED Blink
    int a = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1);
    if(a & GPIO_PIN_1) GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
    else GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);

    // Sadece matematik işlemleri (LCD YOK!)
    sn++;
    if (sn >= 60) {
        sn = 0; dk++;
        if (dk >= 60) {
            dk = 0; sa++;
            if (sa >= 24) sa = 0;
        }
    }

    // Ana döngüye "Hey saniye değişti, ekranı güncelle" haberi ver
    saniye_guncellendi = true;
}

void ekrana_saat_yaz()
{
    LCD_GotoXY(0, 1); // Alt satır başı

    zaman_str[0] = (sa / 10) + '0';
    zaman_str[1] = (sa % 10) + '0';
    zaman_str[2] = ':';
    zaman_str[3] = (dk / 10) + '0';
    zaman_str[4] = (dk % 10) + '0';
    zaman_str[5] = ':';
    zaman_str[6] = (sn / 10) + '0';
    zaman_str[7] = (sn % 10) + '0';
    zaman_str[8] = '\0';

    LCD_Puts(zaman_str);
}

void sicaklik_olc_ve_yaz()
{
    ADCProcessorTrigger(ADC0_BASE, 3);
    while(!ADCIntStatus(ADC0_BASE, 3, false));
    ADCIntClear(ADC0_BASE, 3);
    ADCSequenceDataGet(ADC0_BASE, 3, (uint32_t*)&adc_degeri);

    float voltaj = (adc_degeri * VCC) / 4095.0;

    // Emniyet Kemerleri
    if (voltaj > 3.29) voltaj = 3.29;
    if (voltaj < 0.05) voltaj = 0.05;

    float ntc_direnci = (SERIES_RESISTOR * voltaj) / (VCC - voltaj);

    float steinhart;
    steinhart = ntc_direnci / NTC_NOMINAL;
    steinhart = log(steinhart);
    steinhart /= BETA_VALUE;
    steinhart += 1.0 / (25.0 + 273.15);
    steinhart = 1.0 / steinhart;
    sicaklik_c = steinhart - 273.15;

    int tam = (int)sicaklik_c;
    int ondalik = (int)((sicaklik_c - tam) * 100);
    if (ondalik < 0) ondalik *= -1;

    // Saatin yanına yazmak için kordinatı değiştirdik (9. sütun, 1. satır)
    LCD_GotoXY(9, 1);
    sprintf(sicaklik_str, " %d.%02dC", tam, ondalik); // "24.50C" gibi kısa format
    LCD_Puts(sicaklik_str);
}

void saatilkdegerler(int hour,int minute, int second)
{
    sa = hour;
    dk = minute;
    sn = second;
}
