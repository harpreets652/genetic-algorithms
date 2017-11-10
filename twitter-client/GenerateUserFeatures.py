import os
import psycopg2 as psyco
from concurrent.futures import ThreadPoolExecutor
from concurrent.futures import as_completed
import SqlStatements

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
            readCursor = conn.cursor()
            readCursor.execute(SqlStatements.SELECT_USER_DATA, {'user_id': userId})
            userData = readCursor.fetchone()
            readCursor.close()

            data = {'user_id': userId,
                    'is_genuine': userData[40]}
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


run(5, 5)
