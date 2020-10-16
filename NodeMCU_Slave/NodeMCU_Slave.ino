#include <ESP8266WiFi.h>
#include <espnow.h>

// exemplo de estrutura para receber dados
// deve coincidir com a estrutura no NodeMCU principal
typedef struct struct_message {
    char a[32];
    int b;
    float c;
    String d;
    bool e;
} struct_message;

// cria uma struct_message chamada myData
struct_message myData;

// Função de retorno que será execultada quando os dados são recebidos
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Char: ");
  Serial.println(myData.a);
  Serial.print("Int: ");
  Serial.println(myData.b);
  Serial.print("Float: ");
  Serial.println(myData.c);
  Serial.print("String: ");
  Serial.println(myData.d);
  Serial.print("Bool: ");
  Serial.println(myData.e);
  Serial.println();
}
 
void setup() {
  // inicializa monitor serial
  Serial.begin(115200);
  
  // Define o dispositivo com estação Wi-Fi
  WiFi.mode(WIFI_STA);

  // inicia o ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Quando ESPNow é inicializado, ele define o nó como escravo para receber dados
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  
}
