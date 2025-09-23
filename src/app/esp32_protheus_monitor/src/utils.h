/**
 * @file utils.h
 * @brief Funções utilitárias para o ESP32 Protheus Monitor
 * 
 * @author Lucas Souza
 * @date 23/09/2025
 * 
 * @details
 * Este arquivo contém funções auxiliares para formatação de dados,
 * cálculos e outras operações utilitárias.
 */

#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

/**
 * @brief Converte milissegundos para formato legível (dias, horas, minutos, segundos)
 * @param ms Milissegundos para converter
 * @param buffer Buffer para armazenar o resultado
 * @param bufferSize Tamanho do buffer
 */
void formatUptime(unsigned long ms, char* buffer, size_t bufferSize) {
  unsigned long seconds = ms / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  unsigned long days = hours / 24;

  seconds %= 60;
  minutes %= 60;
  hours %= 24;

  if (days > 0) {
    snprintf(buffer, bufferSize, "%lud %02lu:%02lu:%02lu", days, hours, minutes, seconds);
  } else if (hours > 0) {
    snprintf(buffer, bufferSize, "%02lu:%02lu:%02lu", hours, minutes, seconds);
  } else {
    snprintf(buffer, bufferSize, "%02lu:%02lu", minutes, seconds);
  }
}

/**
 * @brief Formata tamanho de memória em formato legível
 * @param bytes Bytes para converter
 * @param buffer Buffer para armazenar o resultado
 * @param bufferSize Tamanho do buffer
 */
void formatMemorySize(unsigned long bytes, char* buffer, size_t bufferSize) {
  const char* units[] = {"B", "KB", "MB", "GB"};
  int unitIndex = 0;
  float size = bytes;

  while (size >= 1024 && unitIndex < 3) {
    size /= 1024;
    unitIndex++;
  }

  if (unitIndex == 0) {
    snprintf(buffer, bufferSize, "%lu %s", (unsigned long)size, units[unitIndex]);
  } else {
    snprintf(buffer, bufferSize, "%.1f %s", size, units[unitIndex]);
  }
}

/**
 * @brief Obtém descrição do nível de sinal WiFi
 * @param rssi Valor RSSI em dBm
 * @return String com descrição do sinal
 */
const char* getWiFiSignalDescription(int rssi) {
  if (rssi > -50) return "Excelente";
  else if (rssi > -60) return "Bom";
  else if (rssi > -70) return "Regular";
  else if (rssi > -80) return "Fraco";
  else return "Muito Fraco";
}

/**
 * @brief Calcula porcentagem de uso
 * @param used Valor usado
 * @param total Valor total
 * @return Porcentagem (0-100)
 */
float calculatePercentage(int used, int total) {
  if (total == 0) return 0.0;
  return ((float)used / (float)total) * 100.0;
}

/**
 * @brief Gera dados simulados para testes
 * @param data Ponteiro para estrutura de dados do Protheus
 */
void generateSimulatedData(struct ProtheusData* data) {
  data->totalLicenses = SIM_TOTAL_LICENSES;
  data->usedLicenses = random(SIM_MIN_USED_LICENSES, SIM_MAX_USED_LICENSES + 1);
  data->activeUsers = random(SIM_MIN_USERS, SIM_MAX_USERS + 1);
  data->cpuUsage = random(SIM_MIN_CPU * 10, SIM_MAX_CPU * 10) / 10.0;
  data->memoryUsage = random(SIM_MIN_MEMORY * 10, SIM_MAX_MEMORY * 10) / 10.0;
  data->isOnline = true;
  data->lastUpdate = millis();
}

/**
 * @brief Desenha uma barra de progresso no display
 * @param display Referência ao objeto display
 * @param x Posição X
 * @param y Posição Y
 * @param width Largura da barra
 * @param height Altura da barra
 * @param percentage Porcentagem para preencher (0-100)
 * @param showBorder Se deve mostrar borda
 */
void drawProgressBar(Adafruit_SSD1306& display, int x, int y, int width, int height, float percentage, bool showBorder = true) {
  if (showBorder) {
    display.drawRect(x, y, width, height, WHITE);
  }
  
  int fillWidth = (percentage / 100.0) * (width - (showBorder ? 2 : 0));
  if (fillWidth > 0) {
    display.fillRect(x + (showBorder ? 1 : 0), y + (showBorder ? 1 : 0), fillWidth, height - (showBorder ? 2 : 0), WHITE);
  }
}

/**
 * @brief Centraliza texto no display
 * @param display Referência ao objeto display
 * @param text Texto para centralizar
 * @param y Posição Y
 * @param textSize Tamanho do texto
 */
void centerText(Adafruit_SSD1306& display, const char* text, int y, int textSize = 1) {
  display.setTextSize(textSize);
  int textWidth = strlen(text) * 6 * textSize; // Aproximação: 6 pixels por caractere
  int x = (OLED_WIDTH - textWidth) / 2;
  display.setCursor(x, y);
  display.print(text);
}

/**
 * @brief Verifica se uma string é um JSON válido (verificação básica)
 * @param str String para verificar
 * @return true se parecer ser JSON válido
 */
bool isValidJson(const String& str) {
  str.trim();
  return (str.startsWith("{") && str.endsWith("}")) || 
         (str.startsWith("[") && str.endsWith("]"));
}

/**
 * @brief Formata timestamp para string legível
 * @param timestamp Timestamp em milissegundos
 * @param buffer Buffer para armazenar resultado
 * @param bufferSize Tamanho do buffer
 */
void formatTimestamp(unsigned long timestamp, char* buffer, size_t bufferSize) {
  unsigned long seconds = timestamp / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  
  seconds %= 60;
  minutes %= 60;
  hours %= 24;
  
  snprintf(buffer, bufferSize, "%02lu:%02lu:%02lu", hours, minutes, seconds);
}

#endif // UTILS_H