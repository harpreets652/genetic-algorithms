import re
import csv
import glob
import json
import os
import codecs
import string
import psycopg2 as psyco
from langdetect import detect_langs
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

                with codecs.open(tweetDataFile, encoding='utf-8', errors='replace') as inFile:
                    tweetReader = csv.DictReader(x.replace('\0', '') for x in inFile)

                    for line in tweetReader:
                        row = json.loads(json.dumps(line).replace("\\\\ufeff", "").replace("\\ufeff", ""))
                        # detect languages of strings with no URLS, no all-caps words, and no punctuation
                        textToDetect = re.sub(r'http\S+|[A-Z]{2,}', '', row['text'], flags=re.MULTILINE).translate(
                            string.punctuation).strip()

                        if textToDetect:
                            try:
                                langs = detect_langs(textToDetect)
                                if langs:
                                    mostProbableLang = langs[0]
                                    if 'en' != mostProbableLang.lang and mostProbableLang.prob >= 0.75:
                                        continue
                            except Exception:
                                pass

                        try:
                            SqlStatements.modifyData(conn,
                                                     curs,
                                                     SqlStatements.INSERT_INTO_TWEETS,
                                                     SqlStatements.mapTweetInputToEntity(row))
                        except Exception:
                            # continue on
                            pass

                        tweetInsertCounter += 1
                        if tweetInsertCounter > 200:
                            break

        print("{} tweets inserted for {}".format(tweetInsertCounter, tweetDataFile))

    return


def cleanUpData():
    with conn:
        with conn.cursor() as cursor:
            try:
                SqlStatements.modifyData(conn,
                                         cursor,
                                         SqlStatements.DELETE_FEATURE_ROWS_WITH_NO_TWEETS,
                                         {})
            except Exception as e:
                print("Exception occurred during cleaning up.\nException: {}".format(e))

    return


loadUsers(properties.get("loadDataSet.users-directory"))

loadTweets(properties.get("loadDataSet.tweets-directory"))

cleanUpData()
