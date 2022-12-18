#include <Wire.h>
#include <BetterTransferI2CSlave.h>
#define I2C_SLAVE_ADDRESS 9


String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

enum Game_Phase {
  PREP,
  GAME
};

struct GAME_STATE {
  // This will hold the state of the game, armed, TotalStrikes, edgework, etc.
  // This needs to be the same on all modules and the controller
  int strikes;
  Game_Phase phase;
  int batteries;
  bool parallelPort;
  bool serialOdd;
  bool serialVowel;
};

struct MODULE_STATE {
  // This will hold the state of the module that is shared between modules and the controller, ModuleStrikes, FinishedStage
  // This needs to be the same on all modules and the controller
  int module_strikes;
  bool finished_stage;
  char module_name[4];
  uint8_t private_size;
};


BetterTransferI2CSlave ET_GAME_STATE;
GAME_STATE game_state;

BetterTransferI2CSlave ET_MODULE_STATE;
MODULE_STATE module_state;

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SLAVE_ADDRESS);
  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc.
  ET_GAME_STATE.begin(details(game_state), &Wire);
  ET_MODULE_STATE.begin(details(module_state), &Wire);

  inputString.reserve(10);
  Wire.onReceive(receive);
  Wire.onRequest(request);;
}

void loop() {
  // put your main code here, to run repeatedly:

  if (stringComplete) {
    if (inputString == "ready\n" || inputString == "disarm\n") {
      module_state.finished_stage = true;
    }
    if (inputString == "arm\n" || inputString == "unprepared\n") {
      module_state.finished_stage = false;
    }
    if (inputString == "strike\n") {
      module_state.module_strikes++;
    }

    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  
  if (ET_GAME_STATE.receiveData()) {
    Serial.print("Phase: ");
    Serial.println(game_state.phase);
    Serial.print("Strikes: ");
    Serial.println(game_state.strikes);
    Serial.print("Batteries: ");
    Serial.println(game_state.batteries);
    Serial.print("PP: ");
    Serial.println(game_state.parallelPort);
    Serial.print("Serial: ");
    if (game_state.serialOdd) {
      Serial.print("Odd, ");
    } else {
      Serial.print("Even, ");
    }
    if (game_state.serialVowel) {
      Serial.println("Vowel ");
    } else {
      Serial.println("No Vowel ");
    }
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
