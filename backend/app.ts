// load the env variables and set silent to true to suppress the warning
require('dotenv').config({ silent: true })
import { Request, Response, NextFunction } from 'express'

const express = require("express")
const app = express()
const logger = require('morgan')
const db = require('./query')

app.use(logger('dev'))
app.use(express.static('public'))
app.use((req: Request, res: Response, next: NextFunction) => {
    res.header("Access-Control-Allow-Origin", "*")
    res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept, API-Key")
    next()
})

app.use((req: Request, res: Response, next: NextFunction) => {
    const api_key = req.headers['api-key']
    if (!api_key || api_key != process.env.API_KEY) {
        res.status(401).send('Unauthorized')
    } else {
        next()
    }
})

app.get('/', (req: Request, res: Response) => {
    res.status(200).send('Node.js + Express + PostgreSQL API')
})
app.get('/test', (req: Request, res: Response) => {
    res.status(200).send('Authorized');
})

app.get("/insertData", db.insertData)
app.get("/getData", db.getData)
//app.get("/getName", db.getDataByName)

app.listen(8080, () => {
    console.log("⚡️[server]: Server is running on port 3000")
})