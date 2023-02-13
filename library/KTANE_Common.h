enum Game_Phase {
  PREP,
  GAME,
  WIN,
  LOSE
};


struct GAME_STATE {
  int strikes;
  Game_Phase phase;
  int batteries;
  bool parallelPort;
  bool serialOdd;
  bool serialVowel;
};


struct MODULE_STATE {
  int module_strikes;
  bool finished_stage;
  char module_name[4];
  uint8_t private_size;
};
