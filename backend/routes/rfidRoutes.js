// routes/rfidRoutes.js
const express = require('express');
const router = express.Router();
const controller = require('../controllers/rfidController');

// Rota para cadastrar um novo usuário (cartão)
router.post('/cadastrar-cartao', controller.cadastrarUsuario);

// Rota para registrar entrada/saída
router.post('/registrar-acesso', controller.registrarAcesso);

// Rota para verificar se o UID já está cadastrado
router.get('/verificar-usuario/:uid', controller.verificarUsuario);

module.exports = router;
