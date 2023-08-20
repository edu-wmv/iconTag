const mysql = require('mysql');

const pool = mysql.createConnection({
    host: '192.168.1.32',
    user: 'db_icontag',
    password: 'icontag',
    database: 'db_icontag'
})

module.exports = { pool }