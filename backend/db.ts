import { Pool } from "pg"

const pool = new Pool({
    user: 'postgres',
    database: 'icon',
    password: 'ICON2023',
    port: 5432,
    host: '192.168.0.123',
})

module.exports = { pool }