const { Pool } = require('pg')

const pool = new Pool({
    user: 'postgres',
    database: 'icon',
    password: 'ICON2023',
    port: 5432,
    host: 'localhost',
})

module.exports = { pool }