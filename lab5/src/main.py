#!/usr/bin/python
import Adafruit_DHT
import datetime
import sqlite3
from sqlite3 import Error


def create_connection(db):
    con = None
    try:
        con = sqlite3.connect(db)
    except Error as e:
        print(e)
    return con


def create_table(con, create_sql):
    try:
        c = con.cursor()
        c.execute(create_sql)
        c.close()
    except Error as e:
        print(e)


def insert2db(con, val):
    sql_insert = "INSERT INTO measurements(temp, hum, date) VALUES(?,?,?)"
    cur = con.cursor()
    cur.execute(sql_insert, val)
    con.commit()
    cur.close()


def return_table(con):
    cur = con.cursor()
    cur.execute("SELECT * from measurements")
    rows = cur.fetchall()
    for row in rows:
        print(row)
    cur.close()


conn = create_connection('pysqlite.db')
sql_create = "CREATE TABLE IF NOT EXISTS measurements (id INTEGER PRIMARY KEY AUTOINCREMENT, temp REAL, hum REAL, date TIMESTAMP);"
create_table(conn, sql_create)

sensor = Adafruit_DHT.DHT11
pin = 'P8_10'
temp = []
hum = []

read = True # zmienic na false zeby wyswietlic dane z tabeli

if read:
    while True:
        humidity, temperature = Adafruit_DHT.read_retry(sensor, pin)
        if humidity is not None and temperature is not None:
            temp.append(temperature)
            hum.append(humidity)

            if len(temp) == 18:
                temp.remove(max(temp)); temp.remove(min(temp))
                t = sum(temp)/len(temp)

                hum.remove(max(hum)); hum.remove(min(hum))
                h = sum(hum)/len(hum)

                d = datetime.datetime.now()
                values = (t, h, d)
                insert2db(conn, values)

                temp.clear()
                hum.clear()
        else:
            print('Failed to get reading. Try again!')
else:
    return_table(conn)


