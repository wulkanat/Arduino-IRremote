#include "IRremote.h"
#include "IRremoteInt.h"

// RECS80
// Documentation : http://www.sbprojects.com/knowledge/ir/recs80.php

#define RECS80_MARK 158
#define RECS80_ONE_SPACE 7432
#define RECS80_ZERO_SPACE 4902
#define RECS80_BITS 12
#define RECS80_BITS_DATA 9
#define RECS80_ADDRESS_BITS 3
#define RECS80_COMMAND_BITS 6

#if SEND_RECS80

void IRsend::sendRECS80 (uint16_t data) {
  // Set IR carrier frequency
  enableIROut(38);

  // Header
  mark(RECS80_MARK);
  space(RECS80_ONE_SPACE);

  // Data: address and command
  for (uint16_t mask = 1 << (RECS80_COMMAND_BITS + RECS80_ADDRESS_BITS - 1); mask; mask >>=1) {
    mark(RECS80_MARK);

    if (data & mask) {
      space(RECS80_ONE_SPACE);
    } else {
      space(RECS80_ZERO_SPACE);
    }

  }

  mark(RECS80_MARK);
  space(0);
}


#endif // SEND_RECS80

#if DECODE_RECS80

bool IRrecv::decodeRECS80 (decode_results *results)
{
  long data = 0;
  long offset = 1;

  // Check that there is enough data
  if (irparams.rawlen < (2 * RECS80_BITS) - 2) {

    DBG_PRINTLN("Not long enough");
    DBG_PRINTLN(irparams.rawlen);

    return false;
  }

  // Initial mark and space
  if (!MATCH_MARK(results->rawbuf[offset++], RECS80_MARK)) return false;
  if (!MATCH_SPACE(results->rawbuf[offset++], RECS80_ONE_SPACE)) return false;

  // Data: Address and command stored as one variable
  for (int i = 0; i < (RECS80_ADDRESS_BITS + RECS80_COMMAND_BITS); i++) {
    if (!MATCH_MARK(results->rawbuf[offset++], RECS80_MARK)) return false;

    // One
    if (MATCH_MARK(results->rawbuf[offset], RECS80_ONE_SPACE))
      data = (data << 1) | 1; 

    // Zero
    else if (MATCH_MARK(results->rawbuf[offset], RECS80_ZERO_SPACE))
      data = (data << 1) | 0;

    else return false;
    offset++;
  }

  if (!MATCH_MARK(results->rawbuf[offset], RECS80_MARK)) return false;

  // Success
  results->decode_type = RECS80;
  results->bits = RECS80_BITS_DATA;
  results->value = data;

  return true;
  

}

#endif
