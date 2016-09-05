#include <ProTrinketKeyboard.h> 
#include <math.h>

// WHICH BUTTON/SWITCH IS ON WHICH PIN:

    // BUTTON_1 0
    // BUTTON_2 1
    // BUTTON_3 3
    // BUTTON_4 4
    // BUTTON_5 5
    // SWITCH_1A 6
    // SWITCH_1B 8
    // SWITCH_2A 9
    // SWITCH_2B 10
    // SWITCH_3A 11
    // SWITCH_3B 12
    // SWITCH_4A 13
    // SWITCH_4B 14
    // SWITCH_5A 15
    // SWITCH_5B 16
    // BIG_SWITCH 17

#define POT_1 18
#define POT_2 19

const int pins[] = {0, 1, 3, 4, 5, 6, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
const int keycodes[] = {97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112};
int prev_state[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
int current_state[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

int pot1_value = 0;
int pot2_value = 0;
int prev_pot1 = 0;
int prev_pot2 = 0;

void setup() {

  for(int i=0; i<16; i++){    // initialize each pin as input
    pinMode(pins[i], INPUT);
  }

  for(int i=0; i<16; i++){    // buttons & switches are active low, so pull them up
    digitalWrite(pins[i], HIGH);
  }

  TrinketKeyboard.begin();

}

void loop() {
  
  TrinketKeyboard.poll();

  prev_pot1 = pot1_value;   
  prev_pot2 = pot2_value;

  pot1_value = round(analogRead(POT_1) / 100);  // increases the threshold for a keypress
  pot2_value = round(analogRead(POT_2) / 100);  // so that keypresses aren't fired constantly

  if (pot1_value > prev_pot1){          // check to see if pots have been turned
    TrinketKeyboard.typeChar(48);       // if yes, make keypress
  }
  if (pot1_value < prev_pot1){
    TrinketKeyboard.typeChar(49);
  }
  if (pot2_value > prev_pot2){
    TrinketKeyboard.typeChar(50);
  }
  if (pot2_value < prev_pot2){
    TrinketKeyboard.typeChar(51);
  }

  for(int i=0; i<16; i++){

    if (i != 11){
      
      int button = digitalRead(pins[i]);
        
      if (button == LOW && prev_state[i] == 1 && current_state[i] == 1){
           
        executeKeystroke(i, keycodes[i]);             // if the button/switch is pressed execute keypress
        current_state[i] = 0;                         // and set current_state variable to 0. The prev_state
        break;                                        // variable is set to 0 in the executeKeystroke function
        
      }
  
      if(button == LOW && prev_state[i] == 0){        // if the button/switch is pressed but the prev_state variable is
                                                      // is 0, the button/switch has already fired its event. Set prev_state
        prev_state[i] = 1;                            // variable back to 1 and break.
        break;
        
      }

      if(button == HIGH && current_state[i] == 0){    // if the button/switch is NOT pressed but the current_state variable
                                                      // is 0, the button/switch has just been released. Set the current_state var
        current_state[i] = 1;                         // back to 1 to allow for another keypress.
      }
    }
  }
  delay(10);
}

void executeKeystroke(int i, uint8_t keycode){

  TrinketKeyboard.typeChar(keycode);
  prev_state[i] = 0;
  
}

