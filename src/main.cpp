#include <Arduino.h>
#include <MATRIX7219.h>
#include <ESP32RotaryEncoder.h>
#include <Joystick.hpp>
#include "utils.hpp"

//========================================
// Deklarace proměnných - inicializace všech objektů a proměnných pro hru
//========================================
Cursor cursor; // Objekt reprezentující pozici kurzoru (hráčův avatar)
MATRIX7219 display(DATA_PIN, SELECT_PIN, CLOCK_PIN, NUM_DEVICES);
RotaryEncoder encoder(ENCODE_DATA_PIN, ENCODE_CLOCK_PIN, ENCODE_SW_PIN);
Joystick joystick(JOYSTICK_X, JOYSTICK_Y, JOYSTICK_SW);
Projectile projectile(0,3);
MyTimer timer;
MyTimer mainTimer;

const unsigned long GAME_DURATION_MS = 90000UL; // Doba trvání jedné hry v milisekundách (90000 = 90 sekund)
long level = 1; // Aktuální úroveň obtížnosti (1-10)
bool gameRunning = false; // Příznak určující, zda hra právě běží
unsigned long points = 0; // Počet nabratých bodů

//========================================
// Deklarace funkcí - přední deklarace (forward declarations)
//========================================
void displaySetup(); // Inicializace LED matice
void encoderSetup(); // Inicializace otočného enkodéru

//========================================
// Nastavení a test hardwaru - spouští se pouze jednou při zapnutí
//========================================
void setup() {
  Serial.begin(9600); // Inicializace sériové komunikace pro ladění (9600 baudů)
  pinMode(JOYSTICK_SW, INPUT_PULLUP); // Nastavení tlačítka joysticku jako vstup s vnitřním pull-up odporem
  pinMode(BUZZ_PIN, OUTPUT); // Nastavení bzučáku jako výstup
  displaySetup(); // Inicializace LED matice
  encoderSetup(); // Inicializace rotačního enkodéru
  testBuzzer(); // Test Buzzeru
  testDisplay(&display); // Test displeje - sekvence rozsvícení řádků
  projectile.RandStart(); // Nastavení projektilu na náhodnou pozici
}


//========================================
// Hlavní cyklus - běží stále dokola
//========================================
void loop() {
  joystick.update(); // Přečtení aktuálních hodnot z joysticku
  
  // Nastavení obtížnosti a zapnutí hry - spustí se, když hra neběží
  if(!gameRunning){ // Pokud hra v tuto chvíli neběží
    level = encoder.getEncoderValue(); // Přečtení nastavené obtížnosti z enkodéru (1-10)
    if(joystick.getSW()) gameRunning = true; // Pokud je tlačítko joysticku stisknuto, hra se spustí
  }

  // Herní cyklus - běží, když je hra spuštěná
  if(gameRunning){ // Pokud hra právě běží
    displayGame(&display, &cursor, &projectile, &timer, joystick.getX(), joystick.getY(), &points, level, joystick.getSW()); // Aktualizace a vykreslení herního obsahu
    if(digitalRead(JOYSTICK_SW) == 0){ // Pokud je tlačítko joysticku stisknuto (tlačítko má aktivní nízkou úroveň)
      digitalWrite(BUZZ_PIN, HIGH); // Zapnutí bzučáku
    } else {
      digitalWrite(BUZZ_PIN, LOW); // Vypnutí bzučáku
    }

    // Kontrola vypršení doby trvání hry
    if(mainTimer.hasTimePassed(GAME_DURATION_MS)){ // Pokud uplynul čas hry (90 sekund)
      digitalWrite(BUZZ_PIN, LOW); // Vypnutí bzučáku
      resetDisplay(&display); // Krásné vypnutí displeje (animace)
      gameRunning = false; // Zastavení hry
      Serial.println("\033[2J"); // Vymazání sériové konzole
      Serial.println("SKONCIL JSI!"); // Zpráva na konzolu
      Serial.printf("BODY: %lu\n", points); // Výpis dosažených bodů
      points = 0; // Reset bodů pro další hru
    }
  }

  delay(25);
}

//========================================
// Definice funkcí - detailní implementace
//========================================

/**
 * Inicializace LED matice (display)
 */
void displaySetup() {
  display.begin(); // Inicializace komunikace s maticí
  display.clear(); // Smazání obsahu displeje
  display.displayOn(); // Zapnutí displeje
  //display.setSwap(true); // Možnost obrácení obrazu (není potřeba)
  display.setBrightness(1); // Nastavení minimální jasu (1-15)
}

/**
 * Inicializace rotačního enkodéru pro nastavení obtížnosti
 */
void encoderSetup() {
  encoder.setEncoderType(EncoderType::HAS_PULLUP); // Nastavení typu enkodéru (má vnitřní pull-up odpory)
  encoder.setBoundaries(1, 10, true); // Nastavení rozsahu hodnot: minimálně 1, maximálně 10, cyklický
  encoder.onTurned( [](long value) { // Callback funkce při otočení enkodéru
    static unsigned long lastTurn = 0; // Čas poslední rotace
    const unsigned long DEBOUNCE_MS = 5; // Minimální čas mezi registracemi rotací (protidrgání)
    unsigned long now = millis(); // Aktuální čas v milisekundách
    if (now - lastTurn < DEBOUNCE_MS) return; // Pokud byl čas mezi rotacemi příliš krátký, ignoruj
    lastTurn = now; // Aktualizuj čas poslední rotace
    Serial.println("\033[2J"); // Vymaž sériovou konzolu
    Serial.printf("Current difficulty: %ld\n", value); // Vytiskni aktuální obtížnost na konzolu
  });
  encoder.begin(); // Spuštění enkodéru
};