#include "debounceAvoid.h"

/*Header with your flags, we need KEY_READY flags*/
#include "common.h"

/*Header with your global variables, for KEY_READY flags, SND macro*/
#include "glob.h"

/*Header with ports definition, we need SND_ON/OFF macro*/
#include "port.h"

uint8_t keyCode;

volatile uint8_t commonKeyFlags;

void DebounceAvoid(struct Key_TypeDef *keyx, struct Keys_Properties *keysProperties)
{
   if (keysProperties->autorepeatSpeed > 99) // For speed limitation
      keysProperties->autorepeatSpeed = 99;

   if (!(keyx->GPIOx->IDR & keyx->pin))
   {
      keyx->counter++;

      if (keysProperties->longPressDelay != 0)
      {
         if (keyx->counter == keysProperties->longPressDelay) // first wait for longpress
         {
            if (keyx->flags.autorepeat == 1)
            {
#ifdef SND_DOUBLE_ON
               SND_DOUBLE_ON;
#endif
               keyx->counter  = keysProperties->longPressDelay - (100 - keysProperties->autorepeatSpeed);
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
            keyx->flags.isPressed = 1; // Set key pressed, so it not triggers again on release
         }
      }
   }
   else
   {
      if (((keyx->counter >= keysProperties->shortPressDelay) && !(keyx->flags.isPressed))) // if key is unpressed, check if time pressed is enough
      {
         keyx->counter = 0;
         keyCode = keyx->shortPressID;
         SET_KEY_READY;
#ifdef SND_ON
         SND_ON;
#endif
         keyx->flags.isPressed = 1;
      }
      else // if not enough
      {
         keyx->counter = 0;
         keyx->flags.isPressed = 0;
      }
   }
}