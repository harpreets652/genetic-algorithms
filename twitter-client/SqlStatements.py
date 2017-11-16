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
                           following, created_at, timestamp, updated, classification)
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
        %(following)s, %(created_at)s, %(timestamp)s, %(updated)s, %(classification)s);"""

# noinspection SqlNoDataSourceInspection,SqlDialectInspection
INSERT_INTO_TWEETS = """INSERT INTO tss_dev.tweets
                          (tweet_id, user_id_fk, tweet_text, 
                          source, truncated, in_reply_to_status_id, 
                          in_reply_to_user_id, retweeted_status_id, 
                          in_reply_to_screen_name, geo, place, contributors, 
                          retweet_count, reply_count, favorite_count, 
                          favorited, retweeted, possibly_sensitive, 
                          num_hashtags, num_urls, num_mentions, 
                          created_at, timestamp)
VALUES (%(tweet_id)s, %(user_id_fk)s, %(tweet_text)s, 
        %(source)s, %(truncated)s, %(in_reply_to_status_id)s, 
        %(in_reply_to_user_id)s, %(retweeted_status_id)s, 
        %(in_reply_to_screen_name)s, %(geo)s, %(place)s, 
        %(contributors)s, %(retweet_count)s, %(reply_count)s, 
        %(favorite_count)s, %(favorited)s, %(retweeted)s, 
        %(possibly_sensitive)s, %(num_hashtags)s, %(num_urls)s, 
        %(num_mentions)s, %(created_at)s, %(timestamp)s);"""

# noinspection SqlNoDataSourceInspection,SqlDialectInspection
INIT_INSERT_INTO_FEATURES = """INSERT INTO tss_dev.users_features (user_id) VALUES (%(user_id)s);"""

# noinspection SqlNoDataSourceInspection,SqlDialectInspection
GEN_FEATURES_FOR_USERS_SELECT = """SELECT user_id
                                   FROM tss_dev.users_features
                                   WHERE classification IS NULL
                                     AND process_error IS NULL;"""

# noinspection SqlNoDataSourceInspection,SqlDialectInspection
UPDATE_USER_FEATURES = """UPDATE tss_dev.users_features
                          SET classification = %(classification)s, user_age = %(user_age)s,
                              user_status_count = %(user_status_count)s, user_num_followers = %(user_num_followers)s,
                              user_num_friends = %(user_num_friends)s, user_verified = %(user_verified)s,
                              user_has_description = %(user_has_description)s, user_has_url = %(user_has_url)s,
                              avg_length_chars = %(avg_length_chars)s, avg_length_words = %(avg_length_words)s,
                              fract_contains_question = %(fract_contains_question)s, 
                              fract_contains_exclamation = %(fract_contains_exclamation)s,
                              fract_contains_urls = %(fract_contains_urls)s, avg_number_of_urls = %(avg_number_of_urls)s,
                              fract_urls_top_100 = %(fract_urls_top_100)s,
                              fract_contains_company = %(fract_contains_company)s,
                              fract_contains_user_mention = %(fract_contains_user_mention)s,
                              fract_contains_hashtag = %(fract_contains_hashtag)s, fract_retweeted = %(fract_retweeted)s,
                              most_commonly_tweeted_hour = %(most_commonly_tweeted_hour)s,
                              num_tweets_day_sun = %(num_tweets_day_sun)s, num_tweets_day_mon = %(num_tweets_day_mon)s,
                              num_tweets_day_tues = %(num_tweets_day_tues)s, num_tweets_day_wed = %(num_tweets_day_wed)s,
                              num_tweets_day_thur = %(num_tweets_day_thur)s, num_tweets_day_fri = %(num_tweets_day_fri)s,
                              num_tweets_day_sat = %(num_tweets_day_sat)s, 
                              fract_contains_multiple_quest_exlam = %(fract_contains_multiple_quest_exlam)s,
                              fract_contains_pronoun_first_p = %(fract_contains_pronoun_first_p)s,
                              fract_contains_pronoun_second_p = %(fract_contains_pronoun_second_p)s,
                              fract_contains_pronoun_third_p = %(fract_contains_pronoun_third_p)s,
                              avg_sentiment_pos_words = %(avg_sentiment_pos_words)s, avg_sentiment_neg_words = %(avg_sentiment_neg_words)s,
                              avg_sentiment_score = %(avg_sentiment_score)s, user_followers_to_lifetime = %(user_followers_to_lifetime)s,
                              user_reputation_ratio = %(user_reputation_ratio)s  
                          WHERE user_id = %(user_id)s"""

# noinspection SqlNoDataSourceInspection,SqlDialectInspection
UPDATE_USER_FEATURES_ERROR = """UPDATE tss_dev.users_features
                                SET process_error = %(process_error)s 
                                WHERE user_id = %(user_id)s"""

# noinspection SqlNoDataSourceInspection,SqlDialectInspection
SELECT_USER_DATA = """SELECT *
                      FROM tss_dev.users
                      WHERE user_id = %(user_id)s """

# noinspection SqlNoDataSourceInspection,SqlDialectInspection
SELECT_TWEET_TEXT = """SELECT tweet_text
                       FROM tss_dev.tweets
                       WHERE user_id_fk = %(user_id)s;"""

# noinspection SqlNoDataSourceInspection,SqlDialectInspection
SELECT_TWEET_TEXT_FEATURES = """SELECT 
                                   t.user_id_fk AS user_id,
                                   count(t.tweet_id) AS total_count,
                                   avg(length(t.tweet_text)) AS avg_length_char,
                                   avg(array_length(regexp_split_to_array(t.tweet_text, '\s'), 1)) AS avg_length_words,
                                   SUM(CASE WHEN t.tweet_text LIKE '%%\?%%' THEN 1 ELSE 0 END) AS num_question_marks,
                                   SUM(CASE WHEN t.tweet_text LIKE '%%\!%%' THEN 1 ELSE 0 END) AS num_exclam_marks,
                                   avg(t.num_urls) AS avg_num_URLs,                                   
                                   SUM(CASE WHEN t.num_urls > 0 THEN 1 ELSE 0 END) AS num_containing_URLs,
                                   SUM(CASE WHEN (t.num_urls > 0) AND (
                                       t.tweet_text similar to '%%[^[:alnum:]](facebook|twitter|google|youtube|linkedin|wordpress|instagram|pinterest|wikipedia|blogspot|apple|adobe|tumblr|amazon|vimeo|goo|microsoft|yahoo|flickr|qq|bit|godaddy|buydomains|vk|baidu|reddit|w3|nytimes|t|europa|weebly|statcounter|gov|bbc|mozilla|myspace|yandex|blogger|soundcloud|123-reg|github|wp|miitbeian|jimdo|theguardian|addthis|wix|cnn|paypal|nih|bluehost|creativecommons|wixsite|whoisprivacyprotect|digg|stumbleupon|huggingtonpost|issuu|gravatar|imdb|ascii|feedburner|yelp|parallels|dropbox|amazonaws|forbes|tinyurl|miibeian|msn|go|wsj|e-recht24|addtoany|slideshare|etsy|weibo|archive|ameblo|bing|eventbrite|fc2|ebay|free|telegraph|sourceforge|51|taobao|livejournal|mail|about|reuters)[^[:alnum:]]%%'
                                     ) THEN 1 ELSE 0 END) as num_urls_in_top_100,
                                   SUM(CASE WHEN (
                                      t.tweet_text similar to '%%[^[:alnum:]](ICBC|icbc|China Construction Bank|CHINA CONSTRUCTION BANK|china construction bank|Agricultural Bank of China|AGRICULTURAL BANK OF CHINA|agricultural bank of china|Agricultural Bank China|JPMorgan Chase|JPMORGAN CHASE|jpmorgan chase|Berkshire Hathaway|BERKSHIRE HATHAWAY|berkshire hathaway|Exxon Mobil|EXXON MOBIL|exxon mobil|General Electric|GENERAL ELECTRIC|general electric|Wells Fargo|WELLS FARGO|wells fargo|Bank of China|BANK OF CHINA|bank of china|Bank China|PetroChina|PETROCHINA|petrochina|Royal Dutch Shell|ROYAL DUTCH SHELL|royal dutch shell|Toyota Motor|TOYOTA MOTOR|toyota motor|Bank of America|BANK OF AMERICA|bank of america|Bank America|HSBC Holdings|HSBC HOLDINGS|hsbc holdings|Apple|APPLE|apple|Citigroup|CITIGROUP|citigroup|BP|bp|Chevron|CHEVRON|chevron|Volkswagen Group|VOLKSWAGEN GROUP|volkswagen group|Wal-Mart Stores|WAL-MART STORES|wal-mart stores|Gazprom|GAZPROM|gazprom|Samsung Electronics|SAMSUNG ELECTRONICS|samsung electronics|AT&T|at&t|BNP Paribas|BNP PARIBAS|bnp paribas|Total|TOTAL|total|Verizon Communications|VERIZON COMMUNICATIONS|verizon communications|Allianz|ALLIANZ|allianz|China Mobile|CHINA MOBILE|china mobile|Sinopec-China Petroleum|SINOPEC-CHINA PETROLEUM|sinopec-china petroleum|Petrobras|PETROBRAS|petrobras|Daimler|DAIMLER|daimler|Microsoft|MICROSOFT|microsoft|AXA Group|AXA GROUP|axa group|Rosneft|ROSNEFT|rosneft|IBM|ibm|Nestle|NESTLE|nestle|Mitsubishi UFJ Financial|MITSUBISHI UFJ FINANCIAL|mitsubishi ufj financial|Vodafone|VODAFONE|vodafone|Eni|ENI|eni|Procter & Gamble|PROCTER & GAMBLE|procter & gamble|Procter Gamble|Johnson & Johnson|JOHNSON & JOHNSON|johnson & johnson|Johnson Johnson|American International Group|AMERICAN INTERNATIONAL GROUP|american international group|Banco Santander|BANCO SANTANDER|banco santander|BHP Billiton|BHP BILLITON|bhp billiton|Pfizer|PFIZER|pfizer|ItaÃº Unibanco Holding|ITAÃº UNIBANCO HOLDING|itaãº unibanco holding|Ford Motor|FORD MOTOR|ford motor|BMW Group|BMW GROUP|bmw group|Commonwealth Bank|COMMONWEALTH BANK|commonwealth bank|EDF|edf|Statoil|STATOIL|statoil|Google|GOOGLE|google|Siemens|SIEMENS|siemens|Novartis|NOVARTIS|novartis|Royal Bank of Canada|ROYAL BANK OF CANADA|royal bank of canada|Royal Bank Canada|Sumitomo Mitsui Financial|SUMITOMO MITSUI FINANCIAL|sumitomo mitsui financial|Comcast|COMCAST|comcast|Sberbank|SBERBANK|sberbank|Goldman Sachs Group|GOLDMAN SACHS GROUP|goldman sachs group|Westpac Banking Group|WESTPAC BANKING GROUP|westpac banking group|Nippon Telegraph & Tel|NIPPON TELEGRAPH & TEL|nippon telegraph & tel|Nippon Telegraph Tel|Ping An Insurance Group|PING AN INSURANCE GROUP|ping an insurance group|Banco Bradesco|BANCO BRADESCO|banco bradesco|Anheuser-Busch InBev|ANHEUSER-BUSCH INBEV|anheuser-busch inbev|Bank of Communications|BANK OF COMMUNICATIONS|bank of communications|Bank Communications|China Life Insurance|CHINA LIFE INSURANCE|china life insurance|General Motors|GENERAL MOTORS|general motors|TelefÃ³nica|TELEFÃ³NICA|telefã³nica|MetLife|METLIFE|metlife|Honda Motor|HONDA MOTOR|honda motor|Enel|ENEL|enel|BASF|basf|Softbank|SOFTBANK|softbank|National Australia Bank|NATIONAL AUSTRALIA BANK|national australia bank|ANZ|anz|TD Bank Group|TD BANK GROUP|td bank group|ConocoPhillips|CONOCOPHILLIPS|conocophillips|Intel|INTEL|intel|UBS|ubs|Hewlett-Packard|HEWLETT-PACKARD|hewlett-packard|Coca-Cola|COCA-COLA|coca-cola|Cisco Systems|CISCO SYSTEMS|cisco systems|LukOil|LUKOIL|lukoil|Boeing|BOEING|boeing|Zurich Insurance Group|ZURICH INSURANCE GROUP|zurich insurance group|UnitedHealth Group|UNITEDHEALTH GROUP|unitedhealth group|Hyundai Motor|HYUNDAI MOTOR|hyundai motor|Sanofi|SANOFI|sanofi|Credit Agricole|CREDIT AGRICOLE|credit agricole|United Technologies|UNITED TECHNOLOGIES|united technologies|Roche Holding|ROCHE HOLDING|roche holding|Munich Re|MUNICH RE|munich re|PepsiCo|PEPSICO|pepsico|Oracle|ORACLE|oracle|Bank of Nova Scotia|BANK OF NOVA SCOTIA|bank of nova scotia|Bank Nova Scotia|CVS Caremark|CVS CAREMARK|cvs caremark|ING Group|ING GROUP|ing group|Saudi Basic Industries|SAUDI BASIC INDUSTRIES|saudi basic industries|Merck & Co|MERCK & CO|merck & co|Merck Co|Walt Disney|WALT DISNEY|walt disney|Mizuho Financial|MIZUHO FINANCIAL|mizuho financial|China Merchants Bank|CHINA MERCHANTS BANK|china merchants bank|SociÃ©tÃ© GÃ©nÃ©rale|SOCIÃ©TÃ© GÃ©NÃ©RALE|sociã©tã© gã©nã©rale|American Express|AMERICAN EXPRESS|american express|Banco do Brasil|BANCO DO BRASIL|banco do brasil|Morgan Stanley|MORGAN STANLEY|morgan stanley|Prudential|PRUDENTIAL|prudential|CNOOC|cnooc|Rio Tinto|RIO TINTO|rio tinto|Unilever|UNILEVER|unilever|Nissan Motor|NISSAN MOTOR|nissan motor|Credit Suisse Group|CREDIT SUISSE GROUP|credit suisse group|E.ON|e.on|Generali Group|GENERALI GROUP|generali group|AmÃ©rica MÃ³vil|AMÃ©RICA MÃ³VIL|amã©rica mã³vil|GlaxoSmithKline|GLAXOSMITHKLINE|glaxosmithkline|Schlumberger|SCHLUMBERGER|schlumberger|BBVA-Banco Bilbao Vizcaya|BBVA-BANCO BILBAO VIZCAYA|bbva-banco bilbao vizcaya|Nordea Bank|NORDEA BANK|nordea bank|Bayer|BAYER|bayer|China Minsheng Banking|CHINA MINSHENG BANKING|china minsheng banking|Caterpillar|CATERPILLAR|caterpillar|Standard Chartered|STANDARD CHARTERED|standard chartered|China Shenhua Energy|CHINA SHENHUA ENERGY|china shenhua energy|US Bancorp|US BANCORP|us bancorp|Mitsubishi Corp|MITSUBISHI CORP|mitsubishi corp|Dow Chemical|DOW CHEMICAL|dow chemical|Ecopetrol|ECOPETROL|ecopetrol|Industrial Bank|INDUSTRIAL BANK|industrial bank|Shanghai Pudong Development|SHANGHAI PUDONG DEVELOPMENT|shanghai pudong development|EADS|eads|Swiss Re|SWISS RE|swiss re|Iberdrola|IBERDROLA|iberdrola|China Citic Bank|CHINA CITIC BANK|china citic bank|Reliance Industries|RELIANCE INDUSTRIES|reliance industries|Hutchison Whampoa|HUTCHISON WHAMPOA|hutchison whampoa|Home Depot|HOME DEPOT|home depot|Hitachi|HITACHI|hitachi|Hon Hai Precision|HON HAI PRECISION|hon hai precision|Phillips 66|PHILLIPS 66|phillips 66|Capital One Financial|CAPITAL ONE FINANCIAL|capital one financial|MÃ¸ller-Maersk|MÃ¸LLER-MAERSK|mã¸ller-maersk|Aviva|AVIVA|aviva|France Telecom|FRANCE TELECOM|france telecom|Deutsche Telekom|DEUTSCHE TELEKOM|deutsche telekom|Bank of Montreal|BANK OF MONTREAL|bank of montreal|Bank Montreal|VINCI|vinci|MondelÄ“z International|MONDELÄ“Z INTERNATIONAL|mondelä“z international|Suncor Energy|SUNCOR ENERGY|suncor energy|China Telecom|CHINA TELECOM|china telecom|Occidental Petroleum|OCCIDENTAL PETROLEUM|occidental petroleum|Twenty-First Century Fox, Inc.|TWENTY-FIRST CENTURY FOX, INC.|twenty-first century fox, inc.|Mitsui & Co|MITSUI & CO|mitsui & co|Mitsui Co|Qualcomm|QUALCOMM|qualcomm|State Bank of India|STATE BANK OF INDIA|state bank of india|State Bank India|Time Warner|TIME WARNER|time warner|Philip Morris International|PHILIP MORRIS INTERNATIONAL|philip morris international|Honeywell International|HONEYWELL INTERNATIONAL|honeywell international|Express Scripts|EXPRESS SCRIPTS|express scripts|United Parcel Service|UNITED PARCEL SERVICE|united parcel service|Barclays|BARCLAYS|barclays|PNC Financial Services|PNC FINANCIAL SERVICES|pnc financial services|Deutsche Post|DEUTSCHE POST|deutsche post|AIA Group|AIA GROUP|aia group|Duke Energy|DUKE ENERGY|duke energy|Delta Air Lines|DELTA AIR LINES|delta air lines|Deutsche Bank|DEUTSCHE BANK|deutsche bank|National Grid|NATIONAL GRID|national grid)[^[:alnum:]]%%'
                                   ) THEN 1 ELSE 0 END) as num_contains_company,
                                   SUM(CASE WHEN t.num_mentions > 0 THEN 1 ELSE 0 END) AS num_containing_mentions,
                                   SUM(CASE WHEN t.num_hashtags > 0 THEN 1 ELSE 0 END) AS num_containing_hashtags,
                                   SUM(CASE WHEN t.retweeted = TRUE THEN 1 ELSE 0 END) AS num_retweeted,
                                   SUM(CASE WHEN tweet_text SIMILAR TO '%%[^[:alnum:]](I|i|Me|me|We|we|Us|us)[^[:alnum:]]%%'
                                    THEN 1 ELSE 0 END)   AS first_person,
                                   SUM(CASE WHEN tweet_text SIMILAR TO '%%[^[:alnum:]](You|you)[^[:alnum:]]%%'
                                    THEN 1 ELSE 0 END)   AS second_person,
                                   SUM(CASE WHEN tweet_text SIMILAR TO '%%[^[:alnum:]](She|she|He|he|Her|her|Him|him|It|it|They|they|Them|them)[^[:alnum:]]%%'
                                    THEN 1 ELSE 0 END)   AS third_person
                                FROM tss_dev.tweets t
                                WHERE t.user_id_fk = %(user_id)s
                                GROUP BY t.user_id_fk;"""

# noinspection SqlNoDataSourceInspection,SqlDialectInspection
SELECT_MOST_COMMONLY_TWEETED_HOUR = """SELECT
                                          extract(HOUR FROM timestamp) AS tweet_hour,
                                          COUNT(*) AS num_tweets
                                        FROM tss_dev.tweets
                                        WHERE user_id_fk = %(user_id)s
                                        GROUP BY EXTRACT(HOUR FROM TIMESTAMP)
                                        ORDER BY COUNT(*) DESC
                                        LIMIT 1;"""

# noinspection SqlNoDataSourceInspection,SqlDialectInspection
SELECT_TWEET_COUNT_BY_DAY = """SELECT
                                  extract(DOW FROM timestamp) AS tweet_day,
                                  COUNT(*) AS num_tweets
                                FROM tss_dev.tweets
                                WHERE user_id_fk = %(user_id)s
                                GROUP BY extract(DOW FROM timestamp);"""

# noinspection SqlNoDataSourceInspection,SqlDialectInspection
SELECT_NUM_TWEETS_MULTIPLE_QUEST_EXCLAM = """SELECT COUNT(*) as tweet_count
                                             FROM tss_dev.tweets
                                             WHERE user_id_fk = %(user_id)s
                                                   AND (length(tweet_text) - length(regexp_replace(tweet_text, '\?', '', 'g')) > 1 OR
                                                        length(tweet_text) - length(regexp_replace(tweet_text, '\!', '', 'g')) > 1);"""

DAY_OF_WEEK_POSTGRES_MAPPING = {0: 'sun', 1: 'mon', 2: 'tues', 3: 'wed', 4: 'thur', 5: 'fri', 6: 'sat'}


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
            'timestamp': convertInputToTimestamp(inputData['created_at']),
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
            'timestamp': convertInputToTimestamp(inputData['created_at']),
            'updated': convertInputToTimestamp(inputData['updated']),
            'classification': inputData['classification']
            }


def stringNullCheck(input):
    if not input or input.strip() == 'NULL':
        return None

    return input.strip()


def convertInputToBool(input):
    if not input or input.strip() == 'NULL':
        return False

    return True if input.strip() == '1' else False


def convertInputToTimestamp(input):
    if not input or input.strip() == 'NULL':
        return None

    fmts = ["%Y-%m-%d %H:%M:%S", "%m/%d/%y %H:%M", "%m/%d/%y %H:%M:%S", "%a %b %d %H:%M:%S +0000 %Y", "%c"]

    for fmt in fmts:
        # time.sleep(1.0)
        try:
            return datetime.strptime(input.strip().strip('L'), fmt)
        except ValueError:
            pass

    # could be in millisecond epoch time
    try:
        return datetime.fromtimestamp(int(input.strip('L')) / 1000)
    except ValueError:
        pass

    print("Unable to type this datetime input {}".format(input.strip()))
    return None


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
        raise e

    return
