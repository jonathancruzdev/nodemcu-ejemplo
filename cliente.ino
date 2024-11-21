#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define LED_PIN 12 //D6  // Pin del LED (GPIO12)


const char* ssid = "";
const char* password = "";
const String serverUrl = "http://192.168.1.4/api/servidor.php";


WiFiClient client; // Cliente Wi-Fi para el ESP8266

void setup() {
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);

  Serial.begin(115200); // Iniciamos la comunicación serial
  Serial.println("Iniciando ESP8266...");

  // Conexión a la red Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Conectando a Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConexión establecida con Wi-Fi.");
}

void loop() {
    digitalWrite(LED_PIN, HIGH);
    enviarDatos("Encendido");
    delay(5000); 
    digitalWrite(LED_PIN, LOW);
    enviarDatos("Apagado");
    delay(5000);
}

void enviarDatos(String estado) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http; // Objeto HTTPClient

    // Iniciamos la conexión con el servidor
    http.begin(client, serverUrl);
    // Agregamos encabezado de contenido
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    // Creamos el cuerpo de la solicitud
    String postData = "estado=" + String(estado);
    // Realizamos el POST
    int httpResponseCode = http.POST(postData);
    // Manejamos la respuesta del servidor
    if (httpResponseCode > 0) {
      Serial.println("Respuesta del servidor: " + String(httpResponseCode));
      Serial.println("Cuerpo: " + http.getString());
    } else {
      Serial.println("Error al enviar datos: " + String(http.errorToString(httpResponseCode)));
    }

    http.end(); // Finalizamos la conexión
  } else {
    Serial.println("No conectado a Wi-Fi");
     // Intentamos reconectar
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }
    Serial.println("\nReconectado a Wi-Fi.");
  }
}