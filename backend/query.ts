import { Request, Response } from 'express'
import { QueryResult } from 'pg'
const { pool } = require('./db')

const insertData = (req: Request, res: Response) => {
    const name = req.headers['name']
    const uid = req.headers['uid']

    pool.query(
        `INSERT INTO iconicos (name, uid)
         values ('${name}', ARRAY ['${uid}'])`, 
         (error: Error, results: QueryResult) => {
            if (error) {
                throw error
            }
            res.status(200).json('Usuario adicionado!')
        })
}

const insertUID = (req: Request, res: Response) =>  {
    const id = req.headers['id']
    const uid = req.headers['uid']

    pool.query(
        `UPDATE iconicos
         SET uid = array_append(uid, '${uid}')
         WHERE id = ${id}
         RETURNING *`,
        (error: Error, results: QueryResult) => {
            if (error) {
                throw error
            }
            res.status(200).json(`Nova UID adicionada!`)
        }
    )
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

const setPoint = (req: Request, res: Response) => {
    const uuid = req.headers['uuid']
    const data_time = req.headers['time']

    pool.query(
        `SELECT * FROM iconicos 
         WHERE '${uuid}' = ANY (uid)`,
         (error: Error, results: QueryResult) => {
            if (error) {
                throw error
            }
            if (results.rows.length > 0) {
                // ENCONTROU USUÁRIO
                const userId = results.rows[0].id;
                const userName = results.rows[0].name;

                pool.query(
                    `SELECT * FROM pontos 
                     WHERE userid = ${userId} 
                     ORDER BY data DESC 
                     LIMIT 1`,
                    (error: Error, results: QueryResult) => {
                        if (error) {
                            throw error
                        }
                        const isEntrada: boolean = results.rows.length > 0 ? results.rows[0].entrada : false;

                        pool.query(
                            `INSERT INTO pontos (userId, uuid, name, data, entrada)
                             VALUES (${userId}, ${uuid}, '${userName}', '${data_time}', ${!isEntrada})`,
                             (error: Error, results: QueryResult) => {
                                if (error) {
                                    throw error
                                }

                                pool.query(
                                    `SELECT * 
                                     FROM pontos 
                                     WHERE userId = '${userId}'
                                     ORDER BY data DESC
                                     LIMIT 1`,
                                     (error: Error, results: QueryResult) => {
                                        if (error) throw error

                                        const ponto_uid = results.rows[0].uid

                                        pool.query(
                                            `UPDATE iconicos
                                             SET pontos = array_append(pontos, '${ponto_uid}')
                                             WHERE id = ${userId}
                                             RETURNING *`,
                                             (error: Error, results: QueryResult) => {
                                                if (error) throw error


                                             }
                                        )
                                     }
                                )

                                if(isEntrada === false){
                                    res.status(200).json(`Bem vindo ao ICON ${userName}`)
                                } else {
                                    res.status(200).json(`Ja vai tarde.`)
                                    //pool.query(
                                    //    `SELECT * 
                                    //     FROM pontos
                                    //     WHERE userid = '${userId}'
                                    //     ORDER BY data DESC
                                    //     LIMIT 2`,
                                    //    (error: Error, results: QueryResult) => {
                                    //        if (error) throw error
                                    //        
                                    //        const horaEntrada = results.rows[1].data
                                    //        const horaSaida = results.rows[0].data
//
                                    //        const diff = horaSaida - horaEntrada
                                    //        console.log(`Entrada: ${horaEntrada} ///// SAIDA: ${horaSaida}`)
                                    //        console.log(`time: ${diff}`)
                                    //    }
                                    //)
                                }
                             }
                        )
                    }
                    )
                } else {
                    res.status(200).json('Usuario não encontrado.')
                }
        }
    )  
}

module.exports = {
    insertData,
    getData,
    getDataByName,
    insertUID,
    setPoint
}