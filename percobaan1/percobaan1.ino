#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

// Definisi Matriks Keypad 4x4
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Urutan pin sesuai skema gambar modul praktikum
byte rowPins[ROWS] = {5, 4, 3, 2}; // Row 0, 1, 2, 3
byte colPins[COLS] = {8, 7, 6, 11}; // Col 0, 1, 2, 3

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

byte ledPin = 13;
boolean blink = false;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // LED awal kondisi mati
  
  // Mendaftarkan event listener keypad
  keypad.addEventListener(keypadEvent);
}

void loop() {
  // Memanggil scanning keypad
  keypad.getKey();

  // Logika kedip LED saat kondisi blink = true (saat tombol 1 atau * di-HOLD)
  if (blink) {
    digitalWrite(ledPin, !digitalRead(ledPin)); // Toggle ON/OFF
    delay(100);
  }
}

// Fungsi Event Keypad
void keypadEvent(KeypadEvent key) {
  switch (keypad.getState()) {
    
    // 1. Saat tombol ditekan biasa (LED Nyala Stay)
    case PRESSED:
      switch (key) {
        case '1':
        case '*':
          blink = false;             // Pastikan mode kedip   mati dulu
          digitalWrite(ledPin, HIGH); // LED menyala terus
          Serial.print("Tombol Ditekan: ");
          Serial.println(key);
          Serial.println("--> LED ON");
          break;
      }
      break;

    // 2. Saat tombol ditahan/HOLD (LED Berkedip-kedip)
    case HOLD:
      switch (key) {
        case '1':
        case '*':
          blink = true; // Aktifkan mode berkedip
          Serial.print("Tombol Ditahan: ");
          Serial.println(key);
          Serial.println("--> Mode BLINK Aktif");
          break;
      }
      break;

    // 3. Saat tombol dilepas (LED Mati & Stop Kedip)
    case RELEASED:
      switch (key) {
        case '1':
        case '*':
          blink = false;            // Hentikan mode kedip
          digitalWrite(ledPin, LOW); // Turn off LED
          Serial.print("Tombol Dilepas: ");
          Serial.println(key);
          Serial.println("--> LED OFF");
          break;
      }
      break;
  }
}