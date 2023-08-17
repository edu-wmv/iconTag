/*
Banco de dados: iconTag
Tabela: iconicos (id, name, uid, pontos, hours)
🚧 IMPLEMENTAR FUNÇÃO DE MULTIPLAS UID'S 🚧 
*/

import { Request, Response } from 'express'
import { Query, QueryOptions } from 'mysql'
const { pool } = require('./pool')
import { v4 as uuidv4 } from 'uuid'

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

// INSERIR PONTO DE ENTRADA OU SAIDA
const setPoint = (req: Request, res: Response) => {
    const uuid = req.headers['uuid']
    const data_time = req.headers['time']

    pool.query(
        `SELECT * FROM iconicos 
         WHERE uid = '${uuid}'`,
        (error: any, results: any) => {
            if (error) throw error
            if (results.length > 0 && typeof results !== undefined) {
                // USUARIO EXISTE
                const userId = results[0].id
                const userName = results[0].name

                pool.query(
                    `SELECT * FROM pontos
                     WHERE userId = ${userId}
                     ORDER BY data DESC
                     LIMIT 1`,
                    (error: any, results: any) => {
                        if (error) throw error
                        
                        // BOOLEANO PARA VERIFICAR SE O USUARIO ESTA ENTRANDO OU SAINDO
                        const isEntrada = results.length > 0 && typeof results !== undefined ? results[0].entrada : false;

                        // INSERE NOVO PONTO
                        pool.query(
                            `INSERT INTO pontos (uid, userId, uuid, name, data, entrada)
                             VALUES ('${uuidv4()}', ${userId}, '${uuid}', '${userName}', '${data_time}', ${!isEntrada})`,
                            (error: any, results: any) => {
                                if (error) throw error
                                res.status(200).json(`Ponto de ${!isEntrada ? 'entrada' : 'saida'} inserido com sucesso`)
                            }
                        )
                        
                    }
                )
            } else {
                // USUARIO NAO EXISTE
                res.status(200).json(`Iconico não encontrado`)
            }
        }
    )
}

module.exports = { 
    insertData,
    getIconicoByName,
    setPoint 
}