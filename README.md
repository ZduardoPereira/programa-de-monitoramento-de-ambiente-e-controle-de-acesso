# 🌡️🛂 ESP32 Controle de acesso e Monitoramento de temperatura

[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)  
[![GitHub issues](https://img.shields.io/github/issues/seu-usuario/seu-repo)](https://github.com/seu-usuario/seu-repo/issues)  
[![GitHub stars](https://img.shields.io/github/stars/seu-usuario/seu-repo)](https://github.com/seu-usuario/seu-repo/stargazers)

Este projeto consiste em uma **API de monitoramento de ambiente** (DHT11 para temperatura e umidade, MQ-9 para gás) e um **sistema de controle de acesso RFID**, ambos rodando em um ESP32.  
- 🔥 Monitoramento em tempo real via WebSocket  
- 🚨 Alertas configuráveis (e-mail/SMS)  
- 🗄️ Logging no SPIFFS e MySQL  

## Demonstração

### Vídeo de Apresentação  
<div align="center">
  <video src="./demo/monitoramento.mp" controls width="600" />
</div>

### Screenshots  
<div align="center">
  <img src="./screenshots/dashboard1.png" alt="Dashboard Temperatura" width="300" />
  <img src="./screenshots/dashboard2.png" alt="Gráfico Umidade" width="300" />
</div>

> **Nota:** coloque seus arquivos em `/demo` e `/screenshots`.

## Funcionalidades

- **Monitoramento** de temperatura e umidade (DHT11) e gás (MQ-9) em tempo real  
- **Alertas** sonoros, visuais e envio de e-mail/SMS ao ultrapassar limites  
- **Controle de acesso** por RFID, com registro de entrada/saída em MySQL  
- **Webhook** para integração com outros serviços  

## Arquitetura / Stack

| Camada          | Tecnologia                            |
|-----------------|---------------------------------------|
| Firmware ESP32  | Arduino-ESP32                         |
| Backend         | WebSocket, API REST (ESPAsyncWebServer) |
| Banco de dados  | MySQL                                 |
| Frontend        | HTML5, Chart.js                       |
| Autenticação    | RFID RC522                            |

## Como Rodar Localmente

1. Clone o repositório  
   ```bash
   git clone https://github.com/seu-usuario/seu-repo.git
   cd seu-repo
