#include <Wire.h>
#include <BetterTransferI2CMaster.h>
#define I2C_SLAVE_ADDRESS 9

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
unsigned long last_polled = 0;

enum Game_Phase {
  PREP,
  GAME,
  WIN,
  LOSE
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

class Module {

  private:


  public:
    Module(byte i2c_address, GAME_STATE &gState) {
      address = new BT_Address(i2c_address, &Wire);
      BT_Game = new BetterTransferI2CMaster(details(gState), address);
      BT_Module = new BetterTransferI2CMaster(details(state), address);
      //BT_PrivateModule(uint8_t *, uint8_t, BT_Address *);
    }
    BT_Address * address;
    MODULE_STATE state;
    BetterTransferI2CMaster * BT_Game;
    BetterTransferI2CMaster * BT_Module;
    //BetterTransferI2CMaster * BT_PrivateModule(uint8_t *, uint8_t, BT_Address *);
};

//BT/_Address NINE(9, &Wire);

GAME_STATE game_state;
Module * modules[2];


//BetterTransferI2CMaster BT_GAME_STATE(details(game_state), &NINE);
//
//MODULE_STATE module_state;
//BetterTransferI2CMaster BT_MODULE_STATE(details(module_state), &NINE);


void setup() {
  Serial.begin(115200);
  Wire.begin();
  for (int i=0; i < sizeof(modules) / sizeof(modules[0]); i++){
    modules[i] = new Module(9+i, game_state);
  }
  //  ET_GAME_STATE.begin(details(game_state), &Wire);
  //  ET_MODULE_STATE.begin(details(module_state), &Wire);

  inputString.reserve(10);
}
void loop() {

  if (stringComplete) {
    if (inputString == "phase prep\n") {
      game_state.phase = PREP;
    }
    else if (inputString == "phase game\n") {
      game_state.phase = GAME;
    }
    else if (inputString == "phase win\n") {
      game_state.phase = WIN;
    }
    else if (inputString == "phase lose\n") {
      game_state.phase = LOSE;
    }
    else if (inputString.substring(0, 11) == "strike set ") {
      game_state.strikes = inputString.substring(11, inputString.length() - 1).toInt();
    }
    else if (inputString == "serial odd\n") {
      game_state.serialOdd = true;
    }
    else if (inputString == "serial even\n") {
      game_state.serialOdd = false;
    }
    else if (inputString == "serial vowel true\n") {
      game_state.serialVowel = true;
    }
    else if (inputString == "serial vowel false\n") {
      game_state.serialVowel = false;
    }
    else if (inputString == "port true\n") {
      game_state.parallelPort = true;
    }
    else if (inputString == "port false\n") {
      game_state.parallelPort = false;
    }
    else if (inputString.substring(0, 10) == "batteries ") {
      game_state.strikes = inputString.substring(10, inputString.length() - 1).toInt();
    }else if(inputString == "\n"){
      
    }
    else {
      Serial.println("Not a recognised command");
      inputString = "";
      stringComplete = false;
      return;
    }
    printGameState(game_state);
    modules[0]->BT_Game->sendData();

    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  if (modules[0]->BT_Module->receiveData()) {
    Serial.println("Got Data");
  }
  game_state.strikes = modules[0]->state.module_strikes;
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

void printGameState(GAME_STATE gs) {
  Serial.println("------------");
  Serial.print("Phase: ");
  Serial.println(gs.phase);
  Serial.print("Strikes: ");
  Serial.println(gs.strikes);
  Serial.print("Batteries: ");
  Serial.println(gs.batteries);
  Serial.print("PP: ");
  Serial.println(gs.parallelPort);
  Serial.print("Serial: ");
  if (gs.serialOdd) {
    Serial.print("Odd, ");
  } else {
    Serial.print("Even, ");
  }
  if (gs.serialVowel) {
    Serial.println("Vowel ");
  } else {
    Serial.println("No Vowel ");
  }
}
