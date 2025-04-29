#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <RTClib.h>
#include <U8g2lib.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";


const char* mqttServer = "test.mosquitto.org";
int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);


#define sensorsala1 34
#define sensorsala2 35
#define sensorsala3 32
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

RTC_DS1307 rtc;

String nivelOcupacao(int valor) {
  if (valor < 341) return "Vazio";
  else if (valor < 682) return "Medio";
  else return "Lotado";
}

char simboloNivel(int valor) {
  if (valor < 341) return 'O'; 
  else if (valor < 682) return '~'; 
  else return 'X'; 
}

void setup() {
  Serial.begin(115200);
  
  
  Wire.begin();
  u8g2.begin();
  if (!rtc.begin()) {
    Serial.println("RTC não encontrado!");
    while (1);
  }

  
  if (!rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); 
    Serial.println("RTC ajustado com hora de compilação.");
  }

  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado ao Wi-Fi!");

  
  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect("ESP32ClientHospital")) {
      Serial.println("Conectado!");
    } else {
      Serial.print("Erro: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void loop() {
  
  int sala1 = analogRead(sensorsala1);
  int sala2 = analogRead(sensorsala2);
  int sala3 = analogRead(sensorsala3);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr); 

  
  u8g2.drawStr(10, 10, "Status das Salas:");

  
  u8g2.drawFrame(5, 20, 35, 20);
  u8g2.setCursor(10, 35);
  u8g2.print(simboloNivel(sala1));
  u8g2.drawStr(45, 35, nivelOcupacao(sala1).c_str());

  
  u8g2.drawFrame(5, 45, 35, 20);
  u8g2.setCursor(10, 60);
  u8g2.print(simboloNivel(sala2));
  u8g2.drawStr(45, 60, nivelOcupacao(sala2).c_str());

  
  u8g2.drawFrame(75, 20, 35, 20);
  u8g2.setCursor(80, 35);
  u8g2.print(simboloNivel(sala3));
  u8g2.drawStr(115, 35, nivelOcupacao(sala3).c_str());

  u8g2.sendBuffer();
  
  DateTime agora = rtc.now();
  String hora = String(agora.hour()) + ":" + String(agora.minute()) + ":" + String(agora.second());

  
  String msg = "Hora: " + hora + " | Sala1: " + nivelOcupacao(sala1) +", Sala2: " + nivelOcupacao(sala2) + ", Sala3: " + nivelOcupacao(sala3);

  Serial.println(msg);
  client.publish("hospital/salas", msg.c_str());

  delay(2000);
}
