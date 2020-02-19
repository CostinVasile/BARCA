#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//  speed potentiometers pins
const int base__right_power_potentiometer = A0;
const int base__left_power_potentiometer = A1;

//  radio stuff
const uint64_t base__radio_pipe = 0xE8E8F0F0E1LL;
RF24 base__radio(9, 10);  //CSN and CE pins

struct RadioMessage{
  byte left_power;
  byte right_power;
  byte setup_byte;
};

RadioMessage base__payload;

void setup() {
  Serial.begin(9600);
  
  //  pins configuration
  pinMode(base__right_power_potentiometer, INPUT);
  pinMode(base__left_power_potentiometer, INPUT);

  //  payload init
  base__payload.left_power = 50;
  base__payload.right_power = 50;
  base__payload.setup_byte = 50;

  //  radio configuration
  base__radio.begin();
  base__radio.setAutoAck(false);
  base__radio.setDataRate(RF24_250KBPS);
  base__radio.openWritingPipe(base__radio_pipe);
}

void base__send_data(){
  base__radio.write(&base__payload, sizeof(RadioMessage));
}

void base__update_payload(){
  base__payload.right_power = map(analogRead(base__right_power_potentiometer), 0, 1023, 0, 255);
  base__payload.left_power = map(analogRead(base__left_power_potentiometer), 0, 1023, 0, 255);
}

void loop() {
  Serial.println(base__payload.left_power);
  base__update_payload();
  base__send_data();
//  delay(500);
}
