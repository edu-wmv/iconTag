const { pool } = require('./db')

const insertData = (req, res) => {
    const id = parseInt(req.params.id)
    const name = req.params.name
    const uid = req.params.uid

    pool.query(
        "INSERT INTO iconicos (id, name, uid) VALUES ($1, $2, $3)",
        [id, name, uid], (error, results) => {
            if (error) {
                throw error
            }
            res.status(200).json(results.json)
        })
}


const getData = (req, res) => {
    pool.query(
        "SELECT * FROM iconicos ORDER BY id ASC",
        (error, results) => {
            if (error) {
                console.log('a')
            }
            res.status(200).json(results.rows)
        }
    )
}

module.exports = {
    insertData,
    getData
}