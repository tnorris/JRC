/* Tom Norris, 2017 */
/* Built on top of a MicroView, https://www.sparkfun.com/products/12923 */
/* wriring is super simple, two buttons and an Arduino: 
   Arduino Pin 0/1 -> Momentary Button -> Ground */

#include <MicroView.h>
#include <EEPROM.h>

#define ROW_PLUS 1
#define ROW_MINUS 0
#define DEBOUNCE_DELAY 100

#define SAVE_DELAY 5000
unsigned long last_save;

typedef struct {
  bool plus;
  bool minus;
  unsigned long last_press;
} KeyboardState;

KeyboardState k = { false, false, 0 };

unsigned int row = 0;

void setup() {
  readSettings();
  setupPins();
  setupMicroView();
  k.last_press = millis();
  last_save = millis();
}

void setupPins() {
  pinMode(ROW_PLUS, INPUT_PULLUP);
  pinMode(ROW_MINUS, INPUT_PULLUP);
}

void readSettings() {
  EEPROM.get(0, row);
}

void setupMicroView() {
  uView.begin();
  uView.setCursor(0,0);
  uView.setFontType(3);
  uView.clear(PAGE);
  uView.print(row, DEC);
  uView.display();
}

void readKeypad() {
  k.plus = digitalRead(ROW_PLUS);
  k.minus = digitalRead(ROW_MINUS);
}        

void updateRow() {
  if(k.last_press + DEBOUNCE_DELAY > millis() ) { return ; }
  if(k.plus) { row++ ; }
  if(k.minus) { row-- ; }
  if(row < 0) { row = 0; }
  k.last_press = millis();
}

void updateDisplay() {
  uView.setCursor(0,0);
  uView.clear(PAGE);
  uView.print(row, DEC);
  uView.display();
}

void updateSettings() {
  if(last_save + SAVE_DELAY > millis() ) { return ; } /* don't burn out the flash */
  EEPROM.put(0, row);
  last_save = millis();
}

void loop() {
  readKeypad();
  updateDisplay();
  updateRow();
  updateSettings();
}
