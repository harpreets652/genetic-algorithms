import psycopg2 as psyco
from numpy import long
from datetime import datetime

# noinspection SqlNoDataSourceInspection,SqlDialectInspection
INSERT_INTO_USERS = """INSERT INTO tss_dev.users 
                          (user_id, name, screen_name,
                           statuses_count, followers_count,
                           friends_count, favourites_count,
                           listed_count, url, lang, time_zone,
                           location, default_profile, default_profile_image,
                           geo_enabled, profile_image_url, profile_banner_url,
                           profile_use_background_image, profile_background_image_url_https,
                           profile_text_color, profile_image_url_https,
                           profile_sidebar_border_color, profile_background_tile,
                           profile_sidebar_fill_color, profile_background_image_url,
                           profile_background_color, profile_link_color, utc_offset,
                           is_translator, follow_request_sent, protected,
                           verified, notifications, description, contributors_enabled,
                           following, created_at, timestamp, crawled_at, updated, is_user_genuine)
VALUES (%(user_id)s, %(name)s, %(screen_name)s, 
        %(statuses_count)s, %(followers_count)s, 
        %(friends_count)s, %(favourites_count)s, 
        %(listed_count)s, %(url)s, %(lang)s, %(time_zone)s, 
        %(location)s, %(default_profile)s, %(default_profile_image)s, 
        %(geo_enabled)s, %(profile_image_url)s, %(profile_banner_url)s, 
        %(profile_use_background_image)s, %(profile_background_image_url_https)s, 
        %(profile_text_color)s, %(profile_image_url_https)s, 
        %(profile_sidebar_border_color)s, %(profile_background_tile)s, 
        %(profile_sidebar_fill_color)s, %(profile_background_image_url)s, 
        %(profile_background_color)s, %(profile_link_color)s, %(utc_offset)s, 
        %(is_translator)s, %(follow_request_sent)s, %(protected)s, 
        %(verified)s, %(notifications)s, %(description)s, %(contributors_enabled)s, 
        %(following)s, %(created_at)s, %(timestamp)s, %(crawled_at)s, %(updated)s, %(is_user_genuine)s);"""

# noinspection SqlNoDataSourceInspection,SqlDialectInspection
INSERT_INTO_TWEETS = """INSERT INTO tss_dev.tweets
                          (tweet_id, user_id_fk, tweet_text, 
                          source, truncated, in_reply_to_status_id, 
                          in_reply_to_user_id, retweeted_status_id, 
                          in_reply_to_screen_name, geo, place, contributors, 
                          retweet_count, reply_count, favorite_count, 
                          favorited, retweeted, possibly_sensitive, 
                          num_hashtags, num_urls, num_mentions, 
                          created_at, timestamp, crawled_at, updated)
VALUES (%(tweet_id)s, %(user_id_fk)s, %(tweet_text)s, 
        %(source)s, %(truncated)s, %(in_reply_to_status_id)s, 
        %(in_reply_to_user_id)s, %(retweeted_status_id)s, 
        %(in_reply_to_screen_name)s, %(geo)s, %(place)s, 
        %(contributors)s, %(retweet_count)s, %(reply_count)s, 
        %(favorite_count)s, %(favorited)s, %(retweeted)s, 
        %(possibly_sensitive)s, %(num_hashtags)s, %(num_urls)s, 
        %(num_mentions)s, %(created_at)s, %(timestamp)s, %(crawled_at)s, %(updated)s);"""

# noinspection SqlNoDataSourceInspection,SqlDialectInspection
INIT_INSERT_INTO_FEATURES = """INSERT INTO tss_dev.users_features (user_id) VALUES (%(user_id)s);"""

# noinspection SqlNoDataSourceInspection,SqlDialectInspection
GEN_FEATURES_FOR_USERS_SELECT = """SELECT user_id
                                   FROM tss_dev.users_features
                                   WHERE is_user_genuine IS NULL
                                     AND process_error IS NULL;"""

# noinspection SqlNoDataSourceInspection,SqlDialectInspection
UPDATE_USER_FEATURES = """UPDATE tss_dev.users_features
                          SET is_user_genuine = %(is_genuine)s 
                          WHERE user_id = %(user_id)s"""

# noinspection SqlNoDataSourceInspection,SqlDialectInspection
UPDATE_USER_FEATURES_ERROR = """UPDATE tss_dev.users_features
                                SET process_error = %(process_error)s 
                                WHERE user_id = %(user_id)s"""

# noinspection SqlNoDataSourceInspection,SqlDialectInspection
SELECT_USER_DATA = """SELECT *
                      FROM tss_dev.users
                      WHERE user_id = %(user_id)s """


def mapTweetInputToEntity(inputData):
    return {'tweet_id': convertInputToLong(inputData['id']),
            'user_id_fk': convertInputToLong(inputData['user_id']),
            'tweet_text': stringNullCheck(inputData['text']),
            'source': stringNullCheck(inputData['source']),
            'truncated': convertInputToBool(inputData['truncated']),
            'in_reply_to_status_id': convertInputToLong(inputData['in_reply_to_status_id']),
            'in_reply_to_user_id': convertInputToLong(inputData['in_reply_to_user_id']),
            'retweeted_status_id': convertInputToLong(inputData['retweeted_status_id']),
            'in_reply_to_screen_name': stringNullCheck(inputData['in_reply_to_screen_name']),
            'geo': stringNullCheck(inputData['geo']),
            'place': stringNullCheck(inputData['place']),
            'contributors': stringNullCheck(inputData['contributors']),
            'retweet_count': convertInputToLong(inputData['retweet_count']),
            'reply_count': convertInputToLong(inputData['reply_count']),
            'favorite_count': convertInputToLong(inputData['favorite_count']),
            'favorited': convertInputToBool(inputData['favorited']),
            'retweeted': convertInputToBool(inputData['retweeted']),
            'possibly_sensitive': convertInputToBool(inputData['possibly_sensitive']),
            'num_hashtags': convertInputToLong(inputData['num_hashtags']),
            'num_urls': convertInputToLong(inputData['num_urls']),
            'num_mentions': convertInputToLong(inputData['num_mentions']),
            'created_at': stringNullCheck(inputData['created_at']),
            'timestamp': convertInputToTimestamp(inputData['created_at'], '%a %b %d %H:%M:%S +0000 %Y'),
            'crawled_at': convertInputToTimestamp(inputData['crawled_at'], '%m/%d/%y %H:%M'),
            'updated': convertInputToTimestamp(inputData['updated'], '%m/%d/%y %H:%M')
            }


def mapUserInputToFeaturesInit(inputData):
    return {'user_id': convertInputToLong(inputData['id'])}


def mapUserInputToUsersTable(inputData):
    return {'user_id': convertInputToLong(inputData['id']),
            'name': stringNullCheck(inputData['name']),
            'screen_name': stringNullCheck(inputData['screen_name']),
            'statuses_count': convertInputToLong(inputData['statuses_count']),
            'followers_count': convertInputToLong(inputData['followers_count']),
            'friends_count': convertInputToLong(inputData['friends_count']),
            'favourites_count': convertInputToLong(inputData['favourites_count']),
            'listed_count': convertInputToLong(inputData['listed_count']),
            'url': stringNullCheck(inputData['url']),
            'lang': stringNullCheck(inputData['lang']),
            'time_zone': stringNullCheck(inputData['time_zone']),
            'location': stringNullCheck(inputData['location']),
            'default_profile': convertInputToBool(inputData['default_profile']),
            'default_profile_image': convertInputToBool(inputData['default_profile_image']),
            'geo_enabled': convertInputToBool(inputData['geo_enabled']),
            'profile_image_url': stringNullCheck(inputData['profile_image_url']),
            'profile_banner_url': stringNullCheck(inputData['profile_banner_url']),
            'profile_use_background_image': convertInputToBool(inputData['profile_use_background_image']),
            'profile_background_image_url_https': stringNullCheck(inputData['profile_background_image_url_https']),
            'profile_text_color': stringNullCheck(inputData['profile_text_color']),
            'profile_image_url_https': stringNullCheck(inputData['profile_image_url_https']),
            'profile_sidebar_border_color': stringNullCheck(inputData['profile_sidebar_border_color']),
            'profile_background_tile': convertInputToBool(inputData['profile_background_tile']),
            'profile_sidebar_fill_color': stringNullCheck(inputData['profile_sidebar_fill_color']),
            'profile_background_image_url': stringNullCheck(inputData['profile_background_image_url']),
            'profile_background_color': stringNullCheck(inputData['profile_background_color']),
            'profile_link_color': stringNullCheck(inputData['profile_link_color']),
            'utc_offset': convertInputToLong(inputData['utc_offset']),
            'is_translator': convertInputToBool(inputData['is_translator']),
            'follow_request_sent': stringNullCheck(inputData['follow_request_sent']),
            'protected': convertInputToBool(inputData['protected']),
            'verified': convertInputToBool(inputData['verified']),
            'notifications': stringNullCheck(inputData['notifications']),
            'description': stringNullCheck(inputData['description']),
            'contributors_enabled': convertInputToBool(inputData['contributors_enabled']),
            'following': stringNullCheck(inputData['following']),
            'created_at': stringNullCheck(inputData['created_at']),
            'timestamp': convertInputToTimestamp(inputData['created_at'], '%a %b %d %H:%M:%S +0000 %Y'),
            'crawled_at': convertInputToTimestamp(inputData['crawled_at'], '%m/%d/%y %H:%M'),
            'updated': convertInputToTimestamp(inputData['updated'], '%m/%d/%y %H:%M'),
            'is_user_genuine': inputData['is_user_genuine']
            }


def stringNullCheck(input):
    if not input or input.strip() == 'NULL':
        return None

    return input.strip()


def convertInputToBool(input):
    if not input or input.strip() == 'NULL':
        return False

    return True if input.strip() == '1' else False


def convertInputToTimestamp(input, fmt):
    if not input or input.strip() == 'NULL':
        return None

    return datetime.strptime(input.strip(), fmt)


def convertInputToLong(input):
    if not input or input.strip() == 'NULL':
        return None

    return long(input.strip())


def modifyData(conn, cursor, sql, data):
    try:
        cursor.execute(sql, data)
    except Exception as e:
        print("""Exception occurred during sql execution, continuing loop.\nsql: {}\ndata: {}\nException: {}"""
              .format(sql, data, e))
        conn.rollback()

    return
