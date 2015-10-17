#include <Wiring.h>

#include <util/delay.h>

void delay(uint16_t ms)
{
  while ( ms-- ) _delay_ms(1);
}

int main(void) 
{
  // User defined setup routine
  setup();
  // User defined loop routine
  for (;;)
    loop();
}
