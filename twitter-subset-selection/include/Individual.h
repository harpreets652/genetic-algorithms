#ifndef __INDIVIDUAL_H_
#define __INDIVIDUAL_H_

#include <vector>
#include <cstdlib>
#include <iostream>
#include <cmath>

class Evaluator;

#include "Evaluator.h"
#include "Utils.h"

using namespace std;

// MESSAGE BASED FEATURES
#define AVG_LEN_TWEET_CHARACTERS    0
#define AVG_LEN_TWEET_WORDS         1
#define AVG_NUM_POSITIVE_WORDS      2
#define AVG_NUM_NEGATIVE_WORDS      3
#define AVG_SENTIMENT_SCORE         4

// PROPORTIONAL BASED FEATURES
#define FRAC_CONTAINS_QUESTION      5
#define FRAC_CONTAINS_EXCLAMATION   6
#define FRAC_CONTAINS_MULT_Q_E      7
#define FRAC_CONTAINS_EMO_SMILE     8
#define FRAC_CONTAINS_EMO_FROWN     9
#define FRAC_CONTAINS_PRONOUN_1     10
#define FRAC_CONTAINS_PRONOUN_2     11
#define FRAC_CONTAINS_PRONOUN_3     12
#define FRAC_CONTAINS_URLS          13
#define FRAC_CONTAINS_URLS_TOP_100  14
#define FRAC_CONTAINS_MENTION_USER  15
#define FRAC_CONTAINS_HASHTAG       16
#define FRAC_CONTAINS_STOCK_SYM     17
#define FRAC_ARE_RETWEET            18
#define FRAC_PUB_ON_MON             19
#define FRAC_PUB_ON_TUES            20
#define FRAC_PUB_ON_WED             21
#define FRAC_PUB_ON_THUR            22
#define FRAC_PUB_ON_FRI             23
#define FRAC_PUB_ON_SAT             24
#define FRAC_PUB_ON_SUN             25
#define MOST_POPULAR_WEEKDAY        26
#define MOST_POPULAR_HOUR           27

// USER BASED FEATURES
#define USER_REGISTRATION_AGE       28
#define USER_STATUS_COUNT           29
#define USER_NUM_FOLLOWERS          30
#define USER_NUM_FRIENDS            31
#define USER_IS_VERIFIED            32
#define USER_HAS_DESCRIPTION        33
#define USER_HAS_URL                34
#define USER_DEF_PROFILE_PHOTO      35
#define USER_DE

const map<unsigned int, string> FEATURE_STRINGS = {
        {0, ""},
        {1, "1"}
};

class Individual : public vector<bool> {
public:
    Individual();

    void generateRandom();
    void print() const;
    void init();
    void evaluate();
    string to_string() const;
    //vector<double> getAsValues() const;

    void crossoverWith(Individual& other);
    void mutate();

    double distance;
    double fitness;
    double normalizedProb;

    double diffDistance;
    double diffFitness;

private:
    //double getBinaryAsNumber(vector<bool> subset) const;
};


#endif
