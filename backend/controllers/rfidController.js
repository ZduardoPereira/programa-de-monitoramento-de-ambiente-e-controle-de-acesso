// controllers/rfidController.js
const db = require('../models/db');

exports.cadastrarUsuario = (req, res) => {
  const { uid, nome, email } = req.body;
  const sql = 'INSERT INTO usuarios (uid, nome, email) VALUES (?, ?, ?)';
  db.query(sql, [uid, nome, email], (err, result) => {
    if (err) return res.status(500).json({ error: err });
    res.json({ success: true, message: "Usuário cadastrado!", user: { uid, nome, email } });
  });
};

exports.registrarAcesso = (req, res) => {
  const { uid, tipo } = req.body;
  const sql = 'INSERT INTO acessos (uid, tipo) VALUES (?, ?)';
  db.query(sql, [uid, tipo], (err, result) => {
    if (err) return res.status(500).json({ error: err });
    res.json({ success: true, message: "Acesso registrado!" });
  });
};

exports.verificarUsuario = (req, res) => {
  const uid = req.params.uid;
  const sql = 'SELECT * FROM usuarios WHERE uid = ?';
  db.query(sql, [uid], (err, results) => {
    if (err) return res.status(500).json({ error: err });
    if (results.length > 0) {
      return res.json({ user: results[0] });
    } else {
      return res.json({ user: null });
    }
  });
};
  