#include <Wire.h>
#include <BetterTransferI2CMaster.h>
#define I2C_SLAVE_ADDRESS 9

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
unsigned long last_polled = 0;

struct GAME_STATE {
  // This will hold the state of the game, armed, TotalStrikes, edgework, etc.
  // This needs to be the same on all modules and the controller
  int two;
};

struct MODULE_STATE {
  // This will hold the state of the module that is shared between modules and the controller, ModuleStrikes, FinishedStage
  // This needs to be the same on all modules and the controller
  int one;

};


BetterTransferI2CMaster ET_GAME_STATE;
GAME_STATE game_state;

BetterTransferI2CMaster ET_MODULE_STATE;
MODULE_STATE module_state;


void setup() {
  Serial.begin(115200);
  Wire.begin();
  ET_GAME_STATE.begin(details(game_state), &Wire);
  ET_MODULE_STATE.begin(details(module_state), &Wire);

  inputString.reserve(10);
}
void loop() {

  if (stringComplete) {
    if (inputString == "one\n") {
      module_state.one++;
      Serial.print("One: ");
      Serial.println(module_state.one);
    }
    if (inputString == "two\n") {
      game_state.two++;
      Serial.print("Two: ");
      Serial.println(game_state.two);
    }
    ET_GAME_STATE.sendData(I2C_SLAVE_ADDRESS);
    ET_MODULE_STATE.sendData(I2C_SLAVE_ADDRESS);

    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}


void prep_setup() {

}

bool prep_loop() {
  // This is the loop that will run when the bomb is in its prep phase
  // Return true if the bomb is ready to be armed
  return false;
}



void game_setup() {

}

bool game_loop() {
  // This is the loop that will run when the bomb is in its game phase
  // Return true if the bomb is ready to be armed
  return false;
}

void disarmed() {

}

void win() {

}

void reset() {

}
