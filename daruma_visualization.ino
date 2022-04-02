//This program outputs waveforms to the serial monitor for later visualization
//Simply wait for the serial monitor output to stop and then copy the formatted output to a .csv
//It also outputs the waveforms on PWM pins to help compare them by eye
//Originally written for the arduino nano

//Gamma curve from:
//https://learn.adafruit.com/led-tricks-gamma-correction/the-longer-fix
uint8_t  gamma8[] = {
    0,  0,  0,  0,  1,  1,  1,  1,  1,  2,  2,  2,  3,  3,  3,  4,
    4,  4,  5,  5,  6,  6,  6,  7,  7,  8,  8,  9,  9, 10, 10, 11,
   11, 12, 12, 13, 14, 14, 15, 15, 16, 16, 17, 18, 18, 19, 20, 20,
   21, 21, 22, 23, 23, 24, 25, 26, 26, 27, 28, 28, 29, 30, 31, 31,
   32, 33, 34, 34, 35, 36, 37, 37, 38, 39, 40, 41, 41, 42, 43, 44,
   45, 46, 46, 47, 48, 49, 50, 51, 52, 53, 53, 54, 55, 56, 57, 58,
   59, 60, 61, 62, 63, 64, 65, 65, 66, 67, 68, 69, 70, 71, 72, 73,
   74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 88, 89, 90,
   91, 92, 93, 94, 95, 96, 97, 98, 99,100,102,103,104,105,106,107,
  108,109,110,112,113,114,115,116,117,119,120,121,122,123,124,126,
  127,128,129,130,132,133,134,135,136,138,139,140,141,142,144,145,
  146,147,149,150,151,152,154,155,156,158,159,160,161,163,164,165,
  167,168,169,171,172,173,174,176,177,178,180,181,182,184,185,187,
  188,189,191,192,193,195,196,197,199,200,202,203,204,206,207,209,
  210,211,213,214,216,217,218,220,221,223,224,226,227,228,230,231,
  233,234,236,237,239,240,242,243,245,246,248,249,251,252,254,255 };
  
//Variables used for the console output
uint16_t out_count = 0; // count how many times output has been written to console
uint16_t max_out = 255; // stop outputting after one full waveform output

//Variables used in pattern timing state machine
unsigned long currentMillis = millis();
unsigned long previousMillis = 0;
uint8_t millisInterval = 20; // Sets delay on pattern updates

uint8_t phase = 0; // keep track of the phase of the wave

void setup() {
  Serial.begin(115200);
}


void loop() {
  currentMillis = millis();
  
  if ( (currentMillis - previousMillis >= millisInterval) && (out_count < max_out) ) {
    
    output_LED(20); // output each waveform to PWM pins for visual comparison
    // output the current value of the waveform to serial monitor
    Serial.print( sin8(phase) );
    // or apply a gamma correction curve first like this:
    //Serial.print( gamma8[sin8(phase)]);
    Serial.println( "," );
    
    phase += 1; // move the waveform forward one increment
    previousMillis = currentMillis;
    out_count += 1; // increment output counter
    
  } 
  
}

//This function handles the output to LED
void output_LED(uint8_t interval){

  //output each different waveform type to a PWM pin
  analogWrite (3, triwave8(phase)); // a triangle wave
  analogWrite (5, gamma8[triwave8(phase)]);
  analogWrite (6, sin8(phase)); // a "true" sine wave output
  analogWrite (9, gamma8[sin8(phase)]);
  analogWrite (10, ease8InOutQuad(phase)); // a close approximation of a sine wave
  analogWrite (11, gamma8[ease8InOutQuad(phase)]);

}

//This function takes in a variable that increases from 0 to 255 and outputs
//a triangle wave. (Increasing from 0-127, decreasing from 128-255)
uint8_t triwave8(uint8_t i)
 {
    if( i & 0x80) {
         i = 255 - i;
     }
     uint8_t out = i << 1;

     //rescales wave to max PWM levels, lazy, I should improve the calc
     //out = map(out, 0, 255, 0, Palatis::SoftPWM.PWMlevels());
    return out;
}

//8bit sin implementation using sin() and floats, accurate but slow
//https://idyl.io/arduino/sine-pulse-led-pwm-trick/
uint8_t sin8(uint8_t i)
{
  uint8_t out = 255 * (.5 + .5 * sin( ( i / (255 / ( 2 * PI ) ) ) - ( .5 * PI ) ) ) ;
  return out;
}

//This function takes in a variable between 0 to 255 and outputs
//a quadratic approximation of a sine wave. 
//(Increasing from 0-127, decreasing from 128-255)
uint8_t ease8InOutQuad(uint8_t tri){
  //First, generate a triangle wave
  if(tri >= 128){
    tri = 255 - tri;
  }
  tri = tri << 1;

  //Next, convert the triangle wave into a sine wave
  uint8_t quadHalf = tri;
  if(quadHalf >= 128){
    quadHalf = 255 - quadHalf;
  }

  //Then ease the sine wave for a more even transition
  uint8_t quadFull  = (quadHalf * (quadHalf + 1)) / 256;
  uint8_t quadEased = quadFull << 1;
  if(tri >= 128) {
    quadEased = 255 - quadEased;
  }

  return quadEased;
}
