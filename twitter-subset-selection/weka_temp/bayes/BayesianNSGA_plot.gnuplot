# Gnuplot script file for plotting data in file "force.dat"
      # This file is called   force.p
      set   autoscale                        # scale axes automatically
      unset log                              # remove any log-scaling
      unset label                            # remove any previous labels
      set xtic auto                          # set xtics automatically
      set ytic auto                          # set ytics automatically
      set xlabel "Iteration (Generation tick)"
      set ylabel "Percent Accuracy"
      #set key 0.01,100
      #set label "Yield Point" at 0.003,260
      #set arrow from 0.0028,250 to 0.003,280
      set yr [70:100]
      set xr [0:200]
      set terminal png size 600,600 enhanced font 'Verdana,10'

    set title "GA Evolution of Bayesian Network ML with 0.1 Mutation and 0.2 Crossover Probability"
    set output 'Bayesian_NSGA.png'
    plot "BayesianNet-0.001-0.200_NSGA.tsv" using 1:2 title 'Min' with linespoints , \
         "BayesianNet-0.001-0.200_NSGA.tsv" using 1:3 title 'Avg' with linespoints , \
         "BayesianNet-0.001-0.200_NSGA.tsv" using 1:4 title 'Max' with linespoints
