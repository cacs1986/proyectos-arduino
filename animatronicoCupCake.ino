#include <Servo.h>

const int PIN_SERVO_OJOS = 9;
const int PIN_SERVO_BOCA = 10;

// Joystick (Analógico y Botón)
const int PIN_JOY_X = A0; // Eje X del joystick (para Ojos)
const int PIN_JOY_Y = A1; // Eje Y del joystick (para Boca)
const int PIN_BOTON = 5;  // Botón del joystick (para LEDs/Vela)

// LEDs y Vela (Salidas Digitales)
const int PIN_LED_1 = 2;
const int PIN_LED_2 = 3;
const int PIN_VELA = 4; // Vela (o el pin que active su circuito)

// --- Definición de Ángulos y Límites ---
// Ojos (Rango: 20 a 100)
const int OJOS_IZQUIERDA = 20; 
const int OJOS_DERECHA = 100;
const int OJOS_FRENTE = 60; 

// Boca (Rango: 90 a 170)
const int BOCA_ABIERTA = 90; 
const int BOCA_CERRADA = 170; // El valor más alto es el que cierra la boca

// --- Objetos Servo ---
Servo servoOjos;
Servo servoBoca;

// --- Variables de Estado ---
int estadoBoton = 0;
bool encendidoLEDs = false; // Controla el estado actual de los LEDs

void setup() {
  // Inicializar Servos
  servoOjos.attach(PIN_SERVO_OJOS);
  servoBoca.attach(PIN_SERVO_BOCA);

  // Mover a la posición de reposo (ojos al frente, boca cerrada)
  servoOjos.write(OJOS_FRENTE);
  servoBoca.write(BOCA_CERRADA); 

  // Inicializar Pines Digitales
  pinMode(PIN_BOTON, INPUT_PULLUP); // Usar pullup para que el botón sea LOW al pulsarse
  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_2, OUTPUT);
  pinMode(PIN_VELA, OUTPUT);

  // Asegurar que las luces están apagadas al inicio
  digitalWrite(PIN_LED_1, LOW);
  digitalWrite(PIN_LED_2, LOW);
  digitalWrite(PIN_VELA, LOW);

  // Opcional para depuración
  // Serial.begin(9600); 
}

void loop() {
  // ------------------------------------
  // 1. Control de Ojos (Eje X del Joystick)
  // ------------------------------------
  int valorX = analogRead(PIN_JOY_X);
  
  // Mapear el valor del joystick (0 a 1023) al rango de ángulos de los ojos (20 a 100)
  // El mapeo va de 0 a 1023 -> de 20 (izquierda) a 100 (derecha)
  int anguloOjos = map(valorX, 0, 1023, OJOS_IZQUIERDA, OJOS_DERECHA);

  // Mover el servo de los ojos
  servoOjos.write(anguloOjos);

  // ------------------------------------
  // 2. Control de Boca (Eje Y del Joystick)
  // ------------------------------------
  int valorY = analogRead(PIN_JOY_Y);
  
  // Mapear el valor del joystick (0 a 1023) al rango de ángulos de la boca (90 a 170)
  // NOTA: Para la boca, queremos que el valor bajo del joystick (0) abra la boca (90)
  // y el valor alto del joystick (1023) cierre la boca (170).
  int anguloBoca = map(valorY, 0, 1023, BOCA_ABIERTA, BOCA_CERRADA);

  // Mover el servo de la boca
  servoBoca.write(anguloBoca);

  // ------------------------------------
  // 3. Control de LEDs y Vela (Botón del Joystick)
  // ------------------------------------
  estadoBoton = digitalRead(PIN_BOTON);

  // El botón está conectado con PULLUP, por lo que pulsado es LOW.
  if (estadoBoton == LOW) {
    // Esperar un pequeño delay para evitar rebotes del botón
    delay(50);
    
    // Verificar si sigue pulsado
    if (digitalRead(PIN_BOTON) == LOW) {
      
      // Invertir el estado de los LEDs
      encendidoLEDs = !encendidoLEDs; 
      
      if (encendidoLEDs) {
        // Encender
        digitalWrite(PIN_LED_1, HIGH);
        digitalWrite(PIN_LED_2, HIGH);
        digitalWrite(PIN_VELA, HIGH);
      } else {
        // Apagar
        digitalWrite(PIN_LED_1, LOW);
        digitalWrite(PIN_LED_2, LOW);
        digitalWrite(PIN_VELA, LOW);
      }
      
      // Esperar hasta que se suelte el botón para evitar que se cambie el estado
      // múltiples veces mientras se mantiene pulsado.
      while (digitalRead(PIN_BOTON) == LOW) {
        delay(10);
      }
    }
  }
  
  // Pequeño delay para estabilizar la lectura de los servos
  delay(15); 
}
