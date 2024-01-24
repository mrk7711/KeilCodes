/**
 ****************************************************************************************
 *
 * @file user_empty_peripheral_template.c
 *
 * @brief Empty peripheral template project source code.
 *
 * Copyright (C) 2012-2023 Renesas Electronics Corporation and/or its affiliates.
 * All rights reserved. Confidential Information.
 *
 * This software ("Software") is supplied by Renesas Electronics Corporation and/or its
 * affiliates ("Renesas"). Renesas grants you a personal, non-exclusive, non-transferable,
 * revocable, non-sub-licensable right and license to use the Software, solely if used in
 * or together with Renesas products. You may make copies of this Software, provided this
 * copyright notice and disclaimer ("Notice") is included in all such copies. Renesas
 * reserves the right to change or discontinue the Software at any time without notice.
 *
 * THE SOFTWARE IS PROVIDED "AS IS". RENESAS DISCLAIMS ALL WARRANTIES OF ANY KIND,
 * WHETHER EXPRESS, IMPLIED, OR STATUTORY, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. TO THE
 * MAXIMUM EXTENT PERMITTED UNDER LAW, IN NO EVENT SHALL RENESAS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE, EVEN IF RENESAS HAS BEEN ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGES. USE OF THIS SOFTWARE MAY BE SUBJECT TO TERMS AND CONDITIONS CONTAINED IN
 * AN ADDITIONAL AGREEMENT BETWEEN YOU AND RENESAS. IN CASE OF CONFLICT BETWEEN THE TERMS
 * OF THIS NOTICE AND ANY SUCH ADDITIONAL LICENSE AGREEMENT, THE TERMS OF THE AGREEMENT
 * SHALL TAKE PRECEDENCE. BY CONTINUING TO USE THIS SOFTWARE, YOU AGREE TO THE TERMS OF
 * THIS NOTICE.IF YOU DO NOT AGREE TO THESE TERMS, YOU ARE NOT PERMITTED TO USE THIS
 * SOFTWARE.
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdio.h>
#include "rwip_config.h"             // SW configuration
#include "gattc_task.h"
#include "app_api.h"
#include "user_empty_peripheral_template.h"
#include "gpio.h"
#include "arch_system.h"
#include "user_periph_setup.h"
#include "i2c.h"
#include "uart.h"
#include "uart_utils.h"

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
*/
/*
uint8_t last_led_state= 0;
void button_callback() 
{
    if (last_led_state== 0) 
    {
        last_led_state= 1;
        GPIO_SetActive(LED_PORT, LED_PIN);
    } else 
    {
        last_led_state= 0;
        GPIO_SetInactive(LED_PORT, LED_PIN);
    }
}




void control_LED(bool state)
{
    if(state == true)
		{
        GPIO_SetActive(LED_PORT,LED_PIN);
				//printf_string(UARTs, "\n\r *LED ON* ");
		}
    else
		{
        GPIO_SetInactive(LED_PORT,LED_PIN);
				//printf_string(UART, "\n\r *LED OFF* ");
		}
}

void i2c_write(uint8_t reg, uint8_t * bufp, uint16_t len) {
	i2c_abort_t abort_code;					//May be used for error checking
  i2c_master_transmit_buffer_sync( &reg, 1, &abort_code, I2C_F_NONE);
  i2c_master_transmit_buffer_sync(bufp, len, &abort_code, I2C_F_NONE);
}

void i2c_read(uint8_t reg, uint32_t * bufp, uint16_t len) {
  i2c_abort_t abort_code;					//May be used for error checking
  i2c_master_transmit_buffer_sync( &reg, 1, &abort_code, I2C_F_NONE);
  i2c_master_receive_buffer_sync(bufp, len, &abort_code, I2C_F_NONE);
}

void chip_id_read_reg()
{
				uint32_t reg_val=0;
				//while(reg_val != 0x31313831)
    //{
				//GPIO_SetActive(GPIO_PORT_0,GPIO_PIN_2);
        //i2c_read(0x00, &reg_val, 4);
				if(reg_val==0x31313831 )
					{
						control_LED(true);
					}
				if(reg_val!=0x31313831 )
					{
						control_LED(false);
					
					}
    //}
}
void my_timer_cb()
{
   control_LED(false);
}
*/

	//void DSP_reset()
	//{
	//		uint8_t reg_val = 0;
	//		i2c_read(0x12, &reg_val, 1);
		//	reg_val |= 0x01;
	//		i2c_write(0x12, &reg_val, 1);
	//}

void user_app_on_init(void)
{
		//control_LED(true);
		//chip_id_read_reg();
		//Default for Button Interrupt
		//GPIO_EnableIRQ(BUT_PORT, BUT_PIN, GPIO0_IRQn, true, true, 150);
		//GPIO_RegisterCallback(GPIO0_IRQn, button_callback);
    //DSP_reset();
    default_app_on_init();
}





void user_on_connection(uint8_t connection_idx, struct gapc_connection_req_ind const *param)
{
    default_app_on_connection(connection_idx, param);
		user_app_on_init();
		//app_easy_timer(200, my_timer_cb);
		//control_LED(true);
		//chip_id_read_reg();

}
 
void user_on_disconnect( struct gapc_disconnect_ind const *param )
{
    default_app_on_disconnect(param);
		//control_LED(false);
}


void user_catch_rest_hndl(ke_msg_id_t const msgid,
                          void const *param,
                          ke_task_id_t const dest_id,
                          ke_task_id_t const src_id)
{
    switch(msgid)
    {
        case GATTC_EVENT_REQ_IND:
        {
            // Confirm unhandled indication to avoid GATT timeout
            struct gattc_event_ind const *ind = (struct gattc_event_ind const *) param;
            struct gattc_event_cfm *cfm = KE_MSG_ALLOC(GATTC_EVENT_CFM, src_id, dest_id, gattc_event_cfm);
            cfm->handle = ind->handle;
            KE_MSG_SEND(cfm);
        } break;

        default:
            break;
    }
}



/// @} APP
