/**
 * @file config.h
 * @brief Arquivo de configuração para o ESP32 Protheus Monitor
 * 
 * @author Lucas Souza
 * @date 23/09/2025
 * 
 * @details
 * Este arquivo contém todas as configurações do sistema que podem ser
 * facilmente modificadas pelo usuário.
 */

#ifndef CONFIG_H
#define CONFIG_H

// =====================================================================
// CONFIGURAÇÕES DE REDE
// =====================================================================

// Nome e senha do Access Point para configuração
#define WIFI_AP_NAME "Protheus Monitor WIFI"
#define WIFI_AP_PASSWORD "12345678"

// Timeout para portal de configuração (segundos)
#define WIFI_CONFIG_TIMEOUT 180

// =====================================================================
// CONFIGURAÇÕES DA API PROTHEUS
// =====================================================================

// URL base da API REST do Protheus
// Exemplo: "http://192.168.1.100:8080/rest/api/monitor"
#define PROTHEUS_API_URL "http://your-protheus-server:8080/rest/api/monitor"

// Credenciais para autenticação (se necessário)
#define PROTHEUS_API_USER "admin"
#define PROTHEUS_API_PASSWORD "password"

// Timeout para requisições HTTP (milissegundos)
#define HTTP_TIMEOUT 10000

// =====================================================================
// CONFIGURAÇÕES DE TEMPORIZAÇÃO
// =====================================================================

// Intervalo entre atualizações da API (milissegundos)
#define UPDATE_INTERVAL 30000        // 30 segundos

// Tempo de exibição de cada página no display (milissegundos)
#define DISPLAY_CYCLE_TIME 5000      // 5 segundos

// Duração do buzzer para alertas (milissegundos)
#define BUZZER_DURATION 200          // 200ms

// Tempo para mostrar o logo na inicialização (milissegundos)
#define LOGO_DISPLAY_TIME 3000       // 3 segundos

// =====================================================================
// CONFIGURAÇÕES DE ALERTAS
// =====================================================================

// Threshold de uso de licenças para alerta (%)
#define LICENSE_ALERT_THRESHOLD 80

// Threshold de uso de CPU para alerta (%)
#define CPU_ALERT_THRESHOLD 85

// Threshold de uso de memória para alerta (%)
#define MEMORY_ALERT_THRESHOLD 90

// Threshold de sinal WiFi fraco (dBm)
#define WIFI_WEAK_SIGNAL_THRESHOLD -70

// =====================================================================
// CONFIGURAÇÕES DO HARDWARE
// =====================================================================

// Pinos I2C para o display
#define I2C_SDA_PIN 8
#define I2C_SCL_PIN 9

// Pino do botão para reset WiFi
#define BUTTON_RESET_PIN 42

// Pino do buzzer
#define BUZZER_PIN 20

// Configurações do display OLED
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_ADDRESS 0x3C
#define OLED_RESET_PIN -1

// =====================================================================
// CONFIGURAÇÕES DO SISTEMA
// =====================================================================

// Baudrate para comunicação serial
#define SERIAL_BAUDRATE 115200

// Tempo mínimo para considerar um pressionamento de botão (ms)
#define BUTTON_DEBOUNCE_TIME 100

// Tempo para reset WiFi (botão pressionado por X segundos)
#define BUTTON_RESET_TIME 3000

// Número de tentativas de reconexão WiFi antes de reiniciar
#define WIFI_RETRY_COUNT 5

// =====================================================================
// CONFIGURAÇÕES DE DEBUG
// =====================================================================

// Habilitar logs de debug no Serial Monitor
#define DEBUG_ENABLED true

// Habilitar simulação de dados quando API não está disponível
#define ENABLE_DATA_SIMULATION true

// =====================================================================
// MENSAGENS PERSONALIZÁVEIS
// =====================================================================

#define MSG_WIFI_CONNECTING "Conectando WiFi..."
#define MSG_WIFI_CONNECTED "WiFi Conectado!"
#define MSG_WIFI_FAILED "Falha na conexao"
#define MSG_WIFI_RESET "Resetando WiFi..."
#define MSG_API_ERROR "Erro na API"
#define MSG_SYSTEM_OFFLINE "Sistema Offline"
#define MSG_SYSTEM_ONLINE "Sistema Online"

// =====================================================================
// CONFIGURAÇÕES DE DADOS SIMULADOS (para testes)
// =====================================================================

#define SIM_TOTAL_LICENSES 20
#define SIM_MIN_USED_LICENSES 8
#define SIM_MAX_USED_LICENSES 18
#define SIM_MIN_USERS 3
#define SIM_MAX_USERS 12
#define SIM_MIN_CPU 20.0
#define SIM_MAX_CPU 95.0
#define SIM_MIN_MEMORY 30.0
#define SIM_MAX_MEMORY 85.0

#endif // CONFIG_H