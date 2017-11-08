import twitter
import twitter.models
import os
import psycopg2 as psyco
import json
from datetime import datetime
from numpy import long

# noinspection SqlNoDataSourceInspection,SqlDialectInspection
SELECT_TWEETS_TO_HYDRATE_SQL = """SELECT tweet_id FROM tss_dev.fake_news_tweets 
                                  WHERE tweet_content IS NULL;"""

# noinspection SqlNoDataSourceInspection,SqlDialectInspection
INSERT_TWEET_IDS_SQL = """INSERT INTO tss_dev.fake_news_tweets (tweet_id) 
                          VALUES (%(tweetId)s);"""

# noinspection SqlNoDataSourceInspection,SqlDialectInspection
UPDATE_TWEET_RECORD = """UPDATE tss_dev.fake_news_tweets
                         SET tweet_content = %(tweetJSON)s,
                             tweet_created_date = %(tweetCreatedDate)s
                         WHERE tweet_id = %(tweetId)s;"""

ROOT_DIR = os.path.dirname(os.path.abspath(__file__))

PROPERTIES_FILE = 'twitter-client.properties'
properties = dict(line.strip().split('=') for line in open(ROOT_DIR + '/' + PROPERTIES_FILE))

conn = psyco.connect(dbname="cs_776", user="system", password="system", host="localhost")


def loadTweetIds(tweetIdsFile):
    if not tweetIdsFile:
        return

    insertCounter = 0
    totalCounter = 0
    with conn:
        with conn.cursor() as curs:
            for line in open(ROOT_DIR + '/' + tweetIdsFile):
                tweetId = long(line.strip())
                totalCounter += 1
                try:
                    curs.execute(INSERT_TWEET_IDS_SQL, {'tweetId': tweetId})
                    insertCounter += 1
                except Exception as e:
                    print("Exception occurred during insert, continuing loop. Exception: {}".format(e))
                    conn.rollback()

    print("{} tweet ids inserted out of {}".format(insertCounter, totalCounter))
    return


def hydrateTweets(batchSize):
    api = twitter.Api(consumer_key=properties.get("api-key"),
                      consumer_secret=properties.get("api-secret"),
                      access_token_key=properties.get("access-token-key"),
                      access_token_secret=properties.get("access-token-secret"),
                      sleep_on_rate_limit=True)

    print("-----> {}: Starting to hydrate tweets.".format(str(datetime.now())))
    progessCounter = 0
    while True:
        curs = conn.cursor()
        try:
            curs.execute(SELECT_TWEETS_TO_HYDRATE_SQL)
        except Exception as e:
            print("Unable to select tweets to hydrate. Exception: {}".format(e))
            return

        tweetsToHydrate = curs.fetchmany(batchSize)
        curs.close()

        if not tweetsToHydrate:
            print("No tweets to hydrate, ending loop")
            return

        with conn:
            with conn.cursor() as loopCurs:
                for tweetId in tweetsToHydrate:
                    try:
                        tweet = api.GetStatus(status_id=tweetId[0])
                        tweetJson = tweet.AsJsonString()
                        createdDate = datetime.strptime(tweet.created_at, '%a %b %d %H:%M:%S +0000 %Y')
                    except Exception as e:
                        print(
                            "Exception occurred from Twitter.GetStatus for Id {}. Exception: {}".format(tweetId[0], e))
                        tweetJson = json.dumps({'ERROR_RESPONSE': str(e)})
                        createdDate = datetime.now()

                    try:
                        loopCurs.execute(UPDATE_TWEET_RECORD, {'tweetId': tweetId[0],
                                                               'tweetCreatedDate': createdDate,
                                                               'tweetJSON': tweetJson})
                    except Exception as e:
                        print("Exception occurred while inserting json for {}. Exception: {}".format(tweetId[0], e))
                        conn.rollback()

        progessCounter += len(tweetsToHydrate)
        if progessCounter % 10000 == 0:
            print("-----> {}: {} tweets hydrated".format(str(datetime.now()), progessCounter))

    return


# read tweet guids and save them in the db
# loadTweetIds(properties.get("hydrate.dataFile"))

# hydrate tweets
hydrateTweets(int(properties.get("hydrate.batchSize")))

# close connection
conn.close()
