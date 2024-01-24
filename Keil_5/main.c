#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "arch_system.h"
#include "uart.h"
#include "uart_utils.h"
#include "user_periph_setup.h"
#include "gpio.h"

void printbanner(void);
void control_LED(bool state);
void uart_tx(char print_str[]);
void i2c_read(uint8_t reg, uint32_t * bufp, uint16_t len);
void i2c_write(uint8_t reg, uint8_t * bufp, uint16_t len);
void chip_id_read_reg(void);


int main (void)
{
    system_init();
		printbanner();
		chip_id_read_reg();
	 
    while(1);
}

void uart_tx(char print_str[])
{
	uart_send(UART2,print_str, strlen(print_str), UART_OP_BLOCKING);
}

void i2c_read(uint8_t reg, uint32_t * bufp, uint16_t len) 
{
  i2c_abort_t abort_code;					//May be used for error checking
  i2c_master_transmit_buffer_sync( &reg, 1, &abort_code, I2C_F_NONE);
  i2c_master_receive_buffer_sync(bufp, len, &abort_code, I2C_F_NONE);
}

void i2c_write(uint8_t reg, uint8_t * bufp, uint16_t len) 
{
	i2c_abort_t abort_code;					//May be used for error checking
  i2c_master_transmit_buffer_sync( &reg, 1, &abort_code, I2C_F_NONE);
  i2c_master_transmit_buffer_sync(bufp, len, &abort_code, I2C_F_NONE);
}

void chip_id_read_reg()
{	
				
				uint32_t reg_val=0;
				uart_tx("I2C Start1!\n\r");
        i2c_read(0x00, &reg_val, 4);
}

void control_LED(bool state)
{
    if(state == true)
		{
        GPIO_SetActive(LED_PORT,LED_PIN);
		}
    else
		{
        GPIO_SetInactive(LED_PORT,LED_PIN);
		}
}

void printbanner(void) {

	uart_tx("\r\n ---------------");
	uart_tx("\r\n I2C DA1453X_DSP");
  uart_tx("\r\n ---------------\r\n");
}




