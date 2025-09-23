# 📟 ESP32 Protheus Monitor - Versão Melhorada

Um projeto completo usando ESP32 com display OLED para monitorar, em tempo real, **licenças utilizadas**, **usuários conectados** e **consumo de recursos** do ERP **TOTVS Protheus**.

> **Versão:** 1.0.0 - Melhorada  
> **Plataforma:** ESP32-S3  
> **Ambiente:** PlatformIO  
> **Display:** SSD1306 OLED 0.96" (128x64)

## 🆕 **Melhorias Implementadas**

### ✅ **Correções de Bugs**
- ✅ Corrigida inconsistência na definição de pinos (`PIN_AP`)
- ✅ Removido uso incorreto do buzzer (sempre ativo)
- ✅ Substituídos delays bloqueantes por timers não-bloqueantes
- ✅ Implementado tratamento de erros robusto
- ✅ Corrigida estrutura de pastas duplicada

### 🚀 **Novas Funcionalidades**
- ✅ **Sistema de Alertas Inteligente**: Alertas baseados em thresholds configuráveis
- ✅ **Múltiplas Páginas no Display**: 3 telas que alternam automaticamente
- ✅ **Dados Simulados**: Para testes quando API real não está disponível
- ✅ **Configuração Centralizada**: Arquivo `config.h` para fácil customização
- ✅ **Funções Utilitárias**: Biblioteca de funções auxiliares
- ✅ **Logs Detalhados**: Sistema de debug configurável
- ✅ **Indicadores de Status**: Status de conectividade e qualidade do sinal
- ✅ **Reset WiFi Seguro**: Botão com timeout para evitar resets acidentais

### 📊 **Páginas do Display**

#### Página 1 - Monitor de Licenças
```
PROTHEUS MONITOR
================
Licencas: 12/20
Uso: 60.0%
Usuarios: 8
Status: Online
[████████░░] 60%
```

#### Página 2 - Recursos do Sistema
```
RECURSOS SISTEMA
================
CPU: 45.5%
Memoria: 67.2%
WiFi: -45 dBm (Bom)
Atualizacao: 15s
CPU:[███████░░░]
MEM:[████████░░]
```

#### Página 3 - Informações do Sistema
```
INFO SISTEMA
================
Uptime: 02:35:42
RAM Livre: 234 KB
CPU: 240 MHz
Firmware: v1.0.0
```

## 🔧 **Configuração Fácil**

Todas as configurações estão centralizadas no arquivo `src/config.h`:

```cpp
// Configurações de Rede
#define WIFI_AP_NAME "Protheus Monitor WIFI"
#define WIFI_AP_PASSWORD "12345678"

// Configurações da API
#define PROTHEUS_API_URL "http://192.168.1.100:8080/rest/api/monitor"

// Intervalos de Atualização
#define UPDATE_INTERVAL 30000        // 30 segundos
#define DISPLAY_CYCLE_TIME 5000      // 5 segundos por tela

// Thresholds de Alerta
#define LICENSE_ALERT_THRESHOLD 80   // 80% das licenças
#define CPU_ALERT_THRESHOLD 85       // 85% de CPU
#define MEMORY_ALERT_THRESHOLD 90    // 90% de memória
```

## 📡 **API do Protheus**

O sistema espera uma API REST que retorne dados no formato JSON:

```json
{
  "totalLicenses": 20,
  "usedLicenses": 12,
  "activeUsers": 8,
  "cpuUsage": 45.5,
  "memoryUsage": 67.2
}
```

### Endpoints Esperados:
- `GET /rest/api/monitor` - Dados principais do sistema

## 🛠️ **Hardware Requirements**

- **ESP32-S3 Dev Board**
- **Display OLED SSD1306 0.96" I2C** (SDA: GPIO8, SCL: GPIO9)
- **Buzzer** (GPIO20) - Para alertas
- **Botão Push** (GPIO42) - Para reset WiFi
- **Cabo USB** - Para alimentação/programação

### 📌 **Pinout**
```
ESP32-S3 ←→ Componente
GPIO 8   ←→ SDA (Display)
GPIO 9   ←→ SCL (Display)
GPIO 20  ←→ Buzzer (+)
GPIO 42  ←→ Botão Reset
3.3V     ←→ VCC (Display)
GND      ←→ GND (Display, Buzzer, Botão)
```

## 💻 **Software Requirements**

- **PlatformIO** (recomendado) ou Arduino IDE
- **Bibliotecas** (instaladas automaticamente):
  - `WiFiManager@^2.0.17`
  - `Adafruit GFX Library@^1.12.1`
  - `Adafruit SSD1306@^2.5.14`
  - `ArduinoJson@^7.0.4`

## 🚀 **Instalação e Uso**

### 1. Clone o Repositório
```bash
git clone https://github.com/antunesls/esp32-protheus-monitor.git
cd esp32-protheus-monitor
```

### 2. Configure a API
Edite o arquivo `src/config.h` e ajuste:
```cpp
#define PROTHEUS_API_URL "http://SEU-SERVIDOR:PORTA/rest/api/monitor"
```

### 3. Compile e Upload
```bash
# Via PlatformIO CLI
pio run --target upload

# Ou use a interface do VS Code com PlatformIO
```

### 4. Configuração WiFi
1. Primeira inicialização: O ESP32 criará um AP chamado "Protheus Monitor WIFI"
2. Conecte-se com a senha "12345678"
3. Acesse `192.168.4.1` no navegador
4. Configure sua rede WiFi

### 5. Reset WiFi (se necessário)
- Mantenha o botão pressionado por 3 segundos
- O sistema reiniciará e entrará em modo configuração

## 🎛️ **Sistema de Alertas**

O sistema ativa alertas (buzzer + indicação visual) quando:

- ✅ **Licenças**: Uso ≥ 80%
- ✅ **CPU**: Uso ≥ 85%
- ✅ **Memória**: Uso ≥ 90%
- ✅ **WiFi**: Sinal ≤ -70 dBm

**Duração do Alerta**: 200ms (configurável)

## 🔍 **Debug e Monitoramento**

### Monitor Serial (115200 baud):
```
=================================
ESP32 Protheus Monitor v1.0.0
Iniciando sistema...
=================================
WiFi Conectado!
IP: 192.168.1.150
RSSI: -45 dBm
Setup concluído. Iniciando loop principal...
Atualizando dados do Protheus...
Dados atualizados com sucesso!
```

### Logs de Debug
Habilite no `config.h`:
```cpp
#define DEBUG_ENABLED true
```

## 📁 **Estrutura do Projeto**

```
esp32_protheus_monitor/
├── platformio.ini          # Configuração PlatformIO
├── src/
│   ├── main.cpp            # Código principal
│   ├── config.h            # Configurações do sistema
│   ├── utils.h             # Funções utilitárias
│   └── logo.h              # Logo do projeto
├── include/                # Headers (vazio)
├── lib/                    # Bibliotecas locais (vazio)
└── test/                   # Testes (vazio)
```

## 🆘 **Solução de Problemas**

### Display não funciona
- Verifique conexões I2C (SDA/SCL)
- Confirme endereço I2C (padrão: 0x3C)

### WiFi não conecta
- Use botão reset (3 segundos)
- Verifique credenciais da rede

### API não responde
- Verifique URL no `config.h`
- Sistema usará dados simulados se habilitado

### Sem dados no display
- Verifique logs no Serial Monitor
- Confirme que API retorna JSON válido

## 🔄 **Próximas Melhorias**

- [ ] Interface web para configuração
- [ ] Suporte a HTTPS
- [ ] Histórico de dados
- [ ] Notificações push
- [ ] Modo sleep para economia de energia
- [ ] Backup de configurações
- [ ] Suporte a múltiplos servidores Protheus

## 📝 **Changelog**

### v1.0.0 (23/09/2025)
- ✅ Arquitetura completamente reescrita
- ✅ Sistema de configuração centralizada
- ✅ Múltiplas páginas no display
- ✅ Sistema de alertas inteligente
- ✅ Tratamento robusto de erros
- ✅ Funções utilitárias
- ✅ Logs de debug detalhados
- ✅ Dados simulados para testes

## 👨‍💻 **Autor**

**Lucas Souza**  
📧 Email: [seu-email@exemplo.com]  
🔗 GitHub: [@antunesls](https://github.com/antunesls)

## 📄 **Licença**

Este projeto está licenciado sob a [MIT License](LICENSE).

## 🙏 **Contribuições**

Contribuições são bem-vindas! Por favor:

1. Fork o projeto
2. Crie uma branch para sua feature
3. Commit suas mudanças
4. Push para a branch
5. Abra um Pull Request

---

**⭐ Se este projeto foi útil para você, considere dar uma estrela no GitHub!**