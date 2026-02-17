#include <avr/pgmspace.h>
#include <RCSwitch.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// FIJADO A 1 PARA ASEGURAR QUE LOS COMBOS FUNCIONEN
#define Auto_Standby_Support 1  

#define KEY_DELAY 300      // 300 ms break between keys
#define BEEP_DURATION 150  // 150 ms beep time

RCSwitch mySwitch = RCSwitch();

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

const unsigned long Key_Minus_1 PROGMEM = 7061400; 
const unsigned long Key_Plus_1 PROGMEM = 7061396;
const unsigned long Key_Minus_10 PROGMEM = 7061404;
const unsigned long Key_Plus_10 PROGMEM = 7061394;
const unsigned long Key_Auto PROGMEM = 7061402;
const unsigned long Key_Standby PROGMEM = 7061398;

// Seatalk datagrams
const PROGMEM uint16_t ST_NMEA_BridgeID[] =  { 0x190, 0x00, 0xA3 };

const PROGMEM uint16_t ST_Minus_1[] =  { 0x186, 0x21, 0x05, 0xFA };
const PROGMEM uint16_t ST_Minus_10[] = { 0x186, 0x21, 0x06, 0xF9 };
const PROGMEM uint16_t ST_Plus_1[] =   { 0x186, 0x21, 0x07, 0xF8 };
const PROGMEM uint16_t ST_Plus_10[] =  { 0x186, 0x21, 0x08, 0xF7 };
const PROGMEM uint16_t ST_Auto[] =     { 0x186, 0x21, 0x01, 0xFE };
const PROGMEM uint16_t ST_Standby[] =  { 0x186, 0x21, 0x02, 0xFD };

// NUEVOS COMANDOS SEATALK PARA COMBOS (Simulan pulsación simultánea física)
const PROGMEM uint16_t ST_Tack_Port[] = { 0x186, 0x21, 0x21, 0xDE }; // -1 y -10
const PROGMEM uint16_t ST_Tack_Stbd[] = { 0x186, 0x21, 0x22, 0xDD }; // +1 y +10
const PROGMEM uint16_t ST_Wind_Mode[] = { 0x186, 0x21, 0x23, 0xDC }; // Auto y Standby

const PROGMEM uint16_t ST_BeepOn[] =  { 0x1A8, 0x53, 0x80, 0x00, 0x00, 0xD3 };
const PROGMEM uint16_t ST_BeepOff[] = { 0x1A8, 0x43, 0x80, 0x00, 0x00, 0xC3 };

boolean blink = true;
unsigned long wind_timer = 0;   // timer for AWS display
unsigned long beep_timer2 = 0;  // timer to stop alarm sound
unsigned long bridge_timer = 0; // timer to send ST Bridge ID every 10 seconds

unsigned long key_time = 0;     // time of last key detected
unsigned long beep_time = 0;    // timer for beep duration
bool beep_status = false;

// VARIABLES PARA LOGICA DE COMBOS
unsigned long pending_key = 0;
unsigned long pending_time = 0;
bool is_wind_mode = false;
const unsigned long COMBO_WINDOW = 750; // 0.75 segundos de espera para el combo


boolean sendDatagram(const uint16_t data[]) {
  int i = 0; int j = 0;
  boolean ok = true;
  int bytes;
  unsigned int inbyte;
  unsigned int outbyte;

  bytes = (pgm_read_byte_near(data + 1) & 0x0f) + 3; // Messege length is minimum 3, additional bytes in nibble 4

  while (j < 5 ) { // CDMA/CD 5 tries
    unsigned long wait_start = millis();
    
    while (Serial1.available ()) {  // Wait for silence on the bus
      inbyte = (Serial1.read());
      delay(1); // CORRECCIÓN 1: Bajado a 1ms para vaciar el buffer de entrada más rápido
      
      // CORRECCIÓN 2: Timeout de seguridad de 500ms
      if (millis() - wait_start > 500) {
        break; 
      }
    }

    ok = true;
    for (i = 0; (i < bytes) & (ok == true); i++) { // Write and listen to detect collisions
      outbyte = pgm_read_word_near(data + i);
      Serial1.write(outbyte);
      delay(3); // Este delay de 3ms sí se mantiene porque da tiempo a leer el eco del bus Seatalk

      if (Serial1.available ()) {
        inbyte = Serial1.read();  // Not what we sent, collision!
        if (inbyte != outbyte) ok = false;
      }
      else ok = false; // Nothing received
    }

    if ( ok ) return ok;

    j++; // Collision detected
    delay(random(2, 50));  // Random wait for next try
  }
  Display("Send Error", 2);
  return false;
}


void Display(const char *string, int size) {
  display.clearDisplay();
  display.setTextSize(size);
  display.setCursor(0, 0);
  display.println(string);
  display.display();
  wind_timer = millis();
}


// Receive apparent wind speed from bus
int checkWind(char * AWS) {
  unsigned int xx;
  unsigned int y;
  unsigned int inbyte;
  int wind = -1;

  if (Serial1.available ()) {
    inbyte = Serial1.read();
    if (inbyte == 0x111) {    // AWS Seatalk command - See reference from Thomas Knauf
      delay(3);
      inbyte = Serial1.read();
      if (inbyte == 0x01) {   // AWS Setalk command
        delay(3);
        xx = Serial1.read();
        delay(3);
        y = Serial1.read();
        wind = (xx & 0x7f) + (y / 10);  // Wind speed
        if (wind < 100) itoa (wind , AWS, 10);  // Greater 100 must be a receive error
      }
    }
  }
  return wind;
}


void setup() {
  Serial.begin( 9600 );  // Serial out put for function checks with PC
  Serial1.begin( 4800, SERIAL_9N1 );  // Set the Seatalk modus - 9 bit
  Serial1.setTimeout(5);

  // reserve 20 bytes for the inputString:
  inputString.reserve(20);
  
  mySwitch.enableReceive(4);  // RF Receiver on inerrupt 4 => that is pin 7 on Micro

  pinMode(9, OUTPUT);         // LED to show if keys are received
  digitalWrite(9, HIGH);

  pinMode(20, OUTPUT);         // Buzzer to show if keys are received
  digitalWrite(20, LOW);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C
  display.setTextColor(WHITE);
  Display("Start", 4);

  sendDatagram(ST_NMEA_BridgeID);   // Send NMEA Seatakl BridgeID
}


void BeepOn(void) {
  if (beep_status == true) return;  // Already On
  sendDatagram(ST_BeepOn);
  digitalWrite(20, HIGH);
  beep_time = millis();
  beep_status = true;
}


void BeepOff(void) {
  if (beep_status == true && millis() > beep_time + BEEP_DURATION) {
    sendDatagram(ST_BeepOff);
    digitalWrite(20, LOW);
    beep_status = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if ((inChar != '\n') && (inChar != '\r')) inputString += inChar;
    if ((inChar == '\n') || (inChar == '\r')) {
      stringComplete = true;
    }
  }
}

// FUNCION DE APOYO PARA ENVIAR COMANDOS Y MOSTRAR EN PANTALLA
void executeCommand(const char* msg, int textSize, const uint16_t* datagram) {
  Display(msg, textSize);
  sendDatagram(datagram);
  BeepOn();
}

// FUNCION PARA EJECUTAR BOTONES NORMALES INDIVIDUALES
void executeNormalKey(unsigned long val) {
  if (val == Key_Minus_1)       executeCommand("-1", 7, ST_Minus_1);
  else if (val == Key_Plus_1)   executeCommand("+1", 7, ST_Plus_1);
  else if (val == Key_Minus_10) executeCommand("-10", 7, ST_Minus_10);
  else if (val == Key_Plus_10)  executeCommand("+10", 7, ST_Plus_10);
  else if (val == Key_Standby) {
    executeCommand("Standby", 4, ST_Standby);
    is_wind_mode = false; // Al poner standby, salimos del modo viento
  }
}

void loop() {
  
  char AWS[4] = "";
  unsigned long value = 0;

  if (millis() > wind_timer + 2000 ) {
    Display("---", 7);              
    wind_timer = millis();
  }

  if (millis() > beep_timer2 + 3000 ) {
    sendDatagram(ST_BeepOff);       
    beep_timer2 = millis();
  }

  if (millis() > bridge_timer + 10000 ) {
    sendDatagram(ST_NMEA_BridgeID); 
    bridge_timer = millis();
  }

  if (checkWind(AWS) > -1) {
    Display(AWS, 7);
    wind_timer = millis();
  }

  if (mySwitch.available()) {
    value = mySwitch.getReceivedValue();
    mySwitch.resetAvailable();
  }

  serialEvent();  
  
  if (stringComplete) {    
    if(inputString == "-1") value = Key_Minus_1;
    if(inputString == "+1") value = Key_Plus_1;
    if(inputString == "-10") value = Key_Minus_10;
    if(inputString == "+10") value = Key_Plus_10;
    if(inputString == "A") value = Key_Auto;
    if(inputString == "S") value = Key_Standby;
    
    inputString = "";
    stringComplete = false;
  }

  // LÓGICA DE PULSACIONES Y COMBOS
  if (value > 0 && millis() > key_time + KEY_DELAY) {
    key_time = millis();      // Remember time of last key received
    digitalWrite(9, blink);   // LED on/off
    blink = !blink;           // Toggle LED to show received key

    if (pending_key == Key_Auto) {
      // ESTAMOS EN LA VENTANA DE 0.75 SEGUNDOS ESPERANDO UN COMBO
      if (value == Key_Standby) {
        executeCommand("Viento", 4, ST_Wind_Mode);
        is_wind_mode = true;
        pending_key = 0;
      } 
      else if (value == Key_Minus_1) {
        executeCommand("Babor", 4, ST_Tack_Port);
        pending_key = 0;
      } 
      else if (value == Key_Plus_1) {
        executeCommand("Estrib", 4, ST_Tack_Stbd);
        pending_key = 0;
      }
      else if (value == Key_Auto) {
        // Ignoramos si se pulsa Auto dos veces seguidas muy rápido
      } 
      else {
        // Se pulsó otra tecla (ej. +10). Ejecutamos el Auto pendiente y luego la nueva tecla
        executeCommand("Auto", 4, ST_Auto);
        executeNormalKey(value);
        pending_key = 0;
      }
    } 
    else {
      // NO HAY COMBOS PENDIENTES
      if (value == Key_Auto) {
        if (is_wind_mode) {
          // Si estamos en modo viento, Auto nos devuelve al piloto normal inmediatamente
          executeCommand("Auto", 4, ST_Auto);
          is_wind_mode = false;
        } else {
          // Si estamos en compás, iniciamos el temporizador del combo
          pending_key = Key_Auto;
          pending_time = millis();
        }
      } 
      else {
        // Pulsación de tecla normal
        executeNormalKey(value);
      }
    }
    value = 0; // Limpiamos el valor procesado
  }

  // VERIFICAR SI EXPIRÓ EL TIEMPO DE 0.75s PARA EL BOTÓN AUTO
  if (pending_key == Key_Auto && (millis() - pending_time > COMBO_WINDOW)) {
    executeCommand("Auto", 4, ST_Auto);
    pending_key = 0;
  }

  BeepOff();
}
