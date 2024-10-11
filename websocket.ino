#include <WiFi.h>
#include <WebSocketsServer.h>

// Remplacez par votre SSID et mot de passe
const char* ssid = "PoleDeVinci_IFT";
const char* password = "*c.r4UV@VfPn_0";

// Création d'un serveur WebSocket sur le port 81
WebSocketsServer webSocket = WebSocketsServer(81);

const int echoPin = 1;
const int trigPin = 2;
float duration, distance; 
unsigned long previousMillis = 0; // Variable pour stocker le temps précédent
const long interval = 1000;       // Intervalle de 1 seconde

const int motorPin = 3;  // Pin pour activer/désactiver le transistor (qui contrôle le moteur)

const int ledPin = 5;

  // Pin du bouton (vous pouvez modifier la GPIO si nécessaire)
const int buttonPin = 6;
bool lastButtonState = HIGH;  // Dernier état connu du bouton
bool motorState = LOW;  // État initial désactivé

void setup() {
  Serial.begin(115200);

  // Connexion au Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connexion au Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" connecté !");
  Serial.print("Adresse IP: ");
  Serial.println(WiFi.localIP());

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

    // Configurer la pin de la LED
  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, 125); // Éteindre la LED au démarrage

  pinMode(motorPin, OUTPUT);
  digitalWrite(motorPin, LOW); // Désactiver le moteur au démarrage

  // Configurer le bouton
  pinMode(buttonPin, INPUT_PULLUP);  // Utilisation de la résistance de pull-up interne

  // Start WebSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent); 
}

void loop() {
  webSocket.loop();

  // Obtenir le temps actuel
  unsigned long currentMillis = millis();

  // Si 1 seconde s'est écoulée, lire la distance du capteur
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // Sauvegarder le dernier temps

    // Mesurer la distance
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH); 
    distance = (duration * 0.0343) / 2; 
    Serial.print("Distance: ");  
    Serial.println(distance);  

    webSocket.broadcastTXT("d"+String(distance));
  }

    // Lire l'état du bouton
  bool buttonState = digitalRead(buttonPin);
  
  // Détecter un changement d'état du bouton (passage de HIGH à LOW)
  if (buttonState == LOW && lastButtonState == HIGH) {
    Serial.println("Bouton pressé");
    // Envoyer un message via WebSocket au client
    webSocket.broadcastTXT("buttonPressed");  // Envoi d'un message à tous les clients
  }

  // Mettre à jour l'état précédent du bouton
  lastButtonState = buttonState;
}

// Fonction qui gère les événements WebSocket
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_TEXT:
      Serial.printf("Message reçu du client %u : %s\n", num, payload);
      
      // Si le message contient "ledBrightness", extraire la valeur et ajuster la luminosité
      if (strncmp((const char*)payload, "ledBrightness:", 14) == 0) {
        int brightness = atoi((const char*)payload + 14);  // Extraire la valeur de luminosité
        brightness = constrain(brightness, 0, 255);  // Assurer que la valeur soit entre 0 et 255
        analogWrite(ledPin, brightness);  // Ajuster la luminosité de la LED
        Serial.printf("Luminosité ajustée à : %d\n", brightness);
      }

      // Si le message est "toggleMotor", basculer l'état du moteur
      else if (strcmp((const char*)payload, "toggleMotor") == 0) {
        motorState = !motorState;  // Basculer l'état du moteur
        
        // Appliquer l'état du moteur (activation ou désactivation)
        if (motorState) {
          digitalWrite(motorPin, HIGH);  // Activer le moteur
          Serial.println("Moteur activé");
        } else {
          digitalWrite(motorPin, LOW);  // Désactiver le moteur
          Serial.println("Moteur désactivé");
        }
      }
      break;
  }
}

