#ifndef __CONFIG_CPP_
#define __CONFIG_CPP_

#include <Utils.h>
#include <Evaluator.h>
#include "config.h"

Config::Config() {
    getFSMap();
    getTypeMap();
}

Config &Config::operator=(const Config &other) {
    if (this == &other) {
        return *this;
    }
    this->PROB_MUTATION = other.PROB_MUTATION;
    this->PROB_CROSSOVER = other.PROB_CROSSOVER;
    this->INPUT_FILENAME = other.INPUT_FILENAME;
    return *this;
}

void Config::setClassifier(string classifier) {
    Utils::to_upper(classifier);
    if (classifier == "0" ||
            classifier == "BAYES" ||
            classifier == "BAYES_NETWORK") {
        WEKA_CLASSIFIER = BAYES_NET;
    } else if (classifier == "1" ||
            classifier == "PART") {
        WEKA_CLASSIFIER = PART;
    } else if (classifier == "2" ||
            classifier == "RANDOM_FOREST" ||
            classifier == "R_FOREST" ||
            classifier == "R_F") {
        WEKA_CLASSIFIER = RANDOM_FOREST;
    } else if (classifier == "3" ||
            classifier == "RANDOM_TREE" ||
            classifier == "R_TREE" ||
            classifier == "R_T") {
        WEKA_CLASSIFIER = RANDOM_TREE;
    } else if (classifier == "4" ||
            classifier == "DECISION_TABLE" ||
            classifier == "D_TABLE" ||
            classifier == "D_T") {
        WEKA_CLASSIFIER = DECISION_TABLE;
    } else if (classifier == "5" ||
            classifier == "NEURAL_NET" ||
            classifier == "NN" ||
            classifier == "MULTILAYER_PERCEPTRON") {
        WEKA_CLASSIFIER = NEURAL_NETWORK;
    }
}

string Config::getWEKAClassifierName() const {
    switch (WEKA_CLASSIFIER) {
        case BAYES_NET:
            return "weka.classifiers.bayes.BayesNet";
        case PART:
            return "weka.classifiers.rules.PART";
        case ZERO_R:
            return "weka.classifiers.rules.ZeroR";
        case RANDOM_FOREST:
            return "weka.classifiers.trees.RandomForest";
        case RANDOM_TREE:
            return "weka.classifiers.trees.RandomTree";
        case DECISION_TABLE:
            return "weka.classifiers.rules.DecisionTable";
        case NEURAL_NETWORK:
            return "weka.classifiers.functions.MultilayerPerceptron";
    }
}

string Config::getSimpleWEKAName() const {
    switch (WEKA_CLASSIFIER) {
        case BAYES_NET:
            return "BayesianNet";
        case PART:
            return "Part";
        case ZERO_R:
            return "ZeroR";
        case RANDOM_FOREST:
            return "RandomForest";
        case RANDOM_TREE:
            return "RandomTree";
        case DECISION_TABLE:
            return "DecisionTable";
        case NEURAL_NETWORK:
            return "MultilayerPerceptron";
    }
}

string Config::getOutputFilename() const {
    char buffer[200];
    snprintf(buffer, 200, "%s/%s-%.3f-%.3f", Evaluator::getInstance()->getDataLocation().c_str(),
             getSimpleWEKAName().c_str(), PROB_MUTATION, PROB_CROSSOVER);
    return string(buffer);
}

map<unsigned int, string> Config::getFSMap() {
    if (!FEATURE_STRINGS.empty()) {
        return FEATURE_STRINGS;
    }
    map<unsigned int, string> m;
    m[AVG_LEN_TWEET_CHARACTERS] = "avg_length_chars";
    m[AVG_LEN_TWEET_WORDS] = "avg_length_words";
    m[AVG_NUM_POSITIVE_WORDS] = "avg_sentiment_pos_words";
    m[AVG_NUM_NEGATIVE_WORDS] = "avg_sentiment_neg_words";
    m[AVG_SENTIMENT_SCORE] = "avg_sentiment_score";
    m[AVG_NUM_URLS] = "avg_number_of_urls";

    m[FRAC_ARE_RETWEET] = "fract_retweeted";
    m[FRAC_ARE_SELF_PROMOTING] = "fract_self_promoting";

    m[FRAC_FROM_API_SOURCE] = "fract_source_unpopular";
    m[FRAC_FROM_API_SOURCE_W_URL] = "fract_source_unpop_w_url";

    m[FRAC_CONTAINS_QUESTION] = "fract_contains_question";
    m[FRAC_CONTAINS_EXCLAMATION] = "fract_contains_exclamation";
    m[FRAC_CONTAINS_MULT_QU_EXC] = "fract_contains_multiple_quest_exlam";
    m[FRAC_CONTAINS_EMOTICON] = "fract_contains_emoticon";

    m[FRAC_CONTAINS_PRONOUN_1] = "fract_contains_pronoun_first_p";
    m[FRAC_CONTAINS_PRONOUN_2] = "fract_contains_pronoun_second_p";
    m[FRAC_CONTAINS_PRONOUN_3] = "fract_contains_pronoun_third_p";
    m[FRAC_CONTAINS_URLS] = "fract_contains_urls";
    m[FRAC_CONTAINS_URLS_TOP_100] = "fract_urls_top_100";
    m[FRAC_CONTAINS_USER_MENTION] = "fract_contains_user_mention";
    m[FRAC_CONTAINS_HASHTAG] = "fract_contains_hashtag";
    m[FRAC_CONTAINS_COMPANY] = "fract_contains_company";

    m[NUM_PUB_ON_SUN] = "num_tweets_day_sun";
    m[NUM_PUB_ON_MON] = "num_tweets_day_mon";
    m[NUM_PUB_ON_TUES] = "num_tweets_day_tues";
    m[NUM_PUB_ON_WED] = "num_tweets_day_wed";
    m[NUM_PUB_ON_THUR] = "num_tweets_day_thur";
    m[NUM_PUB_ON_FRI] = "num_tweets_day_fri";
    m[NUM_PUB_ON_SAT] = "num_tweets_day_sat";

    m[MOST_POPULAR_HOUR] = "most_commonly_tweeted_hour";

    m[USER_REGISTRATION_AGE] = "user_age";
    m[USER_STATUS_COUNT] = "user_status_count";
    m[USER_NUM_FOLLOWERS] = "user_num_followers";
    m[USER_NUM_FRIENDS] = "user_num_friends";
    m[USER_IS_VERIFIED] = "user_verified";
    m[USER_HAS_DESCRIPTION] = "user_has_description";
    m[USER_HAS_URL] = "user_has_url";
    m[USER_DEF_PROFILE_PHOTO] = "user_has_default_profile_image";
    m[USER_REPUTATION_RATIO] = "user_reputation_ratio";
    m[USER_FOLLOWERS_TO_LIFETIME] = "user_followers_to_lifetime";
    FEATURE_STRINGS = m;
    return FEATURE_STRINGS;
}

map<unsigned int, string> Config::getTypeMap() {
    if (!FEATURE_TYPE_STRINGS.empty()) {
        return FEATURE_TYPE_STRINGS;
    }
    for (unsigned int i = 0; i < config.NUM_FEATURES; i++) {
        switch (i) {
            case USER_HAS_DESCRIPTION:
            case USER_HAS_URL:
            case USER_DEF_PROFILE_PHOTO:
            case USER_IS_VERIFIED:
                FEATURE_TYPE_STRINGS[i] = "boolean";
                break;
            case AVG_NUM_URLS:
            case USER_REGISTRATION_AGE:
            case USER_STATUS_COUNT:
            case USER_NUM_FOLLOWERS:
            case USER_NUM_FRIENDS:
                FEATURE_TYPE_STRINGS[i] = "numeric";
                break;
            case MOST_POPULAR_HOUR:
            case NUM_PUB_ON_SUN:
            case NUM_PUB_ON_MON:
            case NUM_PUB_ON_TUES:
            case NUM_PUB_ON_WED:
            case NUM_PUB_ON_THUR:
            case NUM_PUB_ON_FRI:
            case NUM_PUB_ON_SAT:
                FEATURE_TYPE_STRINGS[i] = "integer";
                break;
            default:
                FEATURE_TYPE_STRINGS[i] = "real";
                break;
        }
    }
    return FEATURE_TYPE_STRINGS;
}

#endif