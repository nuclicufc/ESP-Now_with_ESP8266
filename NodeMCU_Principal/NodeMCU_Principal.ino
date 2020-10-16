

#include <ESP8266WiFi.h>
#include <espnow.h>

// MAC Address do NodeMCU  que vai receber dados
uint8_t broadcastAddress1[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t broadcastAddress2[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t broadcastAddress3[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};


// exemplo de estrutura para enviar dados
// deve coincidir com a estrutura no NodeMCU que receber dados
typedef struct test_struct {
    int x;
    int y;
    int z;
} test_struct;

// cria uma struct_message chamada test para armazenar variáveis para serem enviadas
test_struct test;
test_struct test2;
test_struct test3;

unsigned long lastTime = 0;  
unsigned long timerDelay = 2000;  // send readings timer

// retorno quando a mensagem é enviada
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  char macStr[18];
  Serial.print("Packet to:");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
         mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  
  // Uma vez que o ESPNow é inicializado com sucesso, definimos este no com principal
  // ele irá receber o status dos pacotes envadosg
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress1, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(broadcastAddress2, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(broadcastAddress3, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
 
void loop() {
  if ((millis() - lastTime) > timerDelay) {
    // Set values to send
    test.x = random(1, 50);
    test.y = random(1, 50);
    test.z = random(1, 50);
    test2.x = random(1, 50);
    test2.y = random(1, 50);
    test3.z = random(1, 50); 

    
    // Envia uma mensagem para o ESPNow
    esp_now_send(0, (uint8_t *) &test, sizeof(test));
    esp_now_send(broadcastAddress1, (uint8_t *) &test, sizeof(test));
    esp_now_send(broadcastAddress2, (uint8_t *) &test2, sizeof(test2));
    esp_now_send(broadcastAddress3, (uint8_t *) &test, sizeof(test3));
    
    lastTime = millis();
  }
}
