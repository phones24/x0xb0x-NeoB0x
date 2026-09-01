
#define PICK_NEW_SETUP 0
#define REUSE_SETUP    1

void randomize(uint8_t setupMode, uint8_t p_len);
void randomizeNotes(uint8_t p_len);
void simplifyByLoop(uint8_t p_len);

extern uint16_t scaledNotes[7];
extern uint8_t  rndScale; // selected random scale (0..6), defined in pattern_edit.c
