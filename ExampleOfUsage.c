#include "stm8s.h"
#include "common.h"
#include "packet.h"
#include "glob.h"
#include "port.h"

void DebounceAvoidv2 (GPIO_TypeDef* GPIOx, u8 keyPin, u16 *keyx_cnt, u8 delay, u16 longPressTime, u8 keyID, u8 longPressID);

INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
{
	if(TIM4->SR1 & TIM4_SR1_UIF)
	{
		TIM4->SR1 &= ~TIM4_SR1_UIF;

                /*Лучше выставлять данный флаг в цикле while, когда аткивируется кнопка, 
                в которой нужна эта ункция, а при активации кнопки, 
                где данная функция не нужна, снимать флаг.*/
                SET_KEY_AUTOREPEAT;
                  
                DebounceAvoidv2(GPIOD, P_KEY1, &cntr_key1, 50, 0, 0, 5);
                DebounceAvoidv2(GPIOD, P_KEY2, &cntr_key2, 50, 0, 1, 5);
                DebounceAvoidv2(GPIOD, P_KEY3, &cntr_key3, 50, 0, 2, 5);
                DebounceAvoidv2(GPIOD, P_KEY4, &cntr_key4, 50, 1000, 3, 5);
                
        }
}
