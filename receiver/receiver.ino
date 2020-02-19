#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//  power pwm pins
const int boat__right_motor = 5;
const int boat__left_motor = 3;

// operating mode values
bool boat__autopilot_mode = false;
bool boat__reverse_drive = false;

// radio stuff
const uint64_t boat__radio_pipe = 0xE8E8F0F0E1LL;
RF24 boat__radio(9, 10);  //CSN and CE pins

struct RadioMessage{
  byte left_power;
  byte right_power;
  byte setup_byte;
};

RadioMessage boat__payload;

void setup() {
  Serial.begin(9600);

  //  pins configuration
  pinMode(boat__left_motor, OUTPUT);
  pinMode(boat__right_motor, OUTPUT);
  
  //  payload init
  boat__payload.left_power = 50;
  boat__payload.right_power = 50;
  boat__payload.setup_byte = 50;

  //  radio configuration
  boat__radio.begin();
  boat__radio.setAutoAck(false);
  boat__radio.setDataRate(RF24_250KBPS);  
  boat__radio.openReadingPipe(1,boat__radio_pipe);

  //  radio start listening
  boat__radio.startListening();
}

void boat__receive_data(){
  while(boat__radio.available()){
    boat__radio.read(&boat__payload, sizeof(RadioMessage));
    Serial.println(boat__payload.left_power);
  }
}

void boat__drive(byte left_power, byte right_power, bool reverse){
  if(left_power > 20)
    analogWrite(boat__left_motor, left_power);
   else 
    analogWrite(boat__left_motor, 0);

  if(right_power > 20)
    analogWrite(boat__right_motor, right_power);
   else 
    analogWrite(boat__right_motor, 0);
}

void boat__decode_setup_byte(){
  boat__autopilot_mode = bitRead(boat__payload.setup_byte, 0);
  boat__reverse_drive = bitRead(boat__payload.setup_byte, 1);
}

void loop() {
  boat__receive_data();
  if( !boat__autopilot_mode ){  //  MANUAL MODE
    boat__decode_setup_byte();
    boat__drive(boat__payload.left_power, boat__payload.right_power, boat__reverse_drive);
    
  }

  else{ //  AUTOPILOT MODE
    
  }
}
