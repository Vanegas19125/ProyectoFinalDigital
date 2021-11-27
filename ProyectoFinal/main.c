/**
Jose Vanegas
Proyecto 3 Digital 2
 */
#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include "driverlib/pin_map.h"
#include <inc/hw_types.h>
#include <inc/hw_gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include "driverlib/uart.h"



uint8_t parqueosDisponibles;
uint8_t complementoParqueos;
uint8_t noParqueos;
uint8_t lucesParqueos;
int main(void)
{
    // configuracion: reloj con entrada principal de 16MHZ usando pll con una frecuencia de 40MHZ
    SysCtlClockSet(
    SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_SYSDIV_5);

    // conectando los puertos al reloj del sistema
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD); // ENTRADAS DE LOS SENSORES(DIP SWITCH)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); // SALIDA PARA LOS LEDS

    //configurar los pines
    HWREG(GPIO_PORTD_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE+GPIO_O_CR) |= GPIO_PIN_7;
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, 255);

    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_2 |
    GPIO_PIN_3 | GPIO_PIN_1); // las entradas de los sensores

    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_1 | GPIO_PIN_2 |
    GPIO_PIN_3 | GPIO_PIN_0,
                     GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU); // weak pull up

    /*GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1| GPIO_PIN_2 |
     GPIO_PIN_3 |GPIO_PIN_6 | GPIO_PIN_7, tabla[3]);


     GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, tabla[4]);*/
    // habilitar el uart0, los GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);

    //hacer los pines que sean controlados por uart
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    GPIOPinConfigure(   GPIO_PE4_U5RX);
    GPIOPinConfigure(GPIO_PE5_U5TX);
    // configurado a 115200 bauds sin paridad un bit de parada y largo de 8 bits
    UARTConfigSetExpClk(
            UART5_BASE, SysCtlClockGet(), 115200,
            UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
    UARTFIFOLevelSet(UART5_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8);
    UARTEnable(UART5_BASE);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    // PARTE 5
    //hacer los pines que sean controlados por uart
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    // configurado a 115200 bauds sin paridad un bit de parada y largo de 8 bits
    UARTConfigSetExpClk(
            UART0_BASE, SysCtlClockGet(), 115200,
            UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
    UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8);
    UARTEnable(UART0_BASE);




    GPIOPinWrite(GPIO_PORTB_BASE, 255, 255);
    while (1){

        lucesParqueos = 0;
        parqueosDisponibles = GPIOPinRead(GPIO_PORTD_BASE,
        GPIO_PIN_1 | GPIO_PIN_2 |
        GPIO_PIN_3 | GPIO_PIN_0);
        // se obtienen la cantidad de parqueos disponibles
        noParqueos = ((parqueosDisponibles ) & 1)
                + ((parqueosDisponibles >> 1) & 1)
                + ((parqueosDisponibles >> 2) & 1)
                + ((parqueosDisponibles >> 3) & 1);
        // indicadores para parqueos;
        if (parqueosDisponibles & 1)
            lucesParqueos |= 1;
        else
            lucesParqueos |= 2;

        if (parqueosDisponibles & 2)
            lucesParqueos |= 4;
        else
            lucesParqueos |= 8;

        if (parqueosDisponibles & 4)
            lucesParqueos |= 16;
        else
            lucesParqueos |= 32;

        if (parqueosDisponibles & 8)
            lucesParqueos |= 64;
        else
            lucesParqueos |= 128;

        //noParqueos = 4>>1;
        GPIOPinWrite(GPIO_PORTB_BASE, 255, lucesParqueos);

        SysCtlDelay(200000);
        // se envian los datos

         UARTCharPut(UART5_BASE,  ((parqueosDisponibles)&1) + 48 );
         UARTCharPut(UART5_BASE,  ',');
         UARTCharPut(UART5_BASE,  ((parqueosDisponibles>>1) &1) + 48 );
         UARTCharPut(UART5_BASE,  ',');
         UARTCharPut(UART5_BASE,  ((parqueosDisponibles>>2)  &1) + 48 );
         UARTCharPut(UART5_BASE,  ',');
         UARTCharPut(UART5_BASE,  ((parqueosDisponibles>>3)  &1) +48 );
         UARTCharPut(UART5_BASE,  '\n');


    }


    return 0;
}
