const mysql = require('mysql2');

const db = mysql.createConnection({
  host: 'localhost',
  user: 'root',
  password: 'fatec',
  database: 'monitoramento'
});

db.connect(err => {
  if (err) throw err;
  console.log('Conectado ao MySQL');
});

module.exports = db;
