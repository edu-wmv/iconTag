require('dotenv').config({ silent: true })
import { Request, Response, NextFunction } from 'express'

const express = require("express")
const app = express()
const logger = require('morgan')
const db = require('./query')
const port = process.env.PORT

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

app.post("/insertData", db.insertData)
app.post("/setPoint", db.setPoint)

app.listen(port, () => { console.log(`⚡️[server]: Server is running on port ${port}`) })