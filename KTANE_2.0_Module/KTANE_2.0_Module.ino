#include <Wire.h>
#include <BetterTransferI2CSlave.h>
#include <KTANE_Common.h>
#define I2C_SLAVE_ADDRESS 11


String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete



Game_Phase lastPhase = PREP;






GAME_STATE game_state;
BetterTransferI2CSlave ET_GAME_STATE(details(game_state));

MODULE_STATE module_state;
BetterTransferI2CSlave ET_MODULE_STATE(details(module_state));

void setup() {
  Serial.begin(115200);

  Wire.begin(I2C_SLAVE_ADDRESS);
  Wire.onReceive(BetterTransferI2CSlave::onReceive);
  Wire.onRequest(BetterTransferI2CSlave::onSend);;
  inputString.reserve(10);
  Serial.print("Setup as module: ");
  Serial.println(I2C_SLAVE_ADDRESS);

}

void loop() {
  if (lastPhase != game_state.phase) {
    switch (game_state.phase) {
      case PREP:
        reset();
        prep_setup();
        break;
      case GAME:
        armed_setup();
        break;
      case WIN:
        win_setup();
        break;
      case LOSE:
        lose_setup();
        break;
    }
    lastPhase = game_state.phase;
  } else {

    switch (game_state.phase) {
      case PREP:
        module_state.finished_stage = prep_loop();
        break;
      case GAME:
        if (!module_state.finished_stage) {
          if (armed_loop()) {
            module_state.finished_stage = true;
            disarm_setup();
          }
        } else {
          disarm_loop();
        }
        break;
      case WIN:
        win_loop();
        break;
      case LOSE:
        lose_loop();
        break;
    }
  }


  //  if (ET_GAME_STATE.newData()) {
  //    printGameState(game_state);
  //  }

}

void printGameState(GAME_STATE gs) {
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



void prep_setup() {
  Serial.println("BEGINING SETUP");
}

bool prep_loop() {
  // This is the loop that will run when the bomb is in its prep phase
  // Return true if the bomb is ready to be armed
  return false;
}



void armed_setup() {
  Serial.println("MODULE ARMED");
  Serial.println("Commands: \"disarm\", \"strike\"");

}

bool armed_loop() {
  // This is the loop that will run when the bomb is in its game phase
  // Return true if the bomb is ready to be disarmed
  if (stringComplete) {
    if (inputString == "disarm\n") {
      inputString = "";
      stringComplete = false;
      return true;
    }
    if (inputString == "strike\n") {
      module_state.module_strikes++;
      Serial.print("Strike. Current module strikes: ");
      Serial.print(module_state.module_strikes);
      Serial.print(", game strikes: ");
      Serial.println(game_state.strikes);
      
    }

    inputString = "";
    stringComplete = false;
  }
  return false;
}

void disarm_setup() {
  Serial.println("MODULE DISARMED");
  Serial.println("No Commands. Waiting for win, lose or reset");

}

void disarm_loop() {
  // I doubt there is likley to be anything to go here but there may be an animation to play when module is disarmed
  // Needy modules might also use this to rearm themselves
}

void win_setup() {
  Serial.println("BOMB COMPLETED");

}

bool win_loop() {
  //return if win animation has finished
  return true;
}
void lose_setup() {
  Serial.println("KABOOOOM!!! YOU BEEN EXPLODIFICATED!!!");

}

bool lose_loop() {
  //return if lose animation has finished
  return true;
}

void reset() {
  Serial.println("NOT SURE WHAT THE POINT OF RESET IS");

}
