import csv
import glob
import json
import os
import pprint

import psycopg2 as psyco

import SqlStatements

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

                        insertData(conn,
                                   curs,
                                   SqlStatements.INSERT_INTO_USERS,
                                   SqlStatements.mapInputToUsers(row))

    return


def insertData(conn, cursor, sql, data):
    try:
        cursor.execute(sql, data)
    except Exception as e:
        print("""Exception occurred during sql execution, continuing loop.\nsql: {}\ndata: {}\nException: {}"""
              .format(sql, data, e))
        conn.rollback()

    return


loadUsers(properties.get("loadDataSet.users-directory"))
