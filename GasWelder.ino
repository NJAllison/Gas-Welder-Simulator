/*
 * -----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <nelson.allison@aberdeencarshops.ca> wrote this file.  As long as you retain
 * this notice you can do whatever you want with this stuff. If we meet some day
 * , and you think this stuff is worth it, you can buy me a beer in return.
 * Nelson Allison
 * -----------------------------------------------------------------------------

  Sketch to simulate oxy-acetylene welding light with 3 LEDs (one red, one yellow, one white SMD led 0402)
  The red LED becomes brighter during the flickering process and with the end of the flickering is
  slowly dimmed again and thus simulates the annealing of the welded object) 
  This uses an Arduino Nano board, and uses the PWM outputs and will provide 2 sets of outputs for 2 separate welders
  1. Acetylene gas side of torch is turned on (low) and igniter (usually a flint striker) strikes igniting the acetylene gas. This produces a brief white flash of the igniter followed by the orange glow of the burning gas.
  2. The oxygen side of the torch is turned on and adjusted to produce the white-hot flame required for the welding or cutting operation. This changes the orange glow to a white light.
  3. During a welding or light-duty cutting process, variations in torch position and the material at work cause the visual effect of varying color shifts and intensity between yellow and white to occur. This is reproduced with subtle but noticeable changes to mimic this process
  4. While this occurs the area being welded or cut will start to slowly glow red as it heats.
  5. After approx. 15-20 seconds the torch is shut off.
  6. The heated area will slowly stop glowing.
  7. The process starts again.
*/
 
//Definition of input pins
const int TOUCH_BUTTON_PIN = 2;  // Input pin for touch state
const int TIMER_OR_BUTTON_PIN = 13;  // Timer or Button activated short to +5 common for Button to GND for Timer
// Definition of output pins
int ledYellowPin1 = 3; // Output Yellow LED
int ledWhitePin1 = 5; // Output White LED
int ledRedPin1 = 6; // Output Red LED PWM Pin
int ledYellowPin2 = 9; // Output Yellow LED
int ledWhitePin2 = 10; // Output White LED
int ledRedPin2 = 11; // Output Red LED PWM Pin
int WeldPin1 = 0;
int WeldPin2 = 0;
int GlowPin = 0;
// Definition of the output value for the welding light
byte flicker = 0;
byte flicker2 = 0;
byte adjustw = 0;
byte adjusty = 0;
byte adjustmax = 15;
byte adjustmin = 2;
byte ledflickermin = 25;
byte ledflickermax = 255;
byte ledflickermin1 = 25;
byte ledflickermax1 = 255;
// Definition of flicker Time
byte leddelay = 0;
byte leddelaymin = 15;
byte leddelaymax = 125;
// Definition of how many flicker before break
long flickertimes = 0;
long flickertimesmin = 100;
long flickertimesmax = 255;
// Definition length of break (in milliseconds)
long pause = 0;
long pausemin = 1000;
long pausemax = 3000;
// Definition of the default glow and delay
byte glow = 1;
byte glowdelay = 15;
// Global Variables
int buttonState = 0;             // Variable for reading button
int TorBState = 0;             // Variable for reading Timer or Button pin
long previousMillis_1 = 0;        // will store last time LED was updated
long interval_1 = 20000;           // interval at which to blink (milliseconds)
long previousMillis_2 = 0;        // will store last time LED was updated
long interval_2 = 40000;           // interval at which to blink (milliseconds)

void gas()
{
    analogWrite(WeldPin2, 254); // white LED igniter flash
    delay(600);
    analogWrite(WeldPin1, 154); // yellow LED igniter flash
    analogWrite(WeldPin2, 25); // Turns off the white LED
    delay(900);
    flickertimes = random(flickertimesmin, flickertimesmax); // Selects a random value for the number of flicker
    while(flickertimes > 0) // As long as the number of flicker more than 0 is repeat to the end of the while loop
    {
    flicker = random(ledflickermin, ledflickermax); // Chooses random flicker value
    analogWrite(WeldPin1, flicker); // Output Flicker value yellow LED
    flicker2 = random(ledflickermin1, ledflickermax1); // Chooses random flicker value
    analogWrite(WeldPin2, flicker2); // Output Flicker value white LED
    ++glow; // Add to Glow level 1
    analogWrite(GlowPin, glow); // Output Glow Red LED
    leddelay = random(leddelaymin, leddelaymax); // Select Random delay
    delay(leddelay); // Wait
    --flickertimes; // Reduces flickering time by a value of 1
    } // End of first  "while" loop
    analogWrite(WeldPin1, 0); // Turns off the yellow LED
    analogWrite(WeldPin2, 0); // Turns off the white LED
    while(glow > 0) // As long as the glow is greater than 0, repeat to the end of the while loop
    {
    analogWrite(GlowPin, glow); // Output annealing Red LED
    delay(glowdelay); // Wait
    --glow; // Reduces annealing by 1
    } // End of second "while" Loop
    pause = random(pausemin, pausemax); // Selects random delay for pause between welding operations
    delay (pause); // Waits
    analogWrite(GlowPin, 0); // Turns off the red LED
}
void setup()
{
  pinMode(WeldPin2, OUTPUT);
  pinMode(WeldPin1, OUTPUT);
  pinMode(GlowPin, OUTPUT);
  pinMode(TOUCH_BUTTON_PIN, INPUT);   // Configure button pin as input
  pinMode(TIMER_OR_BUTTON_PIN, INPUT);   // Configure timer or button pin as input
}
void loop(){
  TorBState = digitalRead(TIMER_OR_BUTTON_PIN);   // Read the state of the timer or button jumper
  if (TorBState == HIGH) {        // Button Mode Activated {
    buttonState = digitalRead(TOUCH_BUTTON_PIN);   // Read the state of the capacitive touch board
    if (buttonState == HIGH) {          // If a touch is detected, start welding
      WeldPin1 = ledYellowPin1; 
      WeldPin2 = ledWhitePin1; 
      GlowPin = ledRedPin1;
      gas(); 
      delay (pause);
      WeldPin1 = ledYellowPin2; 
      WeldPin2 = ledWhitePin2; 
      GlowPin = ledRedPin2;  
      gas();
    }
  }
  else {
    unsigned long currentMillis_1 = millis();
    if(currentMillis_1 - previousMillis_1 > interval_1) {
      previousMillis_1 = currentMillis_1;  
      WeldPin1 = ledYellowPin1; 
      WeldPin2 = ledWhitePin1; 
      GlowPin = ledRedPin1;
      gas();
    }
    else {
      analogWrite(WeldPin1, 0); // Turns off the Yellow LED
      analogWrite(WeldPin2, 0); // Turns off the white LED
      analogWrite(GlowPin, 0); // Turns off the red LED
    }     
    unsigned long currentMillis_2 = millis();
    if(currentMillis_2 - previousMillis_2 > interval_2) {
      previousMillis_2 = currentMillis_2; 
      WeldPin1 = ledYellowPin2; 
      WeldPin2 = ledWhitePin2; 
      GlowPin = ledRedPin2;  
      gas();
    }
    else {
      analogWrite(WeldPin1, 0); // Turns off the Yellow LED
      analogWrite(WeldPin2, 0); // Turns off the white LED
      analogWrite(GlowPin, 0); // Turns off the red LED
    }
   }  
}
