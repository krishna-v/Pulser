/***
  This program controls a Havells BLDC fan, cycling through the different speeds.
  The output pin, D1 is to be connected to the data pin of a 433Mhz RF transmitter module.
  The reverse engineered Havells remote protocol looks like the following:
  - A Sync pulse ~440us high followed by 784us low.
  - A 32-bit code, which looks like it is split into a 20 bit address and a 12 bit command.
  - A stop bit, which appears to be always '1'
  The code and stop bit repeat after a ~8ms gap for as long as the remote button is held down.
  0 is encoded as 784us high, followed by 264us low.
  1 is encoded as 264us high, followed by 784us low.
  The address part of the 32 bit code could potentially vary across remotes. I only have 1 remote control with me.
  For reference, the reverse engineered remote codes are:
  ON:       0xA235D1D5
  OFF:      0xA235D9CC
  Speed 1:  0xA235DAB8
  Speed 2:  0xA235D0A3
  Speed 3:  0xA235DB9B
  Speed 4:  0xA235DC8D
  Speed 5:  0xA235DDFB
  1H Timer: 0xA235D2E5
  2H Timer: 0xA235DEDA
  3H Timer: 0xA235D6C3
  4H Timer: 0xA235D8bA
***/
#include <pulser.h>

const int rfPin = D1;
const int LED = D0;
Pulser::Pulser pulser(rfPin);
Pulser::Sequence speed[5];

Pulser::Pulse sync(HIGH, 440, LOW, 784);
Pulser::Pulse zero(HIGH, 784, LOW, 264);
Pulser::Pulse one(HIGH, 264, LOW, 784);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  for(int i = 0; i < 5; i++) {
    speed[i].setZero(zero);
    speed[i].setOne(one);
  }
  speed[0].setBits("10100010001101011101101010111000", 32); // 0xA235DAB8
  speed[1].setBits("10100010001101011101000010100011", 32); // 0xA235D0A3
  speed[2].setBits("10100010001101011101101110011011", 32); // 0xA235DB9B
  speed[3].setBits("10100010001101011101110010001101", 32); // 0xA235DC8D
  speed[4].setBits("10100010001101011101110111111011", 32); // 0xA235DDFB
}

int curspeed = 0;

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Current Speed: ");
  Serial.println(curspeed+1);
  Serial.println("Sending Signal");
  digitalWrite(LED, LOW);
  pulser.sendPulse(sync); // Sync Pulse
  for(int i = 0; i < 8; i++) { // repeat the code 8 times, to simulate holding down the remote button for about .3 seconds.
    pulser.sendSequence(speed[curspeed]);
    pulser.sendPulse(speed[curspeed].one()); // stop bit
    delay(8); // 8ms between repeats.
  }
  digitalWrite(LED, HIGH);
  delay(10000); // sleep 10 seconds before changing the speed.
  curspeed++;
  if(curspeed >= 5) curspeed = 0;
}
