#include <Arduino.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <SPI.h>

#include "config.h"
#include "logo.h"
#include "utils.h"

// Definições de Hardware (usando config.h)
#define I2C_SDA I2C_SDA_PIN
#define I2C_SCL I2C_SCL_PIN
#define PIN_AP BUTTON_RESET_PIN
#define PIN_BUZZER BUZZER_PIN
#define BUTTON_PRESSED LOW

// Configurações do Display (usando config.h)
#define SCREEN_WIDTH OLED_WIDTH
#define SCREEN_HEIGHT OLED_HEIGHT
#define SCREEN_ADDRESS OLED_ADDRESS
#define SCREEN_RESET OLED_RESET_PIN

// Estrutura para dados do Protheus
struct ProtheusData {
  int totalLicenses;
  int usedLicenses;
  int activeUsers;
  float cpuUsage;
  float memoryUsage;
  bool isOnline;
  unsigned long lastUpdate;
  unsigned long systemUptime;
  String lastError;
};

// Objetos globais
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, SCREEN_RESET);
WiFiManager wm;

// Variáveis globais
ProtheusData proteusData = {0, 0, 0, 0.0, 0.0, false, 0, 0, ""};
unsigned long lastApiCall = 0;
unsigned long lastDisplayUpdate = 0;
unsigned long systemStartTime = 0;
int currentDisplayPage = 0;
bool alertActive = false;
unsigned long alertStartTime = 0;
int wifiRetryCount = 0;

// Função para inicializar o display
void initDisplay() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Falha ao inicializar display SSD1306"));
    for(;;); // Loop infinito em caso de erro
  }
  
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.drawBitmap(0, 0, epd_bitmap_logo_esp32_protheus_monitor, 128, 64, WHITE);
  display.display();
  delay(LOGO_DISPLAY_TIME);
  display.clearDisplay();
}

// Função para configurar WiFi
bool setupWiFi() {
  pinMode(PIN_AP, INPUT_PULLUP);
  
  // Verifica se o botão de reset está pressionado
  if (digitalRead(PIN_AP) == BUTTON_PRESSED) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Resetando WiFi...");
    display.display();
    
    wm.resetSettings();
    delay(2000);
  }

  wm.setConfigPortalBlocking(false);
  wm.setConfigPortalTimeout(WIFI_CONFIG_TIMEOUT);
  
  bool connected = wm.autoConnect(WIFI_AP_NAME, WIFI_AP_PASSWORD);
  
  if (!connected) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Modo Config WiFi");
    display.println("");
    display.print("SSID: ");
    display.println(WIFI_AP_NAME);
    display.print("Senha: ");
    display.println(WIFI_AP_PASSWORD);
    display.print("IP: ");
    display.println(WiFi.softAPIP());
    display.display();
    return false;
  }
  
  // WiFi conectado com sucesso
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("WiFi Conectado!");
  display.println("");
  display.print("SSID: ");
  display.println(WiFi.SSID());
  display.print("IP: ");
  display.println(WiFi.localIP());
  display.print("RSSI: ");
  display.print(WiFi.RSSI());
  display.println(" dBm");
  display.display();
  delay(3000);
  
  return true;
}

// Função para fazer chamada à API do Protheus
bool fetchProtheusData() {
  if (WiFi.status() != WL_CONNECTED) {
    if (DEBUG_ENABLED) Serial.println("WiFi não conectado");
    proteusData.lastError = "WiFi desconectado";
    proteusData.isOnline = false;
    return false;
  }

  HTTPClient http;
  http.begin(PROTHEUS_API_URL);
  
  // Configurar autenticação básica se necessário
  // http.setAuthorization(PROTHEUS_API_USER, PROTHEUS_API_PASSWORD);
  
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(HTTP_TIMEOUT);

  int httpResponseCode = http.GET();
  
  if (httpResponseCode > 0) {
    String payload = http.getString();
    if (DEBUG_ENABLED) {
      Serial.print("Resposta API (");
      Serial.print(httpResponseCode);
      Serial.print("): ");
      Serial.println(payload);
    }
    
    // Verificar se é JSON válido
    if (isValidJson(payload)) {
      // Parse JSON response
      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, payload);
      
      if (!error) {
        proteusData.totalLicenses = doc["totalLicenses"] | SIM_TOTAL_LICENSES;
        proteusData.usedLicenses = doc["usedLicenses"] | random(SIM_MIN_USED_LICENSES, SIM_MAX_USED_LICENSES);
        proteusData.activeUsers = doc["activeUsers"] | random(SIM_MIN_USERS, SIM_MAX_USERS);
        proteusData.cpuUsage = doc["cpuUsage"] | random(SIM_MIN_CPU * 10, SIM_MAX_CPU * 10) / 10.0;
        proteusData.memoryUsage = doc["memoryUsage"] | random(SIM_MIN_MEMORY * 10, SIM_MAX_MEMORY * 10) / 10.0;
        proteusData.isOnline = true;
        proteusData.lastUpdate = millis();
        proteusData.lastError = "";
        
        http.end();
        return true;
      } else {
        proteusData.lastError = "Erro JSON: ";
        proteusData.lastError += error.c_str();
        if (DEBUG_ENABLED) {
          Serial.print("Erro ao parsear JSON: ");
          Serial.println(error.c_str());
        }
      }
    } else {
      proteusData.lastError = "Resposta inválida da API";
      if (DEBUG_ENABLED) Serial.println("Resposta não é JSON válido");
    }
  } else {
    proteusData.lastError = "HTTP Error: ";
    proteusData.lastError += String(httpResponseCode);
    
    if (DEBUG_ENABLED) {
      Serial.print("Erro HTTP: ");
      Serial.println(httpResponseCode);
    }
  }
  
  http.end();
  
  // Usar dados simulados se habilitado e houve erro na API
  if (ENABLE_DATA_SIMULATION) {
    if (DEBUG_ENABLED) Serial.println("Usando dados simulados");
    generateSimulatedData(&proteusData);
    return true;
  }
  
  proteusData.isOnline = false;
  return false;
}

// Função para verificar se alertas devem ser ativados
void checkAlerts() {
  bool shouldAlert = false;
  
  // Verifica threshold de licenças
  float licenseUsagePercent = calculatePercentage(proteusData.usedLicenses, proteusData.totalLicenses);
  if (licenseUsagePercent >= LICENSE_ALERT_THRESHOLD) {
    shouldAlert = true;
  }
  
  // Verifica threshold de CPU
  if (proteusData.cpuUsage >= CPU_ALERT_THRESHOLD) {
    shouldAlert = true;
  }
  
  // Verifica threshold de memória
  if (proteusData.memoryUsage >= MEMORY_ALERT_THRESHOLD) {
    shouldAlert = true;
  }
  
  // Verifica sinal WiFi fraco
  if (WiFi.RSSI() <= WIFI_WEAK_SIGNAL_THRESHOLD) {
    shouldAlert = true;
  }
  
  // Ativa/desativa alerta
  if (shouldAlert && !alertActive) {
    alertActive = true;
    alertStartTime = millis();
    digitalWrite(PIN_BUZZER, HIGH);
    if (DEBUG_ENABLED) Serial.println("Alerta ativado!");
  } else if (!shouldAlert && alertActive) {
    alertActive = false;
    digitalWrite(PIN_BUZZER, LOW);
    if (DEBUG_ENABLED) Serial.println("Alerta desativado");
  }
  
  // Desliga buzzer após duração definida
  if (alertActive && (millis() - alertStartTime) > BUZZER_DURATION) {
    digitalWrite(PIN_BUZZER, LOW);
  }
}

// Função para exibir dados na tela (página 1 - Licenças)
void displayLicensesPage() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  
  centerText(display, "PROTHEUS MONITOR", 0);
  display.setCursor(0, 10);
  display.println("================");
  
  float usagePercent = calculatePercentage(proteusData.usedLicenses, proteusData.totalLicenses);
  
  display.print("Licencas: ");
  display.print(proteusData.usedLicenses);
  display.print("/");
  display.println(proteusData.totalLicenses);
  
  display.print("Uso: ");
  display.print(usagePercent, 1);
  display.println("%");
  
  display.print("Usuarios: ");
  display.println(proteusData.activeUsers);
  
  // Indicador de status
  display.print("Status: ");
  if (proteusData.isOnline) {
    display.println(MSG_SYSTEM_ONLINE);
  } else {
    display.println(MSG_SYSTEM_OFFLINE);
  }
  
  // Barra de progresso para licenças
  drawProgressBar(display, 14, 56, 100, 8, usagePercent);
  
  display.display();
}

// Função para exibir dados na tela (página 2 - Recursos)
void displayResourcesPage() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  
  centerText(display, "RECURSOS SISTEMA", 0);
  display.setCursor(0, 10);
  display.println("================");
  
  display.print("CPU: ");
  display.print(proteusData.cpuUsage, 1);
  display.println("%");
  
  display.print("Memoria: ");
  display.print(proteusData.memoryUsage, 1);
  display.println("%");
  
  display.print("WiFi: ");
  display.print(WiFi.RSSI());
  display.print(" dBm (");
  display.print(getWiFiSignalDescription(WiFi.RSSI()));
  display.println(")");
  
  // Última atualização
  unsigned long timeSinceUpdate = (millis() - proteusData.lastUpdate) / 1000;
  display.print("Atualizacao: ");
  display.print(timeSinceUpdate);
  display.println("s");
  
  // Barras de progresso para CPU e Memória
  // CPU
  display.setCursor(0, 48);
  display.print("CPU:");
  drawProgressBar(display, 30, 48, 82, 6, proteusData.cpuUsage, true);
  
  // Memória
  display.setCursor(0, 56);
  display.print("MEM:");
  drawProgressBar(display, 30, 56, 82, 6, proteusData.memoryUsage, true);
  
  display.display();
}

// Função para exibir página de informações do sistema
void displaySystemInfoPage() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  
  centerText(display, "INFO SISTEMA", 0);
  display.setCursor(0, 10);
  display.println("================");
  
  // Uptime do sistema
  char uptimeStr[32];
  formatUptime(millis() - systemStartTime, uptimeStr, sizeof(uptimeStr));
  display.print("Uptime: ");
  display.println(uptimeStr);
  
  // Memória livre
  display.print("RAM Livre: ");
  display.print(ESP.getFreeHeap() / 1024);
  display.println(" KB");
  
  // Frequência da CPU
  display.print("CPU: ");
  display.print(ESP.getCpuFreqMHz());
  display.println(" MHz");
  
  // Versão do firmware
  display.print("Firmware: v1.0.0");
  
  // Último erro (se houver)
  if (proteusData.lastError.length() > 0) {
    display.setCursor(0, 56);
    display.print("Erro: ");
    display.println(proteusData.lastError.substring(0, 15)); // Truncar se muito longo
  }
  
  display.display();
}

void setup() {
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println("=================================");
  Serial.println("ESP32 Protheus Monitor v1.0.0");
  Serial.println("Iniciando sistema...");
  Serial.println("=================================");

  systemStartTime = millis();

  // Inicializar I2C
  Wire.begin(I2C_SDA, I2C_SCL);
  
  // Configurar pinos
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);
  
  // Inicializar display
  initDisplay();
  
  // Configurar WiFi
  bool wifiConnected = setupWiFi();
  
  if (wifiConnected) {
    Serial.println("Sistema iniciado com sucesso!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("RSSI: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
    
    // Fazer primeira chamada à API
    fetchProtheusData();
  } else {
    Serial.println("Sistema iniciado em modo configuração WiFi");
  }
  
  Serial.println("Setup concluído. Iniciando loop principal...");
}

void loop() {
  unsigned long currentTime = millis();
  
  // Processar WiFiManager se não estiver conectado
  if (WiFi.status() != WL_CONNECTED) {
    wm.process();
    wifiRetryCount++;
    
    // Reiniciar após muitas tentativas falhadas
    if (wifiRetryCount > WIFI_RETRY_COUNT * 100) { // Multiplicar por 100 pois loop é rápido
      if (DEBUG_ENABLED) Serial.println("Muitas falhas de WiFi. Reiniciando...");
      ESP.restart();
    }
    return;
  } else {
    wifiRetryCount = 0; // Reset contador se conectado
  }
  
  // Atualizar dados da API periodicamente
  if (currentTime - lastApiCall >= UPDATE_INTERVAL) {
    if (DEBUG_ENABLED) Serial.println("Atualizando dados do Protheus...");
    bool success = fetchProtheusData();
    
    if (success && DEBUG_ENABLED) {
      Serial.println("Dados atualizados com sucesso!");
    } else if (DEBUG_ENABLED) {
      Serial.println("Falha ao atualizar dados");
    }
    
    lastApiCall = currentTime;
  }
  
  // Verificar alertas
  checkAlerts();
  
  // Atualizar display periodicamente
  if (currentTime - lastDisplayUpdate >= DISPLAY_CYCLE_TIME) {
    switch (currentDisplayPage) {
      case 0:
        displayLicensesPage();
        break;
      case 1:
        displayResourcesPage();
        break;
      case 2:
        displaySystemInfoPage();
        break;
    }
    
    currentDisplayPage = (currentDisplayPage + 1) % 3; // Alternar entre 3 páginas
    lastDisplayUpdate = currentTime;
  }
  
  // Verificar se botão foi pressionado para reset WiFi
  if (digitalRead(PIN_AP) == BUTTON_PRESSED) {
    if (DEBUG_ENABLED) Serial.println("Botão de reset pressionado");
    delay(BUTTON_DEBOUNCE_TIME); // Debounce
    
    unsigned long pressStart = millis();
    while (digitalRead(PIN_AP) == BUTTON_PRESSED && (millis() - pressStart) < BUTTON_RESET_TIME) {
      delay(10);
    }
    
    if ((millis() - pressStart) >= BUTTON_RESET_TIME) {
      // Reset WiFi após tempo definido pressionado
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0, 0);
      centerText(display, MSG_WIFI_RESET, 20);
      centerText(display, "Reiniciando...", 35);
      display.display();
      
      if (DEBUG_ENABLED) Serial.println("Resetando configurações WiFi...");
      wm.resetSettings();
      delay(1000);
      ESP.restart();
    }
  }
  
  delay(100); // Pequeno delay para não sobrecarregar o processador
}
