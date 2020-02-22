from flask import Flask, render_template, request
import sqlite3 as sql
import json

app = Flask(__name__)


@app.route('/')
def hello():
    return render_template("index.html")

@app.route('/data/', methods=['GET'])
def db():
    if request.method == 'GET':
        conn = sql.connect("testData.db")
        conn.row_factory = sql.Row 

        cur = conn.cursor()
        cur.execute("SELECT * FROM people")    
        
        rows = cur.fetchall()
        data = []
        for row in rows:
            data.append(dict(row))
        
        conn.close()

    return render_template("index.html", data=data)

@app.route('/input/', methods=['GET','POST'])
def input():
    first = request.form['first']
    last = request.form['last']
    age = request.form['age']
    major = request.form['major']

    conn = sql.connect("testData.db")
    conn.row_factory = sql.Row 

    cur = conn.cursor()
    cur.execute("INSERT INTO people (first, last, age, major) VALUES (?,?,?,?)",(first, last, age, major))    
    conn.commit()

    conn.close()

    return render_template("index.html", )

if __name__ == '__main__':
    app.run(debug=True)