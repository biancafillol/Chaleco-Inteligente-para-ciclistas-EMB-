#include <MD_MAX72xx.h>
#define DEBUG 1
#include <WiFi.h>
#include <WiFiServer.h>

const char* ssid = "MovistarFibra-43438A";
const char* password = "CLG457G92KFgw33LB6N2";
const uint16_t serverPort = 8080;

WiFiServer server(serverPort);

#if DEBUG
#define PRINT(s, x) { Serial.print(F(s)); Serial.print(x); }
#define PRINTS(x) Serial.print(F(x))
#define PRINTD(x) Serial.println(x, DEC)
#else
#define PRINT(s, x)
#define PRINTS(x)
#define PRINTD(x)
#endif

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define CLK_PIN 18 //CLK MATRIZ
#define DATA_PIN 23 //DIN MATRIZ
#define CS_PIN 5 //CS MATRIZ

MD_MAX72XX matriz = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
#define DELAYTIME 100 // en milisegundos

volatile bool intermitenteActivo = false;
unsigned long previousMillis = 0;
const long interval = 500; // Intervalo de parpadeo (500 ms)
bool estadoIntermitente = false;

void encenderDireccionDerecha()
{
  uint8_t arrow[COL_SIZE] =
  {
    0b00001000,
    0b00011100,
    0b00111110,
    0b01111111,
    0b00011100,
    0b00011100,
    0b00111110,
    0b00000000
  };

  MD_MAX72XX::transformType_t t[] =
  {
    MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL,
    MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL,
    MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL,
    MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL,
  };

  matriz.clear();

  matriz.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
  for (uint8_t j = 0; j < matriz.getDeviceCount(); j++)
    matriz.setBuffer(((j + 1) * COL_SIZE) - 1, COL_SIZE, arrow);
  matriz.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
  delay(DELAYTIME);

  matriz.control(MD_MAX72XX::WRAPAROUND, MD_MAX72XX::ON);
  for (uint8_t i = 0; i < (sizeof(t) / sizeof(t[0])); i++)
  {
    matriz.transform(t[i]);
    delay(DELAYTIME * 5);
  }
  matriz.control(MD_MAX72XX::WRAPAROUND, MD_MAX72XX::OFF);

  matriz.clear();
}

void encenderDireccionIzquierda() {
  uint8_t arrow[8] = {
    0b00000000,
    0b00111110,
    0b00011100,
    0b00011100,
    0b01111111,
    0b00111110,
    0b00011100,
    0b00001000
  };

  MD_MAX72XX::transformType_t t[] = {
    MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
    MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
    MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
    MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR
  };

  matriz.clear();

  matriz.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
  for (uint8_t j = 0; j < matriz.getDeviceCount(); j++)
    matriz.setBuffer(((j + 1) * 8) - 1, 8, arrow);
  matriz.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
  delay(DELAYTIME);

  matriz.control(MD_MAX72XX::WRAPAROUND, MD_MAX72XX::ON);
  for (uint8_t i = 0; i < (sizeof(t) / sizeof(t[0])); i++) {
    matriz.transform(t[i]);
    delay(DELAYTIME * 5);
  }
  matriz.control(MD_MAX72XX::WRAPAROUND, MD_MAX72XX::OFF);

  matriz.clear();
}

void encenderFrenado() {
  // Enciende toda la matriz simultáneamente
  matriz.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
  for (uint8_t dev = 0; dev < matriz.getDeviceCount(); dev++) {
    for (uint8_t col = 0; col < 8; col++) {
      matriz.setColumn((dev * 8) + col, 0xFF); // Enciende todos los LEDs de la columna actual en el dispositivo actual
    }
  }
  matriz.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
  
  delay(5000); // Espera 5 segundos
  matriz.clear(); // Apaga todos los LEDs de la matriz después de 5 segundos
}

void encenderIntermitente() {
  intermitenteActivo = true;
}

void manejarIntermitente() {
  if (intermitenteActivo) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      if (estadoIntermitente) {
        matriz.clear(); // Apaga todos los LEDs de la matriz
      } else {
        matriz.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
        for (uint8_t dev = 0; dev < matriz.getDeviceCount(); dev++) {
          for (uint8_t col = 0; col < 8; col++) {
            matriz.setColumn((dev * 8) + col, 0xFF); // Enciende todos los LEDs de la columna actual en el dispositivo actual
          }
        }
        matriz.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
      }
      estadoIntermitente = !estadoIntermitente;
    }
  }
}

void setup()
{
#if DEBUG
  Serial.begin(57600);
#endif

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  server.begin();

  if (!matriz.begin())
    PRINTS("\nNo se pudo inicializar el MD_MAX72XX");
}

void loop()
{
  WiFiClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        String linea = client.readStringUntil('\n'); 
        elegirDireccion(linea);                      
      }                                                     
    client.stop();                                          
    }      
  }

  manejarIntermitente(); // Maneja el intermitente si está activo
}

void elegirDireccion(String linea) {
  if (linea.indexOf("A") >= 0) {
    intermitenteActivo = false; // Detiene encenderIntermitente si está activo
    encenderDireccionIzquierda(); // <-- <---
  } else if (linea.indexOf("B") >= 0) {
    intermitenteActivo = false; // Detiene encenderIntermitente si está activo
    encenderDireccionDerecha();  // --> --->
  } else if (linea.indexOf("C") >= 0) {
    intermitenteActivo = false; // Detiene encenderIntermitente si está activo
    encenderFrenado();
  } else if (linea.indexOf("D") >= 0) {
    if (intermitenteActivo) {
      intermitenteActivo = false; // Detiene encenderIntermitente si está activo
    } else {
      encenderIntermitente();
    }
  }
}
