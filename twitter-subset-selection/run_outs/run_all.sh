#!/bin/bash

# 13203-hpcvis3
nohup ../build/TwitterSubsetSelection --wekaloc=../bin --data=../weka_temp/bayes            --machine=BAYES          > ../run_outs/bayes.out &
#nohup ../build/TwitterSubsetSelection --wekaloc=../bin --data=../weka_temp/part             --machine=PART           > ../run_outs/part.out &
#nohup ../build/TwitterSubsetSelection --wekaloc=../bin --data=../weka_temp/random_forest    --machine=RANDOM_FOREST  > ../run_outs/random_forest.out &
#nohup ../build/TwitterSubsetSelection --wekaloc=../bin --data=../weka_temp/random_tree      --machine=RANDOM_TREE    > ../run_outs/random_tree.out &
nohup ../build/TwitterSubsetSelection --wekaloc=../bin --data=../weka_temp/decision_tree    --machine=DECISION_TREE  > ../run_outs/decision_tree.out &
nohup ../build/TwitterSubsetSelection --wekaloc=../bin --data=../weka_temp/neural_net       --machine=NEURAL_NET     > ../run_outs/neural_net.out &
