/*
Banco de dados: iconTag
Tabela: iconicos (id, name, uid, pontos, hours)
🚧 IMPLEMENTAR FUNÇÃO DE MULTIPLAS UID'S 🚧 
*/

import { Request, Response } from 'express'
import { Query, QueryOptions } from 'mysql'
const { pool } = require('./pool')

// INSERIR NOVO ICONICO NO BANCO DE DADOS
const insertData = (req: Request, res: Response) => {
    const name = req.headers['name']
    const uid = req.headers['uid']

    pool.query(
        `INSERT INTO iconicos (id, name, uid, pontos, hours)
         VALUES (NULL, '${name}', ${uid}, '', NULL)`,
         (error: any, results: any) => {
            if (error) throw error
            res.status(200).json(`Iconico ${name} adicionado com o ID ${results.insertId}`)
         }
    )
}


// PROCURAR ICONICO PELO NOME
const getIconicoByName = (req: Request, res: Response) => {
    const name = req.headers['name']

    pool.query(
        `SELECT * FROM iconicos WHERE name = '${name}'`,
        (error: any, results: any) => {
            if (error) throw error
            results.length > 0 && typeof results !== undefined 
            ? res.status(200).json(results) 
            : res.status(200).json(`Iconico '${name}' não encontrado`)
        }
    )
}

const setPoint = (req: Request, res: Response) => {
    const uuid = req.headers['uuid']
    const time = req.headers['time']

    pool.query(
        `SELECT * FROM iconicos 
         WHERE uid = '${uuid}'`,
        (error: any, results: any) => {
            if (error) throw error
            if (results.length > 0 && typeof results !== undefined) {
                const userId = results[0].id
                const userName = results[0].name

                pool.query(
                    `SELECT * FROM pontos
                     WHERE userId = ${userId}`,
                )
            }
        }
    )
}

module.exports = { 
    insertData,
    getIconicoByName,
    setPoint 
}