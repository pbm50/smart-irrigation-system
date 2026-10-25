
/*
 * SMART IRRIGATION
 * Version: 0.1.0
 * Recovery and session management
 */

const int SENSOR_PIN = 34;
const int BOMBA_PIN = 26;
const int BOTON_PIN = 27;

const float HUMEDAD_INICIO = 30.0;
const float HUMEDAD_RECUPERADA = 40.0;

const unsigned long TIEMPO_RIEGO = 5000;
const unsigned long TIEMPO_ESPERA = 15000;
const unsigned long TIEMPO_ESTABILIDAD = 30000;
const unsigned long INTERVALO_LECTURA = 1000;
const unsigned long ANTIRREBOTE = 50;

const int MAX_CICLOS = 3;

enum EstadoRiego {
  MONITOREANDO,
  REGANDO,
  ESPERANDO,
  VERIFICANDO_RECUPERACION,
  BLOQUEADO
};

EstadoRiego estado = MONITOREANDO;

unsigned long inicioEstado = 0;
unsigned long ultimaLectura = 0;
unsigned long ultimoCambioBoton = 0;
unsigned long inicioRecuperacion = 0;

int ciclos = 0;
float humedad = 0;

bool lecturaDisponible = false;
bool sesionActiva = false;

bool botonAnterior = HIGH;
bool botonEstable = HIGH;

void cambiarEstado(EstadoRiego nuevoEstado,
                   unsigned long ahora) {
  estado = nuevoEstado;
  inicioEstado = ahora;

  digitalWrite(BOMBA_PIN,
               estado == REGANDO ? HIGH : LOW);

  switch (estado) {
    case MONITOREANDO:
      Serial.println("ESTADO: MONITOREANDO");
      break;
    case REGANDO:
      Serial.println("ESTADO: REGANDO");
      break;
    case ESPERANDO:
      Serial.println("ESTADO: ESPERANDO");
      break;
    case VERIFICANDO_RECUPERACION:
      inicioRecuperacion = ahora;
      Serial.println("ESTADO: VERIFICANDO RECUPERACION");
      break;
    case BLOQUEADO:
      Serial.println("ALERTA: SISTEMA BLOQUEADO");
      break;
  }
}

void revisarBoton(unsigned long ahora) {
  bool lectura = digitalRead(BOTON_PIN);

  if (lectura != botonAnterior) {
    ultimoCambioBoton = ahora;
  }

  if (ahora - ultimoCambioBoton >= ANTIRREBOTE) {
    if (lectura != botonEstable) {
      botonEstable = lectura;

      if (botonEstable == LOW && estado == BLOQUEADO) {
        ciclos = 0;
        sesionActiva = false;
        lecturaDisponible = false;

        Serial.println("EVENTO: REINICIO MANUAL");
        cambiarEstado(MONITOREANDO, ahora);
      }
    }
  }

  botonAnterior = lectura;
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);

  digitalWrite(BOMBA_PIN, LOW);
  pinMode(BOMBA_PIN, OUTPUT);
  pinMode(BOTON_PIN, INPUT_PULLUP);

  Serial.println("Smart Irrigation - V0.1.0");
}

void loop() {
  unsigned long ahora = millis();

  revisarBoton(ahora);

  if (ahora - ultimaLectura >= INTERVALO_LECTURA) {
    ultimaLectura = ahora;

    int lectura = analogRead(SENSOR_PIN);
    humedad = lectura * 100.0 / 4095.0;
    lecturaDisponible = true;

    Serial.print("Humedad: ");
    Serial.print(humedad, 1);
    Serial.print("% | Ciclos: ");
    Serial.print(ciclos);
    Serial.print("/");
    Serial.print(MAX_CICLOS);

    Serial.print(" | Estado: ");
    Serial.println(estado);

    if (estado == VERIFICANDO_RECUPERACION) {
      unsigned long transcurrido =
          (ahora - inicioRecuperacion) / 1000;

      Serial.print("Recuperacion: ");
      Serial.print(transcurrido);
      Serial.println("/30 segundos");
    }
  }

  if (!lecturaDisponible) return;

  // Start recovery only during an active session
  if (sesionActiva &&
      estado != BLOQUEADO &&
      estado != VERIFICANDO_RECUPERACION &&
      humedad >= HUMEDAD_RECUPERADA) {
    cambiarEstado(VERIFICANDO_RECUPERACION, ahora);
  }

  switch (estado) {

    case MONITOREANDO:
      if (humedad < HUMEDAD_INICIO) {
        if (ciclos >= MAX_CICLOS) {
          cambiarEstado(BLOQUEADO, ahora);
        } else {
          sesionActiva = true;
          ciclos++;
          cambiarEstado(REGANDO, ahora);
        }
      }
      break;

    case REGANDO:
      if (ahora - inicioEstado >= TIEMPO_RIEGO) {
        cambiarEstado(ESPERANDO, ahora);
      }
      break;

    case ESPERANDO:
      if (ahora - inicioEstado >= TIEMPO_ESPERA) {
        cambiarEstado(MONITOREANDO, ahora);
      }
      break;

    case VERIFICANDO_RECUPERACION:
      if (humedad < HUMEDAD_RECUPERADA) {
        Serial.println("RECUPERACION INTERRUMPIDA");
        cambiarEstado(ESPERANDO, ahora);
      }
      else if (ahora - inicioRecuperacion >=
               TIEMPO_ESTABILIDAD) {

        ciclos = 0;
        sesionActiva = false;

        Serial.println("EVENTO: HUMEDAD RECUPERADA");
        Serial.println("EVENTO: CONTADOR REINICIADO");

        cambiarEstado(MONITOREANDO, ahora);
      }
      break;

    case BLOQUEADO:
      // Pump remains OFF
      break;
  }
}
