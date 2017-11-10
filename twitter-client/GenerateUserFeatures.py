import os
import psycopg2 as psyco
import concurrent
import SqlStatements

ROOT_DIR = os.path.dirname(os.path.abspath(__file__))

PROPERTIES_FILE = 'twitter-client.properties'
properties = dict(line.strip().split('=') for line in open(ROOT_DIR + '/' + PROPERTIES_FILE))

conn = psyco.connect(dbname="cs_776", user="system", password="system", host="localhost")


def run(batchSize, numOfThreads):
    while True:
        curs = conn.cursor()
        try:
            curs.execute(SqlStatements.GEN_FEATURES_FOR_USERS)
        except Exception as e:
            print("Unable to select users to process. Exception: {}".format(e))
            return

        tweetsToProcess = curs.fetchmany(batchSize)

        if not tweetsToProcess:
            print("Features generated for all users, quitting")
            return

        curs.close()

    # process those in multi-threaded fashion

    # wait for batch to finish, then repeat

    return


run(5, 10)
