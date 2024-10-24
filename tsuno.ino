// 8MHz operation!

void setup() {
  // Pins:
  // 0:OC0A - RGB output R
  // 1:OC0B - RGB output G
  // 4:OC1B - RGB output B
  // 3:PINB3 - digital input
  DDRB = 0b00010011;
  // pullup pin 3
  PORTB = 0b00001000;
  
  // save power
  PRR = (1 << PRUSI);

  // we want somewhere around 10kHz clocks for PWM. Exact frequency is not important though
  // set Timer0 Fast PWM, and both OC0A and OC0B to inverting output. The ability to turn the output fully off is important
  TCCR0A = (1 << WGM01) | (1 << WGM00) | (1 << COM0A1) | (1 << COM0A0)  |  (1 << COM0B1) | (1 << COM0B0);
  // set prescaler for Timer0 to /8, this gives 4kHz pulses, good enough
  TCCR0B = (1 << CS01);
  // set PWM mode for Timer1B and OC1B to inverting output
  GTCCR = (1 << PWM1B) | (1 << COM1B1) | (1 << COM1B0);
  // set prescaler for Timer1 to /8 as well
  TCCR1 = (1 << CS12);
  // a bug afflicting some ATTINY85s requires OC1A bits to be set as well for OC1B inverted to work
  TCCR1 |= (3 << COM1A0);
  OCR1C = 255;  // Set CTC match value for 8-bit resolution like Timer0

  // turn off outputs
  setRGB(0,0,0);

  selfTest();
}

void selfTest(){
  // output 7 non-mixed colors
  setRGB(255,0,0);
  delay(250);
  // setRGB(0,255,0);
  // delay(250);
  // setRGB(0,0,255);
  // delay(250);
  // setRGB(255,255,0);
  // delay(250);
  // setRGB(255,0,255);
  // delay(250);
  // setRGB(0,255,255);
  // delay(250);
  // setRGB(255,255,255);
  // delay(250);
  setRGB(0,0,0); 
}

// valid inputs are 0-255 to set a value, and anything outside that to no-op that color
const int16_t COLOR_NO_OP = -1; 
void setRGB(int16_t r, int16_t g, int16_t b){
    if (r >= 0 && r <= 255) {
        OCR0A = 255 - r; // Inverted output for R
    }
    if (g >= 0 && g <= 255) {
        OCR0B = 255 - g; // Inverted output for G
    }
    if (b >= 0 && b <= 255) {
        OCR1B = 255 - b; // Inverted output for B
    }
}

// output a red flash over about 1 second to mimic Nokotan's antlers
void blink() {
  for (int r = 1; r <= 255; r += 2){
    setRGB(r, COLOR_NO_OP, COLOR_NO_OP);
    delay(1);
  }
  for (int r = 255; r >= 0; r--){
    setRGB(r, COLOR_NO_OP, COLOR_NO_OP);
    delay(2);
  }
  delay(150);
}

void loop() {
  // is pin 3 pulled down by the switch?
  if (PINB & (1 << PINB3)){
    delay(250);
  } else {
    blink();
  }
}
