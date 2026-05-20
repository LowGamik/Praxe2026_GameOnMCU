# Dot Hunt
## Štěpán Valenta & Michal Mlčák - skill issue team
Material použit:
- ESP32-WROOM-32
- Piezobzučák
- Joystick
- LED Matrix
- Rotační Encoder

## PROBLEMY
- Zapojení LED Matrix do boot pinů
> První hardwarevé problémy nastaly když jsme zapojovali LED Matrix, protože jsme ho měli napojený na boot piny. Vyřešeno bylo pomocí přepojení do jiných GPIO pinů

- Zapomenutí zapojení LED Matrix do GROUND
> Dále jsme měli problémy se zapojením, protože jsme později zjistili že nemáme LED Matrix zapojený do GROUND (aka. 0V). Vyřešeno zapojením do ground pinu.

- Přesnost os na Joysticku
> Další problém jsme měli s přesností s osami "x" a "y", neboli když "x" bylo 1, tak "y" bylo taky 1, když nebyla pozice diagonální. Vyřešeno bylo způsobem změnšení vstupu napětí z 5V na 3.3V

- Přepisování kurzoru
> Problém byl v top že jsme kreslili kurzor strukturovaně (takzvaně. 1. řádek, 2. řádek, 3. řádek, 2. řádek), a se to pokažde přepsalo. Vyřešení bylo vyměnit celý strukturováný výkres za 2D pole, na kterém jsme mohli rozšiřovat i na projektily.