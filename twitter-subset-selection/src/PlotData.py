import numpy as np
import matplotlib.pyplot as plt
import csv
from collections import namedtuple

NsgaIndividual = namedtuple("NsgaIndividual", "Id Chrom Accur NumBits Time")


def plot_gen_vs_fitness(file_name, title, fig_text):
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

def plot_pareto_front(file_name, title, fig_text):
    nsga_data = load_nsga_data(file_name)

    non_dominated_front = get_non_dominated_list(nsga_data)

    accuracy = []
    num_bits = []
    for individual in non_dominated_front:
        accuracy.append(individual.Accur)
        num_bits.append(individual.NumBits)

    plt.plot(num_bits, accuracy, 'g-', linewidth=2.0)
    plt.scatter(num_bits, accuracy)
    plt.legend(loc='best')

    plt.xlabel("Number of Features")
    plt.ylabel("Percent Accuracy")
    plt.title(title)
    plt.figtext(0.2, 0.009, fig_text)

    plt.show()

    return

# todo: need to implement to show max fitness for multiple ml models
def max_fitness_vs_gen_multi_file():
    return


def load_nsga_data(file_name):
    nsga_data = []

    with open(file_name) as inFile:
        user_reader = csv.DictReader(inFile)
        for line in user_reader:
            data = NsgaIndividual(int(line['Id']),
                                  line['Chrom'],
                                  float(line['Accuracy']),
                                  int(line['NumBits']),
                                  float(line['Time']))

            nsga_data.append(data)

    return nsga_data


def get_non_dominated_list(nsga_data):
    nondominated_front = []

    for current in nsga_data:
        num_dominate_me = 0

        for other in nsga_data:
            if current.Chrom == other.Chrom:
                continue

            if pareto_dominates(other, current):
                num_dominate_me += 1

        if num_dominate_me == 0:
            nondominated_front.append(current)

    return nondominated_front


# if this dominates opponent
def pareto_dominates(this, opponent):
    return (not (this.Accur < opponent.Accur)) and (not (this.NumBits > opponent.NumBits))


def pareto_front(file_name, title, fig_text):
    nsga_data = load_nsga_data(file_name)

    non_dominated_front = get_non_dominated_list(nsga_data)
    print (len(non_dominated_front))

    accuracy = []
    num_bits = []
    for individual in non_dominated_front:
        accuracy.append(individual.Accur)
        num_bits.append(individual.NumBits)

    plt.plot(num_bits, accuracy, 'g-', linewidth=2.0)
    plt.scatter(num_bits, accuracy)
    plt.legend(loc='best')

    plt.xlabel("Number of Features")
    plt.ylabel("Percent Accuracy")
    plt.title(title)
    plt.figtext(0.2, 0.009, fig_text)

    plt.show()

    return


# gen_vs_fitness(
#     "/Users/harpreetsingh/github/genetic-algorithms-submit/twitter-subset-selection/weka_temp/bayes/BayesianNet-0.010-0.950.tsv",
#     "Bayes Network",
#     "Bayes Network ML with 0.1 Mutation and 0.2 Crossover Probabilities")

plot_pareto_front(
    "/Users/harpreetsingh/github/genetic-algorithms-submit/twitter-subset-selection/weka_temp/bayes/BayesianNet_NSGA_LastGen.tsv",
    "Bayes Network Multiobjective Max(Accuracy), Min(Features)",
    "Bayes Network ML with 0.1 Mutation and 0.2 Crossover Probabilities")
