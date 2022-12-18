#include <Wire.h>
#include <BetterTransferI2CSlave.h>
#define I2C_SLAVE_ADDRESS 9


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


void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SLAVE_ADDRESS);
  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc.
  ET_GAME_STATE.begin(details(game_state), &Wire);
  //define handler function on receiving data
  Wire.onReceive(receive);
}

void loop() {
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


// HIDE EVERYTHING ABOVE THIS LINE IN A LIBRARY LATER

struct PRIVATE_MODULE_STATE {
  // This will hold the state of the module that is specific to the one module, Colour of button, Number of wires, etc.
  // This will be different on every module including size.
  // make sure you define arrays like this "int stuff[5];" not like this "int stuff[];" it breaks if they are dynamically assigned
};

void receive(int numBytes) {}
