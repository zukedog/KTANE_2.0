#include <Wire.h>
#include <BetterTransferI2CSlave.h>
#define I2C_SLAVE_ADDRESS 9


String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete


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


BetterTransferI2CSlave ET_GAME_STATE;
GAME_STATE game_state;

BetterTransferI2CSlave ET_MODULE_STATE;
MODULE_STATE module_state;

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SLAVE_ADDRESS);
  ET_GAME_STATE.begin(details(game_state));
  ET_MODULE_STATE.begin(details(module_state));

  inputString.reserve(10);
  Wire.onReceive(BetterTransferI2CSlave::onReceive);
  Wire.onRequest(request);
}

void loop() {
  // put your main code here, to run repeatedly:

  
  if (ET_GAME_STATE.updateData()) {
    Serial.print("One: ");
    Serial.println(module_state.one);
    Serial.print("Two: ");
    Serial.println(game_state.two);

  }
  delay(1000);

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

// HIDE EVERYTHING ABOVE THIS LINE IN A LIBRARY LATER

struct PRIVATE_MODULE_STATE {
  // This will hold the state of the module that is specific to the one module, Colour of button, Number of wires, etc.
  // This will be different on every module including size.
  // make sure you define arrays like this "int stuff[5];" not like this "int stuff[];" it breaks if they are dynamically assigned
};

void receive(int numBytes) {}
void request() {
  ET_MODULE_STATE.sendData();
}
