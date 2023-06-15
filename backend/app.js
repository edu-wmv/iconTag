var express = require("express")
var app = express();
const db = require('./query')

app.listen(3000, () => {
    console.log("Server running on port 3000")
})

app.get("", (req, res, next) => {
    res.json('response here')
})

app.get("/insert_data", db.insertData)

app.get("/get_data", db.getData)
