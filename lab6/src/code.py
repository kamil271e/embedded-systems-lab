#!/usr/bin/python
import Adafruit_DHT
import datetime
import sqlite3
from sqlite3 import Error
import Adafruit_BBIO.UART as UART
import serial
import time
# pip install pyserial

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
    sql_insert = "INSERT INTO measurements(temp, date) VALUES(?,?)"
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
sql_create = "CREATE TABLE IF NOT EXISTS measurements (id INTEGER PRIMARY KEY AUTOINCREMENT, temp REAL, date TIMESTAMP);"
create_table(conn, sql_create)


UART.setup("UART1")

ser = serial.Serial(port = "/dev/ttyO1", baudrate=9600)
ser.close()
ser.open()

read = True

if not read:
    while True:
        if ser.isOpen():
            t = float(ser.readline().decode().split(' ')[0])  # byte strings to unicode
            if t is not None:
                # print(t)
                d = datetime.datetime.now()
                values = (t, d)
                insert2db(conn, values)
            else:
                print('Failed to get reading. Try again!')
else:
    return_table(conn)

#         	RX 	    TX 	    CTS 	RTS 	Device
# UART1 	P9_26 	P9_24 	P9_20 	P9_19 	/dev/ttyO1
