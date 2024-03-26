/******************************************************************************
**************************Setup and Primary Program****************************
* | @file       :	deej-custom.ino
* |	@version    :	V1.0
* | @date       :	2024-03-26
* | @author     : Daniel Nimsgern <nimsgernd>
* | @brief      :	Hardware Audio controler
* |             : 5 Sliders | 3 Knobs | 6 Buttons | 3 1.5" RGB OLED Modules
******************************************************************************/

/******************************************************************************
  Includes
******************************************************************************/

#include "OLED_Driver.h"
#include "GUI_paint.h"
#include "DEV_Config.h"
#include "Debug.h"
#include "ImageData.h"

/******************************************************************************
  Variables
******************************************************************************/
const int NUM_SLIDERS = 8;
const int analogInputs[NUM_SLIDERS] = {A0, A1, A2, A3, A4, A5, A6, A7};

const int NUM_BUTTONS = 6;
const int digitalInputs[NUM_BUTTONS] = {0, 1, 2, 3, 4, 5};

const int NUM_DISP = 3;
const int displaySelect[NUM_DISP] = {9, 10, 12};

int analogSliderValues[NUM_SLIDERS];
int digitalButtonValues[NUM_BUTTONS];

/******************************************************************************
  Initialize I/O and Serial Connections
******************************************************************************/

/**********************************************************
  @brief setup
         Initializes all necessary pins and systems.
**********************************************************/
void setup() { 
  /* Analog inputs */
  for (int i = 0; i < NUM_SLIDERS; i++)
  {
    pinMode(analogInputs[i], INPUT);
  }

  /* Digital Inputs */
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    pinMode(digitalInputs[i], INPUT_PULLDOWN);
  }

  /* OLED Connections */
  System_Init();
  if(USE_IIC) {
    Serial.print("Only USE_SPI_4W, Please revise DEV_config.h !!!");
    return 0;
  }

  Serial.print(F("OLED_Init()...\r\n"));
  for (int i = 0; i < NUM_DISP; i++)
  {
    OLED_1in5_rgb_Init(i);
    Driver_Delay_ms(500); 
    OLED_1in5_rgb_Clear(i);
  }

  /* Serial Connection */
  Serial.begin(115200);
}

/******************************************************************************
  Main Program
******************************************************************************/

/**********************************************************
  @brief loop
         Loops through main process.
**********************************************************/
void loop() {
  updateValues();
  sendValues(); // Actually send data (all the time)
  // printValues(); // For debug
  delay(10);
}

/******************************************************************************
  Functon Definitions
******************************************************************************/

/**********************************************************
  @brief updateValues
         Reads the analog pins to update the values.
**********************************************************/
void updateValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
     analogSliderValues[i] = analogRead(analogInputs[i]);
  }
}

/**********************************************************
  @brief sendValues
         Sends a string containing all read values to
         serial.
**********************************************************/
void sendValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += String((int)analogSliderValues[i]);

    if (i < NUM_SLIDERS - 1) {
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
  for (int i = 0; i < NUM_SLIDERS; i++) {
    String printedString = String("Slider #") + String(i + 1) + String(": ") + String(analogSliderValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_SLIDERS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}
