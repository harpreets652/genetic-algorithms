import os
import psycopg2 as psyco
from concurrent.futures import ThreadPoolExecutor
from concurrent.futures import as_completed
import SqlStatements
from datetime import datetime

ROOT_DIR = os.path.dirname(os.path.abspath(__file__))

PROPERTIES_FILE = 'twitter-client.properties'
properties = dict(line.strip().split('=') for line in open(ROOT_DIR + '/' + PROPERTIES_FILE))

conn = psyco.connect(dbname="cs_776", user="system", password="system", host="localhost")


def run(batchSize, numOfThreads):
    while True:
        curs = conn.cursor()
        try:
            curs.execute(SqlStatements.GEN_FEATURES_FOR_USERS_SELECT)
        except Exception as e:
            print("Unable to select users to process. Exception: {}".format(e))
            return

        usersToProcess = curs.fetchmany(batchSize)

        if not usersToProcess:
            print("Features generated for all users, quitting")
            return
        curs.close()

        completionCounter = 0
        totalNumber = len(usersToProcess)
        with ThreadPoolExecutor(max_workers=numOfThreads) as executor:
            futures = [executor.submit(generateFeatures, userId) for userId in usersToProcess]

            for future in as_completed(futures):
                try:
                    futureResult = future.result()

                    if futureResult:
                        completionCounter += 1
                except Exception as e:
                    print("Exception occurred while retrieving results.\nException: {}".format(e))

        print("{} users successfully processed out of {}".format(completionCounter, totalNumber))

    return


def generateFeatures(userId):
    with conn:
        try:
            # get user data
            userData = getUserData(conn, userId)

            userFeatures = generateUserFeatures(userData)

            usertTweetFeatures = generateTweetFeatures(conn, userData)

            data = {'user_id': userId,
                    'is_genuine': userData['is_user_genuine']}
            data.update(userFeatures)
            data.update(usertTweetFeatures)

            sql = SqlStatements.UPDATE_USER_FEATURES
            result = True
        except Exception as e:
            data = {'user_id': userId,
                    'process_error': str(e)}
            sql = SqlStatements.UPDATE_USER_FEATURES_ERROR
            result = False

        with conn.cursor() as curs:
            SqlStatements.modifyData(conn, curs, sql, data)

    return result


def getUserData(connection, userId):
    readCursor = connection.cursor()
    readCursor.execute(SqlStatements.SELECT_USER_DATA, {'user_id': userId})

    columns = []
    for col in readCursor.description:
        columns.append(col[0])
    data = readCursor.fetchone()

    userData = dict(zip(columns, data))

    readCursor.close()

    return userData


def generateUserFeatures(userData):
    userFeatures = {}

    # registration age
    regAge = (datetime.now() - userData['timestamp']).days
    userFeatures['user_age'] = regAge

    # statuses count on day
    userFeatures['user_status_count'] = userData['statuses_count']

    # number of followers
    userFeatures['user_num_followers'] = userData['followers_count']

    # number of friends
    userFeatures['user_num_friends'] = userData['friends_count']

    # is user verified
    userFeatures['user_verified'] = userData['verified']

    # has description?
    description = userData['description']
    userFeatures['user_has_description'] = True if description and description != 'NULL' else False

    # has url?
    url = userData['url']
    userFeatures['user_has_url'] = True if url and url != 'NULL' else False

    return userFeatures


def generateTweetFeatures(connection, userData):
    readCursor = connection.cursor()
    readCursor.execute(SqlStatements.SELECT_TWEET_TEXT_FEATURES, {'user_id': userData['user_id']})

    columns = []
    for col in readCursor.description:
        columns.append(col[0])
    data = readCursor.fetchone()

    tweetFeaturesFromSql = dict(zip(columns, data))
    readCursor.close()

    userTweetFeatures = {}

    # average number of characters
    userTweetFeatures['avg_length_chars'] = int(tweetFeaturesFromSql['avg_length_char'])

    # average number of words
    userTweetFeatures['avg_length_words'] = int(tweetFeaturesFromSql['avg_length_words'])

    return userTweetFeatures


run(5, 5)
