import twitter.models
import json
import os
import psycopg2 as psyco
import time
from numpy import long
from datetime import datetime
import concurrent

# noinspection SqlNoDataSourceInspection,SqlDialectInspection
SELECT_TWEETS_TO_PROCESS_SQL = """SELECT 
                                    tweet_id, 
                                    tweet_content
                                  FROM tss_dev.fake_news_tweets
                                  WHERE tweet_content IS NOT NULL
                                    AND (tweet_content ->> 'ERROR_RESPONSE' IS NULL)
                                    AND length_char IS NULL;"""

# noinspection SqlNoDataSourceInspection,SqlDialectInspection
COMPLETION_CRITERIA = """SELECT COUNT(*)
                         FROM tss_dev.fake_news_tweets
                         WHERE tweet_content IS NULL;"""

ROOT_DIR = os.path.dirname(os.path.abspath(__file__))

PROPERTIES_FILE = 'twitter-client.properties'
properties = dict(line.strip().split('=') for line in open(ROOT_DIR + '/' + PROPERTIES_FILE))

conn = psyco.connect(dbname="cs_776", user="system", password="system", host="localhost")


def run(batchSize, sleepTimeSeconds, numOfThreads):

    while True:
        curs = conn.cursor()
        try:
            curs.execute(SELECT_TWEETS_TO_PROCESS_SQL)
        except Exception as e:
            print("Unable to select tweets to process. Exception: {}".format(e))
            return

        tweetsToProcess = curs.fetchmany(batchSize)

        if not tweetsToProcess:
            try:
                curs.execute(COMPLETION_CRITERIA)
            except Exception as e:
                print("Unable to execute completion criteria. Exception: {}".format(e))
                return

            waitingToHydrate = curs.fetchone()

            if waitingToHydrate[0] == 0:
                print("All tweets processed, exiting loop")
                return

            print("No tweets to process at this time, sleeping for {} seconds".format(sleepTimeSeconds))
            time.sleep(sleepTimeSeconds)
            continue

        curs.close()

    # process those in multi-threaded fashion

    # wait for batch to finish, then repeat

    return


run(5, 10, 10)