// --- PINES (Cableado real del puente H L298N) ---
const int IN1 = 5;// Control del Motor DERECHO
const int IN2 = 2;// Control del Motor DERECHO
const int IN3 = 3;// Control del Motor IZQUIERDO
const int IN4 = 4; // Control del Motor IZQUIERDO

// PWM
const int ENA = 9; // velocidad motor DERECHO
const int ENB = 10; // velocidad motor IZQUIERDO

// Sensores IR
const int sensorIzq = 13;
const int sensorDer = 12;

// Velocidades Ajustadas (Se mantienen para evitar la vibración)
const int VELOCIDAD_AVANCE = 200; // Mínimo confiable para el arranque
const int VELOCIDAD_REVERSA = 220; // Mayor potencia para salir del borde
const int VELOCIDAD_GIRO= 180; 

// Tiempos
const unsigned long BACK_MS = 1200;
const unsigned long PAUSE_MS = 200;
const unsigned long TURN_MS = 750;
const unsigned long DEBOUNCE_MS = 40;

// -------------------------------------------------------------

void setup() {
pinMode(IN1, OUTPUT);
pinMode(IN2, OUTPUT);
pinMode(IN3, OUTPUT);
pinMode(IN4, OUTPUT);

pinMode(ENA, OUTPUT);
pinMode(ENB, OUTPUT);

pinMode(sensorIzq, INPUT);
pinMode(sensorDer, INPUT);

Serial.begin(9600);
Serial.println("Robot sigue-borde — Dirección de Movimiento Corregida");

detener();
randomSeed(analogRead(A0));
}

// -------------------------------------------------------------
// Lectura estable: LOW = blanco detectado
bool detectaBlanco(int pin) {
if (digitalRead(pin) != LOW) return false;

unsigned long t = millis();
while (millis() - t < DEBOUNCE_MS) {
if (digitalRead(pin) != LOW) return false;
}
return true;
}

// -------------------------------------------------------------
// MOVIMIENTOS (LÓGICA INVERTIDA RESPECTO AL CÓDIGO ANTERIOR)

void adelante() {
// *** ESTA ERA LA LÓGICA DE RETROCEDER ***
// Motor IZQUIERDO (Avanza)
digitalWrite(IN3, HIGH);
digitalWrite(IN4, LOW);
analogWrite(ENB, VELOCIDAD_AVANCE);

// Motor DERECHO (Avanza)
digitalWrite(IN1, LOW); 
digitalWrite(IN2, HIGH);
analogWrite(ENA, VELOCIDAD_AVANCE);
}

void retrocede() {
// *** ESTA ERA LA LÓGICA DE AVANZAR ***
// Motor IZQUIERDO (Retrocede)
digitalWrite(IN3, LOW);
digitalWrite(IN4, HIGH);
analogWrite(ENB, VELOCIDAD_REVERSA);

// Motor DERECHO (Retrocede)
digitalWrite(IN1, HIGH); 
digitalWrite(IN2, LOW);
analogWrite(ENA, VELOCIDAD_REVERSA);
}

void girarDerechaEnSitio() {
// IZQUIERDO debe avanzar (Lógica de adelante)
digitalWrite(IN3, HIGH);
digitalWrite(IN4, LOW);
analogWrite(ENB, VELOCIDAD_GIRO);

  // DERECHO debe retroceder (Lógica de retrocede)
  digitalWrite(IN1, HIGH); 
  digitalWrite(IN2, LOW);
  analogWrite(ENA, VELOCIDAD_GIRO);
}

void girarIzquierdaEnSitio() {
  // IZQUIERDO debe retroceder (Lógica de retrocede)
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, VELOCIDAD_GIRO);

  // DERECHO debe avanzar (Lógica de adelante)
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, VELOCIDAD_GIRO);
}

void detener() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// -------------------------------------------------------------

void setupRecuperacion(bool izq, bool der) {
  detener();
  delay(80);

  Serial.println("RECUP: retrocediendo");
  // Ahora retrocede() físicamente hace que el robot vaya hacia atrás.
  retrocede();
  delay(BACK_MS);

  detener();
  delay(PAUSE_MS);

  // LÓGICA DE RECUPERACIÓN (sensor detecta, gira al lado opuesto)
  // Si sensor IZQ detecta -> gira DERECHA para alejarse. (Correcto)
  if (izq && !der) {
    Serial.println("RECUP: sensor IZQ detecta → giro DERECHA");
    girarDerechaEnSitio();
    delay(TURN_MS);
  }
  // Si sensor DER detecta -> gira IZQUIERDA para alejarse. (Correcto)
  else if (!izq && der) {
    Serial.println("RECUP: sensor DER detecta → giro IZQUIERDA");
    girarIzquierdaEnSitio();
    delay(TURN_MS);
  }
  else {
    Serial.println("RECUP: ambos → giro aleatorio");
    if (random(0,2) == 0) {
      girarDerechaEnSitio();
    } else {
      girarIzquierdaEnSitio();
    }
    delay(TURN_MS);
  }

  detener();
  delay(PAUSE_MS);
}

// -------------------------------------------------------------

void loop() {
  int rawIzq = digitalRead(sensorIzq);
  int rawDer = digitalRead(sensorDer);

  bool izqBlanco = (rawIzq == LOW) && detectaBlanco(sensorIzq);
  bool derBlanco = (rawDer == LOW) && detectaBlanco(sensorDer);

  // Si detecta borde, entra en modo recuperación (retrocede y gira)
  if (izqBlanco || derBlanco) {
    setupRecuperacion(izqBlanco, derBlanco);
  } else {
    // Si no hay borde, avanza.
    adelante();
  }

  delay(40);
}
