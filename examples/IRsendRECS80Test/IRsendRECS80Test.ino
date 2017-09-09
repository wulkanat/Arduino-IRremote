
#include <IRremote.h>

IRsend irsend;

void setup()
{
  delay(2000);
}

void loop() {
  for (int i = 0; i < 512; i++) {
    irsend.sendRECS80(i);
    delay(5000); //5 second delay between each signal burst
  }
}
