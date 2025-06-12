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
  if (valor < 500) return "Vazio";
  else if (valor < 2500) return "Medio";
  else return "Lotado";
}

char simboloNivel(int valor) {
  if (valor < 500) return 'O'; 
  else if (valor < 2500) return '~'; 
  else return 'X'; 
}
float calcularPorcentagemOcupacao(int valorSensor, int minVazio = 500, int maxLotado = 3500) {
  

  
  if (valorSensor < minVazio) {
    return 0.0; 
  }
  if (valorSensor > maxLotado) {
    return 100.0; 
  }

  
  float porcentagem = ((float)(valorSensor - minVazio) / (maxLotado - minVazio)) * 100.0;
  
  
  return constrain(porcentagem, 0.0, 100.0); 
}
void setup_wifi() {
 delay(10);
 Serial.println();
 Serial.print("Connecting to ");
 Serial.println(ssid);


 WiFi.mode(WIFI_STA);
 WiFi.begin(ssid, password);


 while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".");
 }


 randomSeed(micros());


 Serial.println("");
 Serial.println("WiFi connected");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
}
void callback(char* topic, byte* payload, unsigned int length) {
 Serial.print("Message arrived [");
 Serial.print(topic);
 Serial.print("] ");
 for (int i = 0; i < length; i++) {
   Serial.print((char)payload[i]);
 }}
void reconnect() {
 while (!client.connected()) {
   Serial.print("Attempting MQTT connection...");
   String clientId = "ESP32Client-";
   clientId += String(random(0xffff), HEX);
   if (client.connect(clientId.c_str())) {
     Serial.println("Connected");
     client.publish("/hospital/Publish", "Welcome");
     client.subscribe("/hospital/Subscribe");
   } else {
     Serial.print("failed, rc=");
     Serial.print(client.state());
     Serial.println(" try again in 5 seconds");
     delay(5000);
   }}
}
void setup() {
  setup_wifi();
  client.setServer(mqttServer, 1883);
  client.setCallback(callback);
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

  
 
}

void loop() {
  if (!client.connected()) {
   reconnect();
 }
 client.loop();
  int sala1 = analogRead(sensorsala1);
  int sala2 = analogRead(sensorsala2);
  int sala3 = analogRead(sensorsala3);
  int porcentagemSala1 = calcularPorcentagemOcupacao(sala1);
  int porcentagemSala2 = calcularPorcentagemOcupacao(sala2);
  int porcentagemSala3 = calcularPorcentagemOcupacao(sala3);
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

  
  String msg = "Hora: " + hora + " | Sala1: " + nivelOcupacao(sala1) +" "+porcentagemSala1+"%" + ", Sala2: " + nivelOcupacao(sala2) +" "+porcentagemSala2 +"%"+", Sala3: " + nivelOcupacao(sala3)+" "+porcentagemSala3+"%";

  Serial.println(msg);
  client.publish("hospital/salas", msg.c_str());
  client.publish("hospital/sala1", String(porcentagemSala1).c_str());
  client.publish("hospital/sala2", String(porcentagemSala2).c_str());
  client.publish("hospital/sala3", String(porcentagemSala3).c_str());
  
  delay(2000);
}

