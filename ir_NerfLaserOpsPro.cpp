#include "IRremote.h"
#include "IRremoteInt.h"

//==============================================================================
//
//
//                              NERF LASER OPS PRO
//
//
//==============================================================================

#define BITS          15  // The number of bits in the command

#define HDR_MARK    2800  // The length of the Header:Mark
#define HDR_SPACE   6000  // The lenght of the Header:Space

#define ONE_MARK    1800  // The length of a Bit:Mark for 1's
#define ZERO_MARK    800  // The length of a Bit:Mark for 0's
#define ALL_SPACE   2100  // The length of a Bit:Space

#define purpule 0x440
#define red     0x400
#define blue    0x420

//+=============================================================================
//
#if SEND_NERF_LOP
void  IRsend::sendNerfLOP (unsigned long data,  int nbits)
{
	// Set IR carrier frequency
	enableIROut(40);

	// Header
	mark (HDR_MARK);
	space(HDR_SPACE);
  mark (HDR_MARK);

	// Data
	for (unsigned long  mask = 1UL << (nbits - 1);  mask;  mask >>= 1) {
		if (data & mask) {
      space(ALL_SPACE);
			mark (ONE_MARK);
		} else {
			space(ALL_SPACE);
			mark (ZERO_MARK);
		}
	}
  
  space(0);  // Always end with the LED off
}
#endif

//+=============================================================================
//
#if DECODE_NERF_LOP
bool  IRrecv::decodeNerfLOP (decode_results *results)
{
  //NOT IMPLEMENTED YET
  return false;
	unsigned long  data   = 0;  // Somewhere to build our code
	int            offset = 1;  // Skip the Gap reading

	// Check we have the right amount of data
	if (irparams.rawlen != 1 + 2 + (2 * BITS) + 1)  return false ;

	// Check initial Mark+Space match
	if (!MATCH_MARK (results->rawbuf[offset++], HDR_MARK ))  return false ;
	if (!MATCH_SPACE(results->rawbuf[offset++], HDR_SPACE))  return false ;

	// Read the bits in
	for (int i = 0;  i < SHUZU_BITS;  i++) {
		// Each bit looks like: MARK + SPACE_1 -> 1
		//                 or : MARK + SPACE_0 -> 0
		if (!MATCH_MARK(results->rawbuf[offset++], BIT_MARK))  return false ;

		// IR data is big-endian, so we shuffle it in from the right:
		if      (MATCH_SPACE(results->rawbuf[offset], ONE_SPACE))   data = (data << 1) | 1 ;
		else if (MATCH_SPACE(results->rawbuf[offset], ZERO_SPACE))  data = (data << 1) | 0 ;
		else                                                        return false ;
		offset++;
	}

	// Success
	results->bits        = BITS;
	results->value       = data;
	results->decode_type = SHUZU;
	return true;
}
#endif
