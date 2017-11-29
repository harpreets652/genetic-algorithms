# Gnuplot script file for plotting data in file "force.dat"
      # This file is called   force.p
      set   autoscale                        # scale axes automatically
      unset log                              # remove any log-scaling
      unset label                            # remove any previous labels
      set xtic auto                          # set xtics automatically
      set ytic auto                          # set ytics automatically
      set xlabel "Iteration (Generation tick)"
      set ylabel "Fitness"
      #set key 0.01,100
      #set label "Yield Point" at 0.003,260
      #set arrow from 0.0028,250 to 0.003,280
      set yr [70:100]
      set xr [0:200]
      set terminal png size 600,600 enhanced font 'Verdana,10'

    set title "GA Evolution of Decision Table ML with 0.001 Mutation Probability and 0.67 Crossover Probability"
    set output 'DecisionTable_1.png'
    plot "DecisionTable-0.001-0.670.tsv" using 1:2 title 'Min' with linespoints , \
         "DecisionTable-0.001-0.670.tsv" using 1:3 title 'Avg' with linespoints , \
         "DecisionTable-0.001-0.670.tsv" using 1:4 title 'Max' with linespoints
