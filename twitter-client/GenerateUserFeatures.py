import os
import psycopg2 as psyco
import SqlStatements
from vaderSentiment.vaderSentiment import SentimentIntensityAnalyzer
from psycopg2 import extras
from concurrent.futures import ThreadPoolExecutor
from concurrent.futures import as_completed
from datetime import datetime
from numpy import long

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
            userFeatures = generateUserFeatures(conn, userId)
            usertTweetFeatures = generateTweetFeatures(conn, userId)

            data = {'user_id': userId}
            data.update(userFeatures)
            data.update(usertTweetFeatures)

            with conn.cursor() as curs:
                SqlStatements.modifyData(conn,
                                         curs,
                                         SqlStatements.UPDATE_USER_FEATURES,
                                         data)
            result = True
        except Exception as e:
            with conn.cursor() as curs:
                SqlStatements.modifyData(conn,
                                         curs,
                                         SqlStatements.UPDATE_USER_FEATURES_ERROR,
                                         {'user_id': userId,
                                          'process_error': str(e)})
            result = False

    return result


def generateUserFeatures(connection, userId):
    readCursor = connection.cursor(cursor_factory=extras.NamedTupleCursor)

    readCursor.execute(SqlStatements.SELECT_USER_DATA, {'user_id': userId})
    userData = readCursor.fetchone()

    readCursor.close()

    userFeatures = {}

    # is user genuine
    userFeatures['classification'] = userData.classification

    # registration age
    regAge = (datetime.now() - userData.timestamp).days
    userFeatures['user_age'] = regAge

    # statuses count on day
    userFeatures['user_status_count'] = userData.statuses_count

    # number of followers
    userFeatures['user_num_followers'] = userData.followers_count

    # number of friends
    userFeatures['user_num_friends'] = userData.friends_count

    # is user verified
    userFeatures['user_verified'] = userData.verified

    # has description?
    description = userData.description
    userFeatures['user_has_description'] = True if description and description != 'NULL' else False

    # has url?
    url = userData.url
    userFeatures['user_has_url'] = True if url and url != 'NULL' else False

    return userFeatures


def generateTweetFeatures(connection, userId):
    readCursor = connection.cursor(cursor_factory=extras.NamedTupleCursor)

    readCursor.execute(SqlStatements.SELECT_TWEET_TEXT, {'user_id': userId})
    userTweetTexts = readCursor.fetchall()

    # if no tweets, return default values
    if not userTweetTexts:
        readCursor.close()
        return getDefaultTweetFeatures()

    readCursor.execute(SqlStatements.SELECT_TWEET_TEXT_FEATURES, {'user_id': userId})
    tweetFeaturesFromSql = readCursor.fetchone()

    readCursor.execute(SqlStatements.SELECT_MOST_COMMONLY_TWEETED_HOUR, {'user_id': userId})
    mostCommonlyTweetedHour = readCursor.fetchone()

    readCursor.execute(SqlStatements.SELECT_TWEET_COUNT_BY_DAY, {'user_id': userId})
    tweetCountByDay = readCursor.fetchall()

    readCursor.execute(SqlStatements.SELECT_NUM_TWEETS_MULTIPLE_QUEST_EXCLAM, {'user_id': userId})
    tweetCountMultiQuestAndExclam = readCursor.fetchone()

    readCursor.close()

    userTweetFeatures = {}

    # average number of characters
    userTweetFeatures['avg_length_chars'] = int(tweetFeaturesFromSql.avg_length_char)

    # average number of words
    userTweetFeatures['avg_length_words'] = int(tweetFeaturesFromSql.avg_length_words)

    totalNumTweets = int(tweetFeaturesFromSql.total_count)

    # fraction containing question marks
    fractQuestionMarks = int(tweetFeaturesFromSql.num_question_marks) / totalNumTweets
    userTweetFeatures['fract_contains_question'] = fractQuestionMarks

    # fraction containing exclamation marks
    fractionExclamMarks = int(tweetFeaturesFromSql.num_exclam_marks) / totalNumTweets
    userTweetFeatures['fract_contains_exclamation'] = fractionExclamMarks

    # fraction containing multiple exclamation or question marks
    numOfTweetsWithMultiQuestAndExclam = int(tweetCountMultiQuestAndExclam.tweet_count)
    userTweetFeatures['fract_contains_multiple_quest_exlam'] = numOfTweetsWithMultiQuestAndExclam / totalNumTweets

    # fraction containing urls
    fractionUrls = int(tweetFeaturesFromSql.num_containing_urls) / totalNumTweets
    userTweetFeatures['fract_contains_urls'] = fractionUrls

    # average number of urls
    userTweetFeatures['avg_number_of_urls'] = long(tweetFeaturesFromSql.avg_num_urls)

    # average number of urls in top 100
    userTweetFeatures['contains_urls_top_100'] = long(tweetFeaturesFromSql.contains_urls_top_100) / totalNumTweets

    # fraction containing user mentions
    fractContainsUserMention = int(tweetFeaturesFromSql.num_containing_mentions) / totalNumTweets
    userTweetFeatures['fract_contains_user_mention'] = fractContainsUserMention

    # fraction containing hashtags
    userTweetFeatures['fract_contains_hashtag'] = int(tweetFeaturesFromSql.num_containing_hashtags) / totalNumTweets

    # fraction retweeted
    userTweetFeatures['fract_retweeted'] = int(tweetFeaturesFromSql.num_retweeted) / totalNumTweets

    # most commonly tweeted hour
    userTweetFeatures['most_commonly_tweeted_hour'] = int(mostCommonlyTweetedHour.tweet_hour)

    # tweet count by day
    countByDayPrefix = 'num_tweets_day_'
    for days in SqlStatements.DAY_OF_WEEK_POSTGRES_MAPPING.values():
        userTweetFeatures[countByDayPrefix + days] = 0

    for countByDay in tweetCountByDay:
        userTweetFeatures[countByDayPrefix +
                          SqlStatements.DAY_OF_WEEK_POSTGRES_MAPPING[countByDay.tweet_day]] = int(countByDay.num_tweets)

    # fraction of personal pronouns
    userTweetFeatures['fract_contains_pronoun_first_p'] = int(tweetFeaturesFromSql.first_person) / totalNumTweets
    userTweetFeatures['fract_contains_pronoun_second_p'] = int(tweetFeaturesFromSql.second_person) / totalNumTweets
    userTweetFeatures['fract_contains_pronoun_third_p'] = int(tweetFeaturesFromSql.third_person) / totalNumTweets

    # sentiment analysis: average number of positive and negative words in texts using VADER Scoring
    # todo: score ranges from -1 to 1; is avg the best way to capture these values?
    totalPositiveScore = totalNegativeScore = totalCompoundScore = 0
    for tweet in userTweetTexts:
        sentimentScores = getSentimentWordCounts(tweet.tweet_text)
        totalPositiveScore += sentimentScores['pos']
        totalNegativeScore += sentimentScores['neg']
        totalCompoundScore += sentimentScores['compound']

    userTweetFeatures['avg_sentiment_pos_words'] = totalPositiveScore / totalNumTweets
    userTweetFeatures['avg_sentiment_neg_words'] = totalNegativeScore / totalNumTweets
    userTweetFeatures['avg_sentiment_score'] = totalCompoundScore / totalNumTweets

    return userTweetFeatures


def getDefaultTweetFeatures():
    return {'avg_length_chars': 0, 'avg_length_words': 0,
            'fract_contains_question': 0, 'fract_contains_exclamation': 0,
            'fract_contains_multiple_quest_exlam': 0, 'fract_contains_urls': 0,
            'avg_number_of_urls': 0, 'fract_contains_user_mention': 0,
            'fract_contains_hashtag': 0, 'fract_retweeted': 0,
            'most_commonly_tweeted_hour': 0, 'num_tweets_day_sun': 0,
            'num_tweets_day_mon': 0, 'num_tweets_day_tues': 0,
            'num_tweets_day_wed': 0, 'num_tweets_day_thur': 0,
            'num_tweets_day_fri': 0, 'num_tweets_day_sat': 0,
            'fract_contains_pronoun_first_p': 0, 'fract_contains_pronoun_second_p': 0,
            'fract_contains_pronoun_third_p': 0, 'avg_sentiment_pos_words': 0,
            'avg_sentiment_neg_words': 0, 'avg_sentiment_score': 0,
            'contains_urls_top_100': 0
            }


def getSentimentWordCounts(tweet):
    analyzer = SentimentIntensityAnalyzer()
    scores = analyzer.polarity_scores(tweet)

    return scores


run(5, 5)
