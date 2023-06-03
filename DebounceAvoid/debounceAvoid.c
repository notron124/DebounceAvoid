#include "debounceAvoid.h"

/*Header with ports definition, we need SND_ON/OFF macro*/
#include "port.h"

uint16_t keyFlags;
uint16_t autorepeatFlags;
uint8_t keyCode;

volatile uint8_t commonKeyFlags;

void DebounceAvoid(struct Key_TypeDef *keyx)
{
   if (!(keyx->GPIOx->IDR & keyx->pin))
   {
      keyx->counter++;

      if (keyx->longPressDelay != 0)
      {
         if (keyx->counter == keyx->longPressDelay) // first wait for longpress
         {
            if (keyx->flags.autorepeat == 1)
            {
#ifdef SND_DOUBLE_ON
               if (!KEY_AUTO_REPEAT)
                     SND_DOUBLE_ON;
#endif
               keyx->counter  = keyx->longPressDelay - 50;
            }
            else
            {
#ifndef SND_LONG_ON
#ifdef SND_ON
               SND_ON;
#endif
#else
               SND_LONG_ON;
#endif
            }

            keyCode = keyx->longPressID;
            SET_KEY_READY;
            keyx->flags.isPressed = 1;
         }
      }
   }
   else
   {
      if (((keyx->counter >= keyx->shortPressDelay) && !(keyx->flags.isPressed))) // if key is unpressed, check if time pressed is enough
      {
         keyx->counter = 0;
         keyCode = keyx->ID;
         SET_KEY_READY;
#ifdef SND_ON
         SND_ON;
#endif
         keyx->flags.isPressed = 1;
      }
      else // if not enough
      {
         //RESET_KEY_AUTO_REPEAT;
         keyx->counter = 0;
         keyx->flags.isPressed = 0;
      }
   }
}
