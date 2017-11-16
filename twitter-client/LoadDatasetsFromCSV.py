import csv
import glob
import json
import os
import codecs

import psycopg2 as psyco

import SqlStatements

ROOT_DIR = os.path.dirname(os.path.abspath(__file__))

PROPERTIES_FILE = 'dataset.properties'
properties = dict(line.strip().split('=') for line in open(ROOT_DIR + '/' + PROPERTIES_FILE))

conn = psyco.connect(dbname="cs_776", user="system", password="system", host="localhost")


"""
do not import tweets that are not in english
last step: remove users from user_features that don't have any tweets
"""
def loadUsers(usersDir):
    if not usersDir:
        print("usersDir is not provided.")
        return

    for file_name in glob.glob1(ROOT_DIR + '/' + usersDir, "*"):
        classification = 0  # genuine
        if 'social' in file_name.lower():
            classification = 1
        elif 'traditional' in file_name.lower():
            classification = 2

        usersDataFile = ROOT_DIR + '/' + usersDir + file_name

        userInsertCounter = 0
        with conn:
            with conn.cursor() as curs:
                with open(usersDataFile) as inFile:
                    userReader = csv.DictReader(x.replace('\0', '') for x in inFile)
                    for line in userReader:
                        row = json.loads(json.dumps(line).replace("\\ufeff", ""))
                        row['classification'] = classification
                        try:
                            SqlStatements.modifyData(conn,
                                                     curs,
                                                     SqlStatements.INSERT_INTO_USERS,
                                                     SqlStatements.mapUserInputToUsersTable(row))

                            SqlStatements.modifyData(conn,
                                                     curs,
                                                     SqlStatements.INIT_INSERT_INTO_FEATURES,
                                                     SqlStatements.mapUserInputToFeaturesInit(row))
                        except Exception:
                            # continue on
                            pass

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
                print("going through {}".format(tweetDataFile))
                # if '\0' in open(tweetDataFile).read():
                #     print ("you have null bytes in the file: {}".format(tweetDataFile))

                with codecs.open(tweetDataFile, encoding='utf-8', errors='replace') as inFile:
                    tweetReader = csv.DictReader(x.replace('\0', '') for x in inFile)

                    for line in tweetReader:
                        row = json.loads(json.dumps(line).replace("\\\\ufeff", "").replace("\\ufeff", ""))
                        try:
                            SqlStatements.modifyData(conn,
                                                     curs,
                                                     SqlStatements.INSERT_INTO_TWEETS,
                                                     SqlStatements.mapTweetInputToEntity(row))
                        except Exception:
                            # continue on
                            pass

                        tweetInsertCounter += 1

        print("{} tweets inserted for {}".format(tweetInsertCounter, tweetDataFile))

    return


loadUsers(properties.get("loadDataSet.users-directory"))

loadTweets(properties.get("loadDataSet.tweets-directory"))
