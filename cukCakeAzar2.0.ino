#include <Servo.h>

Servo servoo, servob;

int posOjos = 60;  // posición inicial ojos
int posBoca = 120; // posición inicial boca

void setup() {
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  servoo.attach(9);
  servob.attach(10);
  Serial.begin(9600);
}

void moverServoSuave(Servo &servo, int &posActual, int posNueva, int paso, int delayMs) {
  if (posActual < posNueva) {
    for (int p = posActual; p <= posNueva; p += paso) {
      servo.write(p);
      delay(delayMs);
    }
  } else {
    for (int p = posActual; p >= posNueva; p -= paso) {
      servo.write(p);
      delay(delayMs);
    }
  }
  posActual = posNueva;
}

void loop() {
  int r = random(30, 80);
  int r2 = random(90, 170);
  int r3 = random(500, 3500);

  digitalWrite(4, random(0, 2) ? HIGH : LOW);
  digitalWrite(5, random(0, 2) ? HIGH : LOW);

  int b = random(0, 2);

  if (b == 0) {
    Serial.print("Boca -> ");
    Serial.println(r2);
    moverServoSuave(servob, posBoca, r2, 2, 10);
  } else {
    Serial.print("Ojos -> ");
    Serial.println(r);
    moverServoSuave(servoo, posOjos, r, 2, 10);
  }

  delay(r3);
}