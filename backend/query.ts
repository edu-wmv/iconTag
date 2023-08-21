/*
Banco de dados: iconTag
Tabela: iconicos (id, name, uid, pontos, hours)
🚧 IMPLEMENTAR FUNÇÃO DE MULTIPLAS UID'S 🚧 
*/

import { Request, Response } from 'express'
import { OkPacket, queryCallback } from 'mysql'
const { pool } = require('./pool')
import { v4 as uuidv4 } from 'uuid'

// INSERIR NOVO ICONICO NO BANCO DE DADOS
const insertData = async (req: Request, res: Response) => {
    const name = req.headers['name']
    const uid = req.headers['uid']

    // VERIFICA SE O USUARIO EXISTE
    try {
        const exists = await checkUid(uid) 
        if (exists) {
            res.status(200).json(`Iconico já cadastrado`)
        } else {
            pool.query(
                `INSERT INTO iconicos (name, uid)
                 VALUES ('${name}', ${uid})`,
                 (error: Error, results: OkPacket) => {
                    if (error) throw error
                    res.status(201).json(`Iconico ${name} adicionado com o ID ${results.insertId}`)
                }
            )
        }
    } catch (error) {
        console.log(error)
        res.status(500).json({ error: 'Erro interno ao verificar UID' })
    }
}

// INSERIR PONTO DE ENTRADA OU SAIDA
const setPoint = (req: Request, res: Response) => {
    const uuid = req.headers['uuid']
    const data_time = req.headers['time']

    // VERIFICA SE O USUARIO EXISTE
    pool.query(
        `SELECT * FROM iconicos 
         WHERE uid = ${uuid}`,
        (error: Error, results: any) => {
            if (error) throw error

            // USUARIO EXISTE
            if (results.length > 0 && typeof results !== undefined) {
                const userId = results[0].id
                const userName = results[0].name

                // VERIFICA ULTIMO PONTO
                pool.query(
                    `SELECT * FROM pontos
                     WHERE userId = ${userId}
                     ORDER BY data DESC
                     LIMIT 1`,
                    (error: Error, results: any) => {
                        if (error) throw error
                        
                        // BOOLEANO PARA VERIFICAR SE O USUARIO ESTA ENTRANDO OU SAINDO
                        const last_point: boolean = results.length > 0 && typeof results !== undefined ? results[0].entrada : false

                        // INSERE NOVO PONTO
                        pool.query(
                            `INSERT INTO pontos (id_ponto, userId, uuid, name, data, entrada)
                             VALUES ('${uuidv4()}', ${userId}, ${uuid}, '${userName}', '${data_time}', ${!last_point})`,
                            (error: Error) => {
                                if (error) throw error

                                // ATUALIZA PONTOS DO USUARIO
                                pool.query(
                                    `SELECT * FROM pontos
                                     WHERE userId = ${userId}
                                     ORDER BY data DESC
                                     LIMIT 1`,
                                    (error: Error, results: any) => {
                                        if (error) throw error

                                        // ADICIONA UID DO PONTO NA LISTA DE PONTOS DO USUARIO
                                        const ponto_uid = results[0].id_ponto
                                        pool.query(
                                            `UPDATE iconicos
                                             SET pontos = CONCAT_WS(IFNULL(',', ''), pontos, '${ponto_uid}')
                                             WHERE id = ${userId}`,
                                            (error: Error) => { if (error) throw error }
                                        )
                                    }
                                )
                            }
                        )

                        // ATUALIZA HORAS DO USUARIO
                        if (last_point == false) {
                            res.status(200).json(`Bem vindo ${userName}!`)
                        } else {
                            try {
                                pool.query(
                                    `SELECT * FROM pontos
                                     WHERE userId = ${userId}
                                     ORDER BY data DESC
                                     LIMIT 2`,
                                     (error: Error, results: any) => {
                                        if (error) throw error
                                    
                                        const entrada = new Date(results[1].data)
                                        const saida = new Date(results[0].data)
                                        const diff = (saida.getTime() - entrada.getTime()) / 1000
                                        const time = `${toHoursAndMinutes(diff).h}:${toHoursAndMinutes(diff).m}:${toHoursAndMinutes(diff).s}`
                                    
                                        pool.query(
                                            `UPDATE iconicos
                                             SET hours = ADDTIME(hours, '${time}')
                                             WHERE id = ${userId}`,
                                            (error: Error) => { if (error) throw error }
                                        )

                                        res.status(200).json(`Até logo ${userName}!`)
                                    }
                                )
                            } catch (error) {
                                console.log(error)
                                res.status(500).json(`Erro interno ao atualizar horas do usuario`)
                            }
                        }
                        
                    }
                )

            // USUARIO NAO EXISTE
            } else {
                res.status(200).json(`Iconico não encontrado`)
            }
        }
    )
}


// VERIFICA SE UID JA EXISTE NO BANCO DE DADOS
const checkUid = async (uid: any): Promise<boolean> => {
    try {
        const results = new Promise<queryCallback>((resolve, reject) => {
            pool.query(
                `SELECT * FROM iconicos WHERE uid = ${uid}`,
                (error: Error, results: queryCallback) => {
                    if (error) reject(error)
                    resolve(results)
                }
            )
        })

        return (await results).length > 0 && typeof (await results) !== undefined ? true : false
    } catch (error) {
        console.log(error)
        return true
    }
}

// CONVERTER SEGUNDOS EM HORAS
function toHoursAndMinutes(totalSeconds: number) {
    const totalMinutes = Math.floor(totalSeconds / 60);
  
    const seconds = totalSeconds % 60;
    const hours = Math.floor(totalMinutes / 60);
    const minutes = totalMinutes % 60;
  
    return { h: hours, m: minutes, s: seconds };
}

module.exports = {
    insertData,
    setPoint,
}