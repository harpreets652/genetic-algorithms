import os
import psycopg2 as psyco
import csv
import glob
import json
from datetime import datetime
from numpy import long
import SqlStatements
import pprint

ROOT_DIR = os.path.dirname(os.path.abspath(__file__))

PROPERTIES_FILE = 'twitter-client.properties'
properties = dict(line.strip().split('=') for line in open(ROOT_DIR + '/' + PROPERTIES_FILE))

conn = psyco.connect(dbname="cs_776", user="system", password="system", host="localhost")


def loadUsers(usersDir):
    if not usersDir:
        print("usersDir is not provided.")
        return

    for file_name in glob.glob1(ROOT_DIR + '/' + usersDir, "*"):
        genuineUsers = False
        if 'genuine' in file_name:
            genuineUsers = True

        usersDataFile = ROOT_DIR + '/' + usersDir + file_name
        with conn:
            with conn.cursor() as curs:
                with open(usersDataFile) as inFile:
                    userReader = csv.DictReader(inFile)
                    for line in userReader:
                        row = json.loads(json.dumps(line).replace("\\ufeff", ""))
                        row['is_user_genuine'] = genuineUsers

                        pprint.pprint(row)
                        insertUser(conn, curs, SqlStatements.mapInputToUsers(row))

    return


def insertUser(conn, cursor, data):
    try:
        cursor.execute(SqlStatements.INSERT_INTO_USERS, data)
    except Exception as e:
        print("""Exception occurred during user insert, continuing loop.\ndata: {}\nException: {}""".format(data, e))
        conn.rollback()

    return


loadUsers(properties.get("loadDataSet.users-directory"))
