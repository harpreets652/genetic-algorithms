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
#define MOST_POPULAR_WEEKDAY        19
#define MOST_POPULAR_HOUR           20

// USER BASED FEATURES
#define USER_REGISTRATION_AGE       21
#define USER_STATUS_COUNT           22
#define USER_NUM_FOLLOWERS          23
#define USER_NUM_FRIENDS            24
#define USER_IS_VERIFIED            25
#define USER_HAS_DESCRIPTION        26
#define USER_HAS_URL                27



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
