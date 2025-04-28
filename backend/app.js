const express = require('express');
const cors = require('cors');
const app = express();
const http = require('http').createServer(app);
const WebSocket = require('ws');
const db = require('./models/db');
const rotas = require('./routes/rfidRoutes'); // ajuste o caminho se estiver diferente

const wss = new WebSocket.Server({ server: http });

app.use(cors());
app.use(express.json());
app.use(rotas);

app.use('/api', require('./routes/rfidRoutes'));

wss.on('connection', (ws) => {
  console.log("Dispositivo conectado via WebSocket");
  ws.on('message', (message) => {
    console.log("Mensagem recebida:", message.toString());
    // Aqui você pode tratar os dados do ESP32
  });
});



const PORT = 3000;
http.listen(PORT, () => {
  console.log(`Servidor rodando na porta ${PORT}`);
});
