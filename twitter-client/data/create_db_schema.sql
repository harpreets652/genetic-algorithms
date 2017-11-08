\connect cs_776;

CREATE SCHEMA TSS_DEV AUTHORIZATION system;

\echo 'Creating FAKE_NEWS_TWEETS table IN TSS_DEV'
CREATE TABLE TSS_DEV.FAKE_NEWS_TWEETS (
  tweet_id                      BIGINT PRIMARY KEY,
  tweet_content                 JSONB      DEFAULT NULL,
  tweet_created_date            TIMESTAMP DEFAULT NULL,
  length_char                   SMALLINT  DEFAULT NULL,
  length_words                  SMALLINT  DEFAULT NULL,
  contains_question             BOOLEAN   DEFAULT NULL,
  contains_exclamation          BOOLEAN   DEFAULT NULL,
  contains_multiple_quest_exlam BOOLEAN   DEFAULT NULL,
  contains_emoticon_frown       BOOLEAN   DEFAULT NULL,
  contains_emoticion_smile      BOOLEAN   DEFAULT NULL,
  contains_pronoun_first_p      BOOLEAN   DEFAULT NULL,
  contains_pronoun_second_p     BOOLEAN   DEFAULT NULL,
  contains_pronoun_third_p      BOOLEAN   DEFAULT NULL,
  number_of_urls                SMALLINT  DEFAULT NULL,
  number_of_urls_top_100        SMALLINT  DEFAULT NULL,
  number_of_urls_top_1000       SMALLINT  DEFAULT NULL,
  number_of_urls_top_10000      SMALLINT  DEFAULT NULL,
  contains_user_mention         BOOLEAN   DEFAULT NULL,
  contains_hashtag              BOOLEAN   DEFAULT NULL,
  contains_stock_sym            BOOLEAN   DEFAULT NULL,
  is_retweeted                  BOOLEAN   DEFAULT NULL,
  sentiment_pos_words           SMALLINT  DEFAULT NULL,
  sentiment_neg_words           SMALLINT  DEFAULT NULL,
  sentiment_score               DECIMAL   DEFAULT NULL,
  user_verified                 BOOLEAN   DEFAULT NULL,
  user_age                      SMALLINT  DEFAULT NULL,
  user_status_count             INTEGER   DEFAULT NULL,
  user_num_followers            INTEGER   DEFAULT NULL,
  user_num_friends              INTEGER   DEFAULT NULL,
  user_has_description          BOOLEAN   DEFAULT NULL,
  user_has_url                  BOOLEAN   DEFAULT NULL,
  prop_degree_of_root           INTEGER   DEFAULT NULL,
  prop_size_max_subtree         INTEGER   DEFAULT NULL,
  prop_max_degree_non_root      INTEGER   DEFAULT NULL,
  prop_max_depth                INTEGER   DEFAULT NULL,
  prop_avg_depth                INTEGER   DEFAULT NULL,
  prop_max_level_size           INTEGER   DEFAULT NULL
);