/*******************************************************************************
**************************Setup and Primary Program*****************************
* | @file       :	deej-custom.ino
* |	@version    :	V1.0
* | @date       :	2024-03-26
* | @author     : Daniel Nimsgern <nimsgernd>
* | @brief      :	Hardware Audio controller
* |             : 10 Sliders | 2 Knobs | 12 Mute Buttons | 16 Macro Buttons |
* |             : 12 LED Mute indicators | 4 0.96" Monochrome SPI OLED Modules
*******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Keyboard.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Analog Inputs */
#define NUM_POTS      (int) 12
  /*****************
   | Master : A0
   | Mic    : A1
   | Faders : A2-A11
   *****************/
static const int analogPotInputs[NUM_POTS] = {A0, A1, A2, A3, A4, A5, A6, A7,
                                              A8, A9, A10, A11};

/* Digital Inputs */
#define NUM_MUTE_BUTTONS  (int) 12
#define NUM_MACRO_BUTTONS (int) 16
  /**************************
   | Master Mute : 68
   | Mic Mute    : 69
   | Fader Mutes : 6-15
   | Macro 1-16  : 22-52 even
   **************************/
static const int digitalMuteButtonInputs[NUM_MUTE_BUTTONS] = {68, 69, 6, 7,
                                                              8, 9, 10, 11, 12,
                                                              13, 14, 15};
static const int digitalMacroButtonInputs[NUM_MACRO_BUTTONS] = {22, 24, 26, 28,
                                                                30, 32, 34, 36,
                                                                38, 40, 42, 44,
                                                                46, 48, 50, 52
                                                                };

/* LED Pins */
#define NUM_LEDS      (int) 12
  /********************
   | Master : 23
   | Mic    : 25
   | Faders : 27-45 odd
   ********************/
static const int digitalLEDOutputs[NUM_LEDS] = {23, 25, 27, 29, 31, 33, 35, 37,
                                                39, 41, 43, 45};

/* OLED Values */
#define SCREEN_WIDTH  (int) 128 // OLED display width, in pixels
#define SCREEN_HEIGHT (int) 64 // OLED display height, in pixels

#define NUM_DISPLAYS  (int) 4
  /******************
   | OLED Reset : 0
   | OLED DC    : 1
   | OLED CS    : 2-5
   ******************/
#define OLED_RESET    (int) 0
#define OLED_DC       (int) 1
static const int oledCS[NUM_DISPLAYS] = {2, 3, 4, 5};

/* Program Data */
static int analogPotValues[NUM_POTS];
static bool digitalMuteButtonValues[NUM_MUTE_BUTTONS];
static Adafruit_SSD1306 display[NUM_DISPLAYS];

/*******************************************************************************
 * Initialize I/O and Serial Connections
 ******************************************************************************/

/**********************************************************
  @brief setup
         Initializes all necessary pins and systems.
**********************************************************/
void setup() {
  /* Serial Connection */
  Serial.begin(115200);

  /* Analog Inputs */
  analogReadResolution(12);
  for (int i = 0; i < NUM_POTS; i++)
  {
    pinMode(analogPotInputs[i], INPUT);
  }

  /* Button Inputs */
  if (NUM_POTS != NUM_MUTE_BUTTONS)
  {
    Serial.println("Number of potentiameters is not equal to the number of mute buttons. This may have unexpeced results.\n");
  }
  for (int i = 0; i < NUM_MUTE_BUTTONS; i++)
  {
    pinMode(digitalMuteButtonInputs[i], INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(digitalMuteButtonInputs[i]),
                    muteButtonPress, FALLING);
  }

  Keyboard.begin(KeyboardLayout_en_US);
  for (int i = 0; i < NUM_MACRO_BUTTONS; i++)
  {
    pinMode(digitalMacroButtonInputs[i], INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(digitalMuteButtonInputs[i]),
                    macroButtonPress, FALLING);
  }

  /* LED Outputs */
  if (NUM_MUTE_BUTTONS != NUM_LEDS)
  {
    Serial.println("Number of LEDs is not equal to the number of mute buttons. This may have unexpeced results.\n");
  }
  for (int i = 0; i < NUM_LEDS; i++)
  {
    pinMode(digitalLEDOutputs[i], OUTPUT);
  }

  /* OLED Connections */
  for (int i = 0; i < NUM_DISPLAYS; i++)
  {
    display[i] = (SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_DC, OLED_RESET,
                  oledCS[i]);
    if(!display[i].begin(SSD1306_SWITCHCAPVCC))
    {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever
    }
  }
}

/*******************************************************************************
 * Main Program
 ******************************************************************************/

/**********************************************************
  @brief loop
         Loops through main process.
**********************************************************/
void loop() {
  updateValues();
  updateLEDs();
  sendValues(); // Actually send data (all the time)
  // printValues(); // For debug
  delay(10);
}

/*******************************************************************************
 * Functon Definitions
 ******************************************************************************/

/**********************************************************
  @brief updateValues
         Reads the analog pins to update the values.
**********************************************************/
void updateValues() {
  for (int i = 0; i < NUM_POTS; i++) {
     if (digitalMuteButtonValues[i])
     {
      analogPotValues[i] = 0;
     }
     else
     {
      analogPotValues[i] = analogRead(analogPotInputs[i]);
     }
  }
}

/**********************************************************
  @brief updateLEDs
         Sets LED states based on the mute button states.
**********************************************************/
void updateLEDs()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    if(digitalMuteButtonValues[i])
    {
      digitalWrite(digitalLEDOutputs[i], HIGH);
    }
    else
    {
      digitalWrite(digitalLEDOutputs[i], LOW);
    }
  }
}

/**********************************************************
  @brief sendValues
         Sends a string containing all read values to
         serial.
**********************************************************/
void sendValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_POTS; i++) {
    builtString += String((int)analogPotValues[i]);

    if (i < NUM_POTS - 1) {
      builtString += String("|");
    }
  }
  
  Serial.println(builtString);
}

/**********************************************************
  @brief printValues
         Prints the read values to the serial console.
**********************************************************/
void printValues() {
  for (int i = 0; i < NUM_POTS; i++) {
    String printedString = String("Slider #") + String(i + 1) + String(": ") +
                           String(analogPotValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_POTS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}

/*******************************************************************************
 * Interrupts
 ******************************************************************************/

/**********************************************************
  @brief muteButtonPress
         Interrupt to record when a mute button has been
         pressed.
**********************************************************/
void muteButtonPress()
{
  for (int i = 0; i < NUM_MUTE_BUTTONS; i++)
  {
    if (digitalRead(digitalMuteButtonInputs[i]) == LOW)
    {
      digitalMuteButtonValues[i] != digitalMuteButtonValues[i];
    }
  }
}

/**********************************************************
  @brief macroButtonPress
         Interrupt to record when a macro button has been
         pressed.
**********************************************************/
void macroButtonPress()
{
  int buttonNumber;
  for (int i = 0; i < NUM_MACRO_BUTTONS; i++)
  {
    if(digitalRead(digitalMacroButtonInputs[i]) == LOW)
    {
      switch(i)
      {
        // Add aditional macro stwitch statments as desired;
        default :
        {
          Serial.write("No macro mapped to this button.\n");
        }
      }
    }
  }
}
