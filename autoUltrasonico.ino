// Pines del Sensor Ultrasónico
const int trigger = 3;
const int echo = 2;

// Pines de Dirección (Digitales)
// Motor A (Derecho)
int IN1 = 4;
int IN2 = 5; 
// Motor B (Izquierdo)
int IN3 = 6;
int IN4 = 7; 

// Pines de Velocidad (PWM)
const int ENA = 9;  // Conectado al pin ENA del L298N (PWM)
const int ENB = 10; // Conectado al pin ENB del L298N (PWM)

// Variables para el sensor y la velocidad
long tiempo;
int distancia; 

// --- Configuración de Velocidad ---
const int VELOCIDAD_AVANCE = 150; 
const int VELOCIDAD_RETROCESO = 100; 
const int VELOCIDAD_GIRO = 120; // Nueva velocidad para el giro (puede ser menor)

// --- Valores de Comportamiento ---
const int DISTANCIA_PARADA = 20; 
const int TIEMPO_RETROCESO = 500; // Tiempo en ms para retroceder
const int TIEMPO_GIRO = 400;      // Tiempo en ms para girar y evadir el obstáculo. ¡AJÚSTALO!
const int TIEMPO_PAUSA = 100; 

void setup() {
  // Inicialización de pines (Se mantiene igual)
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); 
  pinMode(ENB, OUTPUT);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);

  digitalWrite(trigger, LOW);
  
  detener();
  delay(1000); 
}

// ------------------------------------
// --- FUNCIONES DE MOVIMIENTO BASE ---
// ------------------------------------

void avanzar() {
  digitalWrite(IN1, HIGH); 
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); 
  digitalWrite(IN4, LOW);
  
  analogWrite(ENA, VELOCIDAD_AVANCE); 
  analogWrite(ENB, VELOCIDAD_AVANCE);
}

void retroceder() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH); 
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  
  analogWrite(ENA, VELOCIDAD_RETROCESO);
  analogWrite(ENB, VELOCIDAD_RETROCESO);
}

void detener() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// ------------------------------------
// --- NUEVAS FUNCIONES DE GIRO ---
// ------------------------------------

void girarIzquierda() {
  // Gira sobre su eje: El Motor A avanza y el Motor B retrocede.
  
  // Motor A (Derecho): AVANZA
  digitalWrite(IN1, HIGH); 
  digitalWrite(IN2, LOW);
  
  // Motor B (Izquierdo): RETROCEDE
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  
  // Aplicar velocidad de giro
  analogWrite(ENA, VELOCIDAD_GIRO);
  analogWrite(ENB, VELOCIDAD_GIRO);
}

void girarDerecha() {
  // Gira sobre su eje: El Motor A retrocede y el Motor B avanza.
  
  // Motor A (Derecho): RETROCEDE
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, HIGH);
  
  // Motor B (Izquierdo): AVANZA
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  
  // Aplicar velocidad de giro
  analogWrite(ENA, VELOCIDAD_GIRO);
  analogWrite(ENB, VELOCIDAD_GIRO);
}


// --- Función de Lectura del Sensor (Se mantiene igual) ---

int leerDistancia() {
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  
  long tiempo = pulseIn(echo, HIGH);
  int distancia = tiempo / 59; 
  
  return distancia;
}

// ------------------------------------
// --- BUCLE PRINCIPAL (LÓGICA MEJORADA) ---
// ------------------------------------

void loop() {
  int dist = leerDistancia();
  
  if (dist < DISTANCIA_PARADA) {
    
    // 1. Detenerse y frenar
    detener();
    delay(TIEMPO_PAUSA); 
    
    // 2. Retroceder para tener espacio
    retroceder();
    delay(TIEMPO_RETROCESO); 
    
    // 3. Detenerse antes de girar
    detener();
    delay(TIEMPO_PAUSA); 
    
    // 4. Ejecutar la maniobra de giro (Ejemplo: Girar a la izquierda)
    girarIzquierda();
    delay(TIEMPO_GIRO); // Gira el tiempo suficiente para cambiar de rumbo.
    
    // 5. Detenerse para re-evaluar la distancia
    detener();
    delay(TIEMPO_PAUSA); 
    
  } else {
    // 6. No hay obstáculo, continuar avanzando
    avanzar();
    delay(50); 
  }
}