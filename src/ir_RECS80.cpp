#include "IRremote.h"
#include "IRremoteInt.h"

// RECS80
// Documentation : http://www.sbprojects.com/knowledge/ir/recs80.php

#define RECS80_MARK 158
#define RECS80_ONE_SPACE 7426
#define RECS80_ZERO_SPACE 4898 
#if SEND_RECS80

void IRsend::sendRECS80 (uint8_t address, uint8_t address_nbit, uint8_t command, uint8_t command_nbit) {
  // address: 1 - 3 bits
  // command: 1 - 7 bits

  // Set IR carrier frequency
  enableIROut(38);

  // Header
  mark(RECS80_MARK);
  space(RECS80_ONE_SPACE);

  // Address
  for (uint8_t mask = 1 << (address_nbit - 1); mask; mask >>=1) {
    mark(RECS80_MARK);

    if (address & mask) {
      space(RECS80_ONE_SPACE);
    } else {
      space(RECS80_ONE_ZERO);
    }

  }

  // Command
  for (uint8_t mask = 1 << (command_nbit - 1); mask; mask >>=1) {
    mark(RECS80_MARK);

    if (command & mask) {
      space(RECS80_ONE_SPACE);
    } else {
      space(RECS80_ONE_ZERO);
    }

  }
}


#endif // SEND_RECS80
