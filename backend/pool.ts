const mysql = require('mysql');

const pool = mysql.createConnection({
    host: '192.168.0.13',
    user: 'tagend',
    password: 'tagendpass',
    database: 'icon_icontag'
})

module.exports = { pool }