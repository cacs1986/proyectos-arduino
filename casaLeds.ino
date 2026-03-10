char dato;             // Dato recibido por Bluetooth
bool modoManual = false; // Estado del modo manual

void setup() {
  pinMode(2, OUTPUT);  // Verde
  pinMode(3, OUTPUT);  // Azul
  pinMode(4, OUTPUT);  // Amarillo
  pinMode(5, OUTPUT);  // Rojo

  pinMode(6, INPUT_PULLUP);  // Pulsador rojo
  pinMode(7, INPUT_PULLUP);  // Pulsador verde
  pinMode(8, INPUT_PULLUP);  // Pulsador azul
  pinMode(9, INPUT_PULLUP);  // Pulsador amarillo

  Serial.begin(9600);
  
}

void loop() {
  // --- Lectura Bluetooth ---
  if (Serial.available() > 0) {
    dato = Serial.read();

    switch (dato) {
      // --- Activar o desactivar modo manual ---
      case 'M':
        modoManual = true;
        Serial.println("Modo MANUAL activado.");
        break;

      case 'N':
        modoManual = false;
        Serial.println("Modo BLUETOOTH activado.");
        break;

      // --- Control por Bluetooth (solo si no está en modo manual) ---
      case 'R':
        if (!modoManual) digitalWrite(5, HIGH);
        Serial.println("Habitacion 1 encendida");
        break;

      case 'r':
        if (!modoManual) digitalWrite(5, LOW);
        Serial.println("Habitacion 1 apagada");
        break;

      case 'V':
        if (!modoManual) digitalWrite(2, HIGH);
        Serial.println("Habitacion 2 encendida");
        break;

      case 'v':
        if (!modoManual) digitalWrite(2, LOW);
        Serial.println("Habitacion 2 apagada");
        break;

      case 'A':
        if (!modoManual) digitalWrite(3, HIGH);
        Serial.println("Habitacion 3 encendida");
        break;

      case 'a':
        if (!modoManual) digitalWrite(3, LOW);
        Serial.println("Habitacion 3 apagada");
        break;

      case 'Y':
        if (!modoManual) digitalWrite(4, HIGH);
        Serial.println("Habitacion 4 encendida");
        break;

      case 'y':
        if (!modoManual) digitalWrite(4, LOW);
        Serial.println("Habitacion 4 apagada");
        break;
    }
  }

  // --- Control por pulsadores si el modo manual está activado ---
  if (modoManual) {
    digitalWrite(5, digitalRead(6) == LOW ? HIGH : LOW);
    digitalWrite(2, digitalRead(7) == LOW ? HIGH : LOW);
    digitalWrite(3, digitalRead(8) == LOW ? HIGH : LOW);
    digitalWrite(4, digitalRead(9) == LOW ? HIGH : LOW);
  }
}
