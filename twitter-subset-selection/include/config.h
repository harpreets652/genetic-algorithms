#ifndef __CONFIG_H_
#define __CONFIG_H_

#include <cstdlib>
#include <string>
#include <iostream>
#include <map>

using namespace std;

#define BREAKINGPOINT char dummychar; cin >> dummychar;

enum MLType {
    BAYES_NET,
    PART,
    ZERO_R,
    RANDOM_FOREST,
    RANDOM_TREE,
    DECISION_TABLE,
    NEURAL_NETWORK
};

struct Config {

    Config() = default;
    Config& operator=(const Config& other);

    void setClassifier(string classifier);
    double PROB_MUTATION = 0.1; // 0.01, 0.001, 0.0001
    double PROB_CROSSOVER = 0.2;  // 0.2, 0.67, 0.99

    string INPUT_FILENAME;
    MLType WEKA_CLASSIFIER = DECISION_TABLE;

    string getWEKAClassifierName() const;
    string getSimpleWEKAName() const;
    string getOutputFilename() const;

    unsigned int NUM_FEATURES = 40;
    unsigned int POPULATION_SIZE = 100;
    unsigned int ITERATION_SIZE = 200;
    unsigned int TOTAL_GAS_SIZE = 30; // 5 for testing, 30 for reality

    map<unsigned int, string> getFSMap();
    map<unsigned int, string> getTypeMap();
    map<unsigned int, string> FEATURE_STRINGS;
    map<unsigned int, string> FEATURE_TYPE_STRINGS;
};

extern Config config;

// MESSAGE BASED FEATURES
#define AVG_LEN_TWEET_CHARACTERS    0
#define AVG_LEN_TWEET_WORDS         1
#define AVG_NUM_POSITIVE_WORDS      2
#define AVG_NUM_NEGATIVE_WORDS      3
#define AVG_SENTIMENT_SCORE         4

// PROPORTIONAL BASED FEATURES
#define FRAC_CONTAINS_QUESTION      5
#define FRAC_CONTAINS_EXCLAMATION   6
#define FRAC_CONTAINS_MULT_QU_EXC   7
#define FRAC_CONTAINS_EMOTICON      8
#define AVG_NUM_URLS                9
#define FRAC_CONTAINS_PRONOUN_1     10
#define FRAC_CONTAINS_PRONOUN_2     11
#define FRAC_CONTAINS_PRONOUN_3     12
#define FRAC_CONTAINS_URLS          13
#define FRAC_CONTAINS_URLS_TOP_100  14
#define FRAC_CONTAINS_USER_MENTION  15
#define FRAC_CONTAINS_HASHTAG       16
#define FRAC_CONTAINS_COMPANY       17
#define FRAC_ARE_RETWEET            18
#define NUM_PUB_ON_MON              19
#define NUM_PUB_ON_TUES             20
#define NUM_PUB_ON_WED              21
#define NUM_PUB_ON_THUR             22
#define NUM_PUB_ON_FRI              23
#define NUM_PUB_ON_SAT              24
#define NUM_PUB_ON_SUN              25
#define MOST_POPULAR_HOUR           26
#define FRAC_FROM_API_SOURCE        27
#define FRAC_FROM_API_SOURCE_W_URL  28
#define FRAC_ARE_SELF_PROMOTING     29


// USER BASED FEATURES
#define USER_REGISTRATION_AGE       30
#define USER_STATUS_COUNT           31
#define USER_NUM_FOLLOWERS          32
#define USER_NUM_FRIENDS            33
#define USER_IS_VERIFIED            34
#define USER_HAS_DESCRIPTION        35
#define USER_HAS_URL                36
#define USER_DEF_PROFILE_PHOTO      37
#define USER_REPUTATION_RATIO       38
#define USER_FOLLOWERS_TO_LIFETIME  39

#endif
