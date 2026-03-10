#include <Keypad.h>

// ---------- Configuración de pines ----------
const int ledRojo = 10;
const int ledVerde = 11;
const int ledAmarilloBuzzer = 12; // comparte pin
const int pirPin = 13;

// ---------- Variables de estado ----------
bool alarmaActiva = false;
bool movimientoDetectado = false;
String claveCorrecta = "1234"; // Clave para activar/desactivar
String claveIngresada = "";

// ---------- Configuración del teclado ----------
const byte FILAS = 4; // 4 filas
const byte COLUMNAS = 4; // 4 columnas
char teclas[FILAS][COLUMNAS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte pinesFilas[FILAS] = {9, 8, 7, 6};
byte pinesColumnas[COLUMNAS] = {5, 4, 3, 2};

Keypad teclado = Keypad(makeKeymap(teclas), pinesFilas, pinesColumnas, FILAS, COLUMNAS);

void setup() {
  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarilloBuzzer, OUTPUT);
  pinMode(pirPin, INPUT);

  digitalWrite(ledRojo, HIGH); // Comienza desactivada
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledAmarilloBuzzer, LOW);

  Serial.begin(9600);
  Serial.println("Sistema de alarma iniciado. Clave para activar/desactivar: 1234");
}

void loop() {
  char tecla = teclado.getKey();

  // Ingreso de clave
  if (tecla) {
    if (tecla == '#') {
      verificarClave();
    } else if (tecla == '*') {
      claveIngresada = "";
      Serial.println("Clave reiniciada.");
    } else {
      claveIngresada += tecla;
      Serial.print("*");
    }
  }

  // Si la alarma está activa, verificar sensor PIR
  if (alarmaActiva) {
    int pirEstado = digitalRead(pirPin);
    if (pirEstado == HIGH) {
      activarAlarma();
    } else {
      digitalWrite(ledAmarilloBuzzer, LOW);
    }
  }
}

// ---------- Función para verificar la clave ----------
void verificarClave() {
  if (claveIngresada == claveCorrecta) {
    if (!alarmaActiva) {
      activarSistema();
    } else {
      desactivarSistema();
    }
  } else {
    claveIncorrecta();
  }
  claveIngresada = "";
}

// ---------- Activar sistema ----------
void activarSistema() {
  Serial.println("\nClave correcta. Alarma activada.");
  alarmaActiva = true;

  // Parpadeo breve de confirmación
  for (int i = 0; i < 2; i++) {
    digitalWrite(ledVerde, HIGH);
    delay(200);
    digitalWrite(ledVerde, LOW);
    delay(200);
  }

  digitalWrite(ledVerde, HIGH); // Queda encendido
  digitalWrite(ledRojo, LOW);
}

// ---------- Desactivar sistema ----------
void desactivarSistema() {
  Serial.println("\nAlarma desactivada.");
  alarmaActiva = false;

  digitalWrite(ledVerde, LOW);
  digitalWrite(ledRojo, HIGH);
  digitalWrite(ledAmarilloBuzzer, LOW);
}

// ---------- Clave incorrecta ----------
void claveIncorrecta() {
  Serial.println("\nClave incorrecta.");
  // Parpadea brevemente el verde
  for (int i = 0; i < 2; i++) {
    digitalWrite(ledVerde, HIGH);
    delay(150);
    digitalWrite(ledVerde, LOW);
    delay(150);
  }
  // Enciende rojo nuevamente
  digitalWrite(ledRojo, HIGH);
}

// ---------- Activar alarma sonora y visual ----------
void activarAlarma() {
  Serial.println("Movimiento detectado!");
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledAmarilloBuzzer, HIGH);
    delay(150);
    digitalWrite(ledAmarilloBuzzer, LOW);
    delay(150);
  }
}

