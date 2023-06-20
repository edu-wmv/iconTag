import { Request, Response } from 'express'
import { QueryResult } from 'pg'
const { pool } = require('./db')

const insertData = (req: Request, res: Response) => {
    const name = req.headers['name']
    const uid = req.headers['uid']

    pool.query(
        `INSERT INTO iconicos (name, uid)
         values (${name}, ARRAY ['${uid}'])`, 
         (error: Error, results: QueryResult) => {
            if (error) {
                throw error
            }
            res.status(200).json('Usuario adicionado!')
        })
}

const getData = (req: Request, res: Response) => {
    pool.query(
        "SELECT * FROM iconicos ORDER BY id ASC",
        (error: Error, results: QueryResult) => {
            if (error) {
                throw error
            }
            res.status(200).json(results.rows)
        }
    )
}

const getDataByName = (req: Request, res: Response) => {
    const name = req.params.name

    pool.query(
        "SELECT * FROM iconicos WHERE name = ($1)",
        [name], (error: Error, results: QueryResult) => {
            if (error) {
                throw error
            }
            res.status(200).json(results.rows)
        }
    )
}

module.exports = {
    insertData,
    getData,
    getDataByName,
}