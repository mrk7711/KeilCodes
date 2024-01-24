

#include "user_periph_setup.h"
#include "datasheet.h"
#include "system_library.h"
#include "rwip_config.h"
#include "gpio.h"
#include "uart.h"
#include "syscntl.h"
#include "i2c.h"

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */


#if DEVELOPMENT_DEBUG



void GPIO_reservations(void)
{
/*
    i.e. to reserve P0_1 as Generic Purpose I/O:
    RESERVE_GPIO(DESCRIPTIVE_NAME, GPIO_PORT_0, GPIO_PIN_1, PID_GPIO);
*/

#if defined (CFG_PRINTF_UART2)
    RESERVE_GPIO(UART2_TX, UART2_TX_PORT, UART2_TX_PIN, PID_UART2_TX);
#endif

#if !defined (__DA14586__)
    //RESERVE_GPIO(SPI_EN, SPI_EN_PORT, SPI_EN_PIN, PID_SPI_EN);
		//RESERVE_GPIO(LED, LED_PORT, LED_PIN, PID_GPIO);
		//RESERVE_GPIO(LED2, LED2_PORT, LED2_PIN, PID_GPIO);
		//RESERVE_GPIO(BUTTON, BUT_PORT, BUT_PIN, PID_GPIO);
		RESERVE_GPIO(SDA, I2C_SDA_PORT, I2C_SDA_PIN, PID_I2C_SDA);
		RESERVE_GPIO(SCL, I2C_SCL_PORT, I2C_SCL_PIN, PID_I2C_SCL);
		RESERVE_GPIO(UART2_TX, UART2_TX_PORT, UART2_TX_PIN, PID_UART2_TX);
		//RESERVE_GPIO(UART2_RX, UART2_RX_PORT, UART2_RX_PIN, PID_UART2_RX);
	
#endif
}
#endif

void set_pad_functions(void)
{
/* 
    i.e. to set P0_1 as Generic purpose Output:
    GPIO_ConfigurePin(GPIO_PORT_0, GPIO_PIN_1, OUTPUT, PID_GPIO, false);
*/

#if defined (__DA14586__)
    // Disallow spontaneous DA14586 SPI Flash wake-up
    GPIO_ConfigurePin(GPIO_PORT_2, GPIO_PIN_3, OUTPUT, PID_GPIO, true);
#else
    // Disallow spontaneous SPI Flash wake-up
    //GPIO_ConfigurePin(SPI_EN_PORT, SPI_EN_PIN, OUTPUT, PID_SPI_EN, true);
		//GPIO_ConfigurePin(LED_PORT, LED_PIN, OUTPUT, PID_GPIO, false);
		//GPIO_ConfigurePin(LED2_PORT, LED2_PIN, OUTPUT, PID_GPIO, false);
		//GPIO_ConfigurePin(BUT_PORT, BUT_PIN, INPUT_PULLUP, PID_GPIO, false);
	  GPIO_ConfigurePin(I2C_SDA_PORT, I2C_SDA_PIN, INPUT, PID_I2C_SDA, true);
		GPIO_ConfigurePin(I2C_SCL_PORT, I2C_SCL_PIN, OUTPUT, PID_I2C_SCL, true);
		GPIO_ConfigurePin(UART2_TX_PORT, UART2_TX_PIN, OUTPUT, PID_UART2_TX, false);
		//GPIO_ConfigurePin(UART2_RX_PORT, UART2_RX_PIN, OUTPUT, PID_UART2_RX, false);
#endif

#if defined (CFG_PRINTF_UART2)
    // Configure UART2 TX Pad
    GPIO_ConfigurePin(UART2_TX_PORT, UART2_TX_PIN, OUTPUT, PID_UART2_TX, false);
#endif

}



// Configuration struct for UART2
static const uart_cfg_t uart_cfg = {
    .baud_rate = UART2_BAUDRATE,
    .data_bits = UART2_DATABITS,
    .parity = UART2_PARITY,
    .stop_bits = UART2_STOPBITS,
    .auto_flow_control = UART2_AFCE,
    .use_fifo = UART2_FIFO,
    .tx_fifo_tr_lvl = UART2_TX_FIFO_LEVEL,
    .rx_fifo_tr_lvl = UART2_RX_FIFO_LEVEL,
    .intr_priority = 2,
};


void periph_init(void)
{
#if defined (__DA14531__)
    // In Boost mode enable the DCDC converter to supply VBAT_HIGH for the used GPIOs
    syscntl_dcdc_turn_on_in_boost(SYSCNTL_DCDC_LEVEL_3V0);
#else
    // Power up peripherals' power domain
    SetBits16(PMU_CTRL_REG, PERIPH_SLEEP, 0);
    while (!(GetWord16(SYS_STAT_REG) & PER_IS_UP));
    SetBits16(CLK_16M_REG, XTAL16_BIAS_SH_ENABLE, 1);
#endif

		//configuration of the I2C peripheral
		static const i2c_cfg_t i2c_cfg = {
  .clock_cfg.ss_hcnt = I2C_SS_SCL_HCNT_REG_RESET,
  .clock_cfg.ss_lcnt = I2C_SS_SCL_LCNT_REG_RESET,
  .clock_cfg.fs_hcnt = I2C_FS_SCL_HCNT_REG_RESET,
  .clock_cfg.fs_lcnt = I2C_FS_SCL_LCNT_REG_RESET,
  .restart_en = I2C_RESTART_ENABLE,
  .speed = I2C_SPEED_MODE,
  .mode = I2C_MODE_MASTER,
  .addr_mode = I2C_ADDRESS_MODE,
  .address = I2C_SLAVE_ADDRESS,
  .tx_fifo_level = 1,
  .rx_fifo_level = 1,
};
			
    // ROM patch
    patch_func();

    // Initialize peripherals

    // Set pad functionality
    set_pad_functions();

    // Initialize UART2
    uart_initialize(UART, &uart_cfg);

		// Initialize the I2C module
		i2c_init(&i2c_cfg);

    // Enable the pads
    GPIO_set_pad_latch_en(true);
}
