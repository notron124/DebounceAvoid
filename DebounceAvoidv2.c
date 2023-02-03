/*Your main header file (like stm8s.h)*/
//#include "stmxxxxx.h"

/*Your file with flags, most importantly KEY_READY and KEY_AUTOREPEAT flags*/
//#include "common.h"

/*Your header with global variables, key_code and flag holders in this case */
//#include "glob.h"

/*Your header with ports, only SND_ON used here*/
//#include "port.h"

u8 keyFlags;

void DebounceAvoidv2 (GPIO_TypeDef* GPIOx, u8 keyPin, volatile u16 *keyx_cnt, u8 delay, u16 longPressTime, u8 keyID, u8 longPressID)
{
  if (!(GPIOx->IDR & keyPin))
  {
    (*keyx_cnt)++;
    if (longPressTime != 0)
    {
      if ((*keyx_cnt) == longPressTime)
      { 
        if (KEY_AUTOREPEAT)
        {
          (*keyx_cnt) = longPressTime - 50;
#ifdef SND_ON
          SND_ON;
#endif
        }
        else
        {
          (*keyx_cnt) = longPressTime;    		
#ifndef SND_LONG_ON
          SND_ON;
#else
          SND_LONG_ON;
#endif
          
        }
            
        key_code = longPressID;
        SET_KEY_READY;
        keyFlags &= ~keyPin;
      }
    }
  }
  else
  {
    if (((*keyx_cnt) >= delay) && (keyFlags & keyPin))
    {
      (*keyx_cnt) = 0;
      key_code = keyID;
      SET_KEY_READY;
#ifdef SND_ON
      SND_ON;
#endif
      keyFlags &= ~keyPin;
    }
    else
    {
      (*keyx_cnt) = 0;
      keyFlags |= keyPin;
    }
  }
}
