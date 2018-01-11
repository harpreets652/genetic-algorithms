#!/bin/bash

#echo "now working social v genuine on bayes"
#../cmake-build-debug/TwitterSubsetSelection --wekaloc=../bin --data=../weka_social_temp/bayes            --machine=BAYES          > ../run_outs/final_expanded/bayes.out
echo "now working on part"
../cmake-build-debug/TwitterSubsetSelection --wekaloc=../bin --data=../weka_social_temp/part             --machine=PART           > ../run_outs/final_expanded/part.out
#echo "now working social v genuine on random_forest"
#../cmake-build-debug/TwitterSubsetSelection --wekaloc=../bin --data=../weka_social_temp/random_forest    --machine=RANDOM_FOREST  > ../run_outs/final_expanded/random_forest.out
#echo "now working social v genuine on random_tree"
#../cmake-build-debug/TwitterSubsetSelection --wekaloc=../bin --data=../weka_social_temp/random_tree      --machine=RANDOM_TREE    > ../run_outs/final_expanded/random_tree.out
echo "now working on decision_tree"
../cmake-build-debug/TwitterSubsetSelection --wekaloc=../bin --data=../weka_social_temp/decision_tree    --machine=DECISION_TREE  > ../run_outs/final_expanded/decision_tree.out
echo "now working on neural_net"
../cmake-build-debug/TwitterSubsetSelection --wekaloc=../bin --data=../weka_social_temp/neural_net       --machine=NEURAL_NET     > ../run_outs/final_expanded/neural_net.out
