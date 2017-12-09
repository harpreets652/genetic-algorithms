import numpy as np
import matplotlib.pyplot as plt


def gen_vs_fitness(file_name, title, fig_text):
    data = np.loadtxt(file_name)
    gen, min_val, avg_val, max_val = data[:, 0], data[:, 1], data[:, 2], data[:, 3]

    plt.plot(gen, max_val, 'b-', label="Max", linewidth=2.0)
    plt.plot(gen, avg_val, 'g-', label="Avg", linewidth=2.0)
    plt.plot(gen, min_val, 'r-', label="Min", linewidth=2.0)
    plt.legend(loc='best')

    plt.xlabel("GA Generation")
    plt.ylabel("Fitness (Percent Accuracy)")
    plt.title(title)
    plt.figtext(0.2, 0.009, fig_text)

    plt.show()

    return


def num_bits_vs_fitness(file_name, title, fig_text):
    data = np.loadtxt(file_name)
    gen, avg_val, avg_num_bits = data[:, 0], data[:, 2], data[:, 5]

    plt.plot(avg_num_bits, avg_val, 'g-', label="avg", linewidth=2.0)
    plt.legend(loc='best')

    plt.xlabel("Number of Features")
    plt.ylabel("Percent Accuracy")
    plt.title(title)
    plt.figtext(0.2, 0.009, fig_text)

    plt.show()

    return


gen_vs_fitness(
    "/Users/harpreetsingh/github/genetic-algorithms-submit/twitter-subset-selection/weka_temp/bayes/BayesianNet-0.010-0.950.tsv",
    "Bayes Network",
    "Bayes Network ML with 0.1 Mutation and 0.2 Crossover Probabilities")


num_bits_vs_fitness("/Users/harpreetsingh/github/genetic-algorithms-submit/twitter-subset-selection/weka_temp/bayes/BayesianNet-0.010-0.950_NSGA.tsv",
                    "Bayes Network Multiobjective (Max Accuracy, Min Features",
                    "Bayes Network ML with 0.1 Mutation and 0.2 Crossover Probabilities")