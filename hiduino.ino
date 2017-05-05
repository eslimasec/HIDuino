/* Arduino USB HID Keyboard Demo
 * Random Key/Random Delay Windows Server 2003, Windows Vista, Windows Server 2008, Windows Server 2003 with SP2, Windows Server 2003 R2, Windows Server 2008 R2, Windows Server 2000, Windows Server 2012, Windows Server 2003 with SP1, Windows 8
 */

#define PIN_BUTTON 7
#define UP 0
#define DOWN 1
/* Keyboard report buffer */
uint8_t buf[8] = { 0 };   
int keypressDelay = 0;
int state;
int prevState = DOWN;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_BUTTON, INPUT);
  digitalWrite(PIN_BUTTON, 1); 
  delay(200);
}

void loop() 
{
  state = digitalRead(PIN_BUTTON);
//we need to capture the big red button down action properly
//we only act when the button is lifted up and then pushed down
  if (state == DOWN && prevState == UP) {
    //press win key plus r => run shortcut
    buf[0] = 8;    
    buf[2] = 21;
    Serial.write(buf, 8); 
    releaseKey();
    delay(200);
      
    //write: shutdown /s /t 1 /f<enter key>  
    //hid decimal codes: 22 11 24 ..
    int message[21] = {22,11,24,23,7,18,26,17,44,56,22,44,56,23,44,30,44,56,9,40,40};
    for (int i = 0; i < 20; i++){ 
      writeChar(message[i]);
    }
  }
  prevState = state;    
}

void writeChar(uint8_t character) {
  buf[0] = 0X00;    
  buf[2] = character; 
  Serial.write(buf, 8); 
  releaseKey();
  delay(keypressDelay);
}

void releaseKey() {
  buf[0] = 0;
  buf[2] = 0;
  Serial.write(buf, 8); // Release key  
}
