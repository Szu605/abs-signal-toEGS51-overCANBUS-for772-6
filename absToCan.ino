#include <SPI.h>
#include <mcp_can.h>

const float hi = 1.43374129; // Hi constant Transfer box on Hi + diff and wheel size
const float lo = 1.98968179; // Lo constant Transfer box on Lo + diff and wheel size
const float q = 2.467; // Q constant - 
int dec = 0;
MCP_CAN CAN0(10); // Set CS pin for MCP2515 CAN module

byte msgData200[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
byte msgData208[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
byte msgIndex = 0;
byte seqIndex = 0;
bool buttonPressed = false;
const int BUTTON_PIN = 5;
const int BUTTON_PINA = 6;
unsigned long buttonPressedTime = 0;
unsigned char seq;

volatile int pps = 0;
volatile int impulses = 0; // number of impulses
volatile unsigned long timeElapsed = 0; // time elapsed since last print
volatile unsigned long timeElapseda = 0;
const unsigned long PRINT_INTERVAL = 20; // print interval in ms
const unsigned long COUNT_INTERVAL = 1000; // count interval in ms

void setup() {
  Serial.begin(115200);
  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!");
  else Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be transmitted // put your setup code here, to run once:
  
  pinMode(5, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP); // Set pin 6 as input with internal pull-up resistor
  attachInterrupt(digitalPinToInterrupt(3), countImpulse, RISING); // attach interrupt to pin 3
}


byte getSequenceByte() {
  byte sequence[] = {0x00, 0x44, 0x48, 0x0C, 0x50, 0x14, 0x18, 0x5C, 0x60, 0x24, 0x28, 0x6c, 0x30, 0x74, 0x78, 0x3C}; // Brake pedal not pressed, sequence in byte 2 of message every 20ms
  byte seqByte = sequence[msgIndex];
  msgIndex = (msgIndex + 1) % 16;
  return seqByte;
}

byte getNextSequenceByte() {
  byte sequence[] = {0x41, 0x05, 0x09, 0x4D, 0x11, 0x55, 0x59, 0x1D, 0x21, 0x65, 0x69, 0x2D, 0x71, 0x35, 0x39, 0x7D}; // Brake pedal pressed, sequence in byte 2 of message every 20ms
  byte seqByte = sequence[seqIndex];
  seqIndex = (seqIndex + 1) % 16;
  return seqByte;
}

void loop() {
  
  unsigned long currentTime = millis(); // get current time

  if (currentTime - timeElapsed >= PRINT_INTERVAL) { // check if it's time to print
    Serial.println(impulses); // print impulses
    sendCANMessage(impulses); // send CAN message with impulses
    sendCANMessageb(impulses);
    timeElapsed = currentTime; // reset time elapsed
    
  }
  if (currentTime - timeElapseda >= COUNT_INTERVAL) { // check if it's time to count impulses
  timeElapseda = currentTime;
  impulses = round(pps * q * hi);
  pps = 0; // reset impulses
  }
handleButton(); // handle button state


}

void countImpulse() {
  pps++; // increment impulses
}

void sendCANMessage(int impulses) {
  unsigned char masgData200[8]; // CAN message data array
  msgData200[1] = seq;
  msgData200[2] = (impulses >> 8) & 0xFF; // put impulses in byte 1
  msgData200[3] = impulses & 0xFF; // put impulses in byte 2
  msgData200[4] = (impulses >> 8) & 0xFF; // put impulses in byte 1
  msgData200[5] = impulses & 0xFF; // put impulses in byte 2  
    for (int i = 0; i > 6;) { // set remaining data bytes to 0
    msgData200[i] = 0;
  }
  CAN0.sendMsgBuf(0x0200, 0, 8, msgData200); // send CAN message with ID 200 and 8 data bytes
}

void sendCANMessageb(int impulses) {
  unsigned char masgData208[8]; // CAN message data array
  msgData208[0] = 0;
  msgData208[1] = 40;
  msgData208[2] = 0;
  msgData208[3] = 0;
  msgData208[4] = (impulses >> 8) & 0xFF; // put impulses in byte 1
  msgData208[5] = impulses & 0xFF; // put impulses in byte 2  
  msgData208[6] = (impulses >> 8) & 0xFF; // put impulses in byte 1
  msgData208[7] = impulses & 0xFF; // put impulses in byte 2
  
    CAN0.sendMsgBuf(0x0208, 0, 8, msgData208); // send CAN message with ID 200 and 8 data bytes
}




void handleButton() {
  bool buttonState = digitalRead(BUTTON_PIN); // read button state
  if ( buttonState == LOW ) { // button pressed for less than 1 second
    seq = getSequenceByte();
    delay(20);
    } else { // button pressed for 1 second or more
    seq = getNextSequenceByte();
    delay(20);
    }
  }

  void hiLoButton() {
  bool buttonState = digitalRead(BUTTON_PINA); 
  if ( buttonState == LOW ) { 
    int dec = round(impulses * lo * q);
  
    } else { // button pressed for 1 second or more
    int dec = round(impulses * hi * q);
    
    }
  }
