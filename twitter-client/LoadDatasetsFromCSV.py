import csv
import glob
import json
import os

import psycopg2 as psyco

import SqlStatements

ROOT_DIR = os.path.dirname(os.path.abspath(__file__))

PROPERTIES_FILE = 'dataset.properties'
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

        userInsertCounter = 0
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
                                   SqlStatements.mapUserInputToUsersTable(row))

                        insertData(conn,
                                   curs,
                                   SqlStatements.INIT_INSERT_INTO_FEATURES,
                                   SqlStatements.mapUserInputToFeaturesInit(row))
                        userInsertCounter += 1

        print("{} users inserted for {}".format(userInsertCounter, usersDataFile))

    return


def loadTweets(tweetDir):
    if not tweetDir:
        print("tweetDir is not provided.")
        return

    for file_name in glob.glob1(ROOT_DIR + '/' + tweetDir, "*"):
        tweetDataFile = ROOT_DIR + '/' + tweetDir + file_name

        tweetInsertCounter = 0
        with conn:
            with conn.cursor() as curs:
                with open(tweetDataFile) as inFile:
                    tweetReader = csv.DictReader(inFile)

                    for line in tweetReader:
                        row = json.loads(json.dumps(line).replace("\\ufeff", ""))

                        insertData(conn,
                                   curs,
                                   SqlStatements.INSERT_INTO_TWEETS,
                                   SqlStatements.mapTweetInputToEntity(row))
                        tweetInsertCounter += 1

        print("{} tweets inserted for {}".format(tweetInsertCounter, tweetDataFile))

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

loadTweets(properties.get("loadDataSet.tweets-directory"))
