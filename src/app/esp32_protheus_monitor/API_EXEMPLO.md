# Exemplo de API para Protheus Monitor

Este arquivo contém exemplos de como implementar uma API REST simples para fornecer dados do Protheus para o ESP32 Monitor.

## Endpoint Principal

**URL:** `GET /rest/api/monitor`

**Resposta JSON esperada:**
```json
{
  "totalLicenses": 20,
  "usedLicenses": 12,
  "activeUsers": 8,
  "cpuUsage": 45.5,
  "memoryUsage": 67.2,
  "timestamp": "2025-09-23T10:30:00Z",
  "serverName": "PROTHEUS-SRV-01",
  "version": "12.1.33"
}
```

## Exemplo de Implementação (Node.js/Express)

```javascript
const express = require('express');
const app = express();
const port = 8080;

// Middleware para CORS (se necessário)
app.use((req, res, next) => {
  res.header('Access-Control-Allow-Origin', '*');
  res.header('Access-Control-Allow-Headers', 'Content-Type');
  next();
});

// Endpoint principal
app.get('/rest/api/monitor', (req, res) => {
  // Aqui você integraria com o Protheus para obter dados reais
  // Por exemplo, consultando banco de dados ou chamando APIs do Protheus
  
  const data = {
    totalLicenses: getTotalLicenses(),
    usedLicenses: getUsedLicenses(),
    activeUsers: getActiveUsers(),
    cpuUsage: getCpuUsage(),
    memoryUsage: getMemoryUsage(),
    timestamp: new Date().toISOString(),
    serverName: process.env.SERVER_NAME || "PROTHEUS-SRV-01",
    version: "12.1.33"
  };
  
  res.json(data);
});

// Funções para obter dados do Protheus
// (Implementar conforme sua infraestrutura)

function getTotalLicenses() {
  // Consulta ao banco ou arquivo de configuração
  return 20;
}

function getUsedLicenses() {
  // Consulta tabela de sessões ativas ou logs
  // Exemplo SQL: SELECT COUNT(*) FROM SYS_USERS WHERE STATUS='ACTIVE'
  return Math.floor(Math.random() * 20);
}

function getActiveUsers() {
  // Consulta usuários ativos no momento
  return Math.floor(Math.random() * 15);
}

function getCpuUsage() {
  // Monitoria do sistema operacional
  const os = require('os');
  return (100 - (os.freemem() / os.totalmem()) * 100).toFixed(1);
}

function getMemoryUsage() {
  // Uso de memória do servidor
  const os = require('os');
  return ((1 - (os.freemem() / os.totalmem())) * 100).toFixed(1);
}

app.listen(port, () => {
  console.log(`API Protheus Monitor rodando na porta ${port}`);
});
```

## Exemplo de Implementação (Python/Flask)

```python
from flask import Flask, jsonify
import psutil
import random
from datetime import datetime

app = Flask(__name__)

@app.route('/rest/api/monitor', methods=['GET'])
def get_monitor_data():
    """Endpoint principal para dados do monitor"""
    
    data = {
        'totalLicenses': get_total_licenses(),
        'usedLicenses': get_used_licenses(),
        'activeUsers': get_active_users(),
        'cpuUsage': get_cpu_usage(),
        'memoryUsage': get_memory_usage(),
        'timestamp': datetime.now().isoformat(),
        'serverName': 'PROTHEUS-SRV-01',
        'version': '12.1.33'
    }
    
    return jsonify(data)

def get_total_licenses():
    """Retorna total de licenças configuradas"""
    # Implementar consulta ao Protheus ou arquivo config
    return 20

def get_used_licenses():
    """Retorna licenças atualmente em uso"""
    # Consulta ao banco do Protheus
    # Exemplo: SELECT COUNT(*) FROM SYS_LICENSES WHERE ACTIVE=1
    return random.randint(8, 18)

def get_active_users():
    """Retorna usuários ativos"""
    # Consulta à tabela de usuários ativos
    return random.randint(5, 12)

def get_cpu_usage():
    """Retorna uso da CPU do servidor"""
    return round(psutil.cpu_percent(interval=1), 1)

def get_memory_usage():
    """Retorna uso de memória do servidor"""
    memory = psutil.virtual_memory()
    return round(memory.percent, 1)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8080, debug=False)
```

## Integração com Banco Protheus

### Consultas SQL de Exemplo

```sql
-- Total de licenças configuradas
SELECT COUNT(*) as total_licenses 
FROM SYS_COMPANY 
WHERE ACTIVE = '1';

-- Licenças em uso no momento
SELECT COUNT(DISTINCT USER_ID) as used_licenses
FROM SYS_SESSIONS 
WHERE SESSION_STATUS = 'ACTIVE' 
AND LOGOUT_TIME IS NULL;

-- Usuários únicos ativos
SELECT COUNT(DISTINCT USERNAME) as active_users
FROM SYS_AUDIT_LOG 
WHERE EVENT_DATE >= DATE(NOW()) 
AND EVENT_TYPE = 'LOGIN';

-- Sessões ativas por módulo
SELECT MODULE_NAME, COUNT(*) as sessions
FROM SYS_SESSIONS s
JOIN SYS_MODULES m ON s.MODULE_ID = m.MODULE_ID
WHERE s.SESSION_STATUS = 'ACTIVE'
GROUP BY MODULE_NAME;
```

## Configuração do Protheus

### appserver.ini - Habilitar logs necessários
```ini
[GENERAL]
EnableAuditLog=1
LogConnections=1
LogLevel=3

[REST]
EnableREST=1
RestPort=8080
RestAccessLog=1
```

### Criação de usuário para API
```advpl
// Criar usuário específico para monitoramento
User Function CreateMonitorUser()
    Local cUser := "MONITOR_API"
    Local cPass := "monitor123"
    
    // Criar usuário com permissões limitadas
    PswOrder(1)
    If !PswSeek(cUser)
        PswSave(cUser, cPass, "Monitor API", "", .F., .F.)
    EndIf
Return
```

## Segurança

### Autenticação Básica
```javascript
// Express.js com autenticação básica
const auth = require('basic-auth');

app.use('/rest/api/monitor', (req, res, next) => {
  const credentials = auth(req);
  
  if (!credentials || 
      credentials.name !== 'monitor' || 
      credentials.pass !== 'senha123') {
    res.status(401).json({error: 'Acesso negado'});
    return;
  }
  
  next();
});
```

### Token JWT
```javascript
const jwt = require('jsonwebtoken');

app.use('/rest/api/monitor', (req, res, next) => {
  const token = req.headers['authorization'];
  
  if (!token) {
    return res.status(401).json({error: 'Token necessário'});
  }
  
  try {
    const decoded = jwt.verify(token, process.env.JWT_SECRET);
    req.user = decoded;
    next();
  } catch (err) {
    return res.status(403).json({error: 'Token inválido'});
  }
});
```

## Monitoramento da API

### Health Check
```javascript
app.get('/rest/api/health', (req, res) => {
  res.json({
    status: 'ok',
    timestamp: new Date().toISOString(),
    uptime: process.uptime(),
    version: '1.0.0'
  });
});
```

### Logs
```javascript
const winston = require('winston');

const logger = winston.createLogger({
  level: 'info',
  format: winston.format.json(),
  transports: [
    new winston.transports.File({ filename: 'api-monitor.log' })
  ]
});

app.use('/rest/api/monitor', (req, res, next) => {
  logger.info(`Monitor API accessed from ${req.ip}`);
  next();
});
```

## Docker (Opcional)

### Dockerfile
```dockerfile
FROM node:16-alpine

WORKDIR /app
COPY package*.json ./
RUN npm install

COPY . .

EXPOSE 8080
CMD ["node", "server.js"]
```

### docker-compose.yml
```yaml
version: '3.8'
services:
  protheus-monitor-api:
    build: .
    ports:
      - "8080:8080"
    environment:
      - NODE_ENV=production
      - SERVER_NAME=PROTHEUS-SRV-01
    volumes:
      - ./logs:/app/logs
    restart: unless-stopped
```

## Testes

### Teste com curl
```bash
# Teste básico
curl http://localhost:8080/rest/api/monitor

# Com autenticação
curl -u monitor:senha123 http://localhost:8080/rest/api/monitor

# Teste de health check
curl http://localhost:8080/rest/api/health
```

### Resposta esperada
```json
{
  "totalLicenses": 20,
  "usedLicenses": 12,
  "activeUsers": 8,
  "cpuUsage": 45.5,
  "memoryUsage": 67.2,
  "timestamp": "2025-09-23T10:30:00.000Z",
  "serverName": "PROTHEUS-SRV-01",
  "version": "12.1.33"
}
```