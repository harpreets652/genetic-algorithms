import glob
import subprocess
import random
import numpy as np
import matplotlib.pyplot as plt


def plotdata(filename, title, figText, ylabel):
    data = np.loadtxt(filename)
    gen, min_val, avg_val, max_val = data[:, 0], data[:, 1], data[:, 2], data[:, 3]

    plt.plot(gen, max_val, 'r-', label="max", linewidth=2.0)
    plt.plot(gen, avg_val, 'b-', label="avg", linewidth=2.0)
    plt.plot(gen, min_val, 'g-', label="min", linewidth=2.0)
    plt.legend(loc='best')

    plt.xlabel("Number of Evaluations")
    plt.ylabel(ylabel)
    plt.title(title)
    plt.figtext(0.3, 0.009, figText)

    plt.show()
    return


def averageoverfiles(num_of_generations, population_size, input_data_directory, output_filename):
    summation_of_runs = np.zeros((num_of_generations, 4), float)
    num_Of_Files = 0

    for file_name in glob.glob1(input_data_directory, "*.data"):
        data = np.loadtxt(input_data_directory + file_name, float)
        num_Of_Files += 1

        for i in range(num_of_generations):
            for j in range(4):
                summation_of_runs[i][j] += data[i][j]

    average_of_runs = summation_of_runs / num_Of_Files

    average_of_runs[:, 0] *= population_size

    # todo: change precision as needed
    np.savetxt(output_filename, average_of_runs, ['%d', '%.2f', '%.2f', '%.2f'])

    return


def runga(number_of_times, outputDirectory):
    for i in range(number_of_times):
        args = ("/Users/harpreetsingh/github/genetic-algorithms/assignment3/cmake-build-debug/GeneticAlgorithm",
                str(random.randrange(1, 1000)),
                outputDirectory + "run" + str(i + 1)
                )

        process = subprocess.Popen(args, stdout=subprocess.PIPE)
        process.wait()
        output = process.stdout.read()
        print output

    return


runga(30,
      "/Users/harpreetsingh/github/genetic-algorithms/assignment3/results/DeJongFunctionFour/LoCrossoverLoMutation/"
      )

#todo: configure precision as well
averageoverfiles(60,
                 80,
                 "/Users/harpreetsingh/github/genetic-algorithms/assignment3/results/DeJongFunctionFour/LoCrossoverLoMutation/",
                 "/Users/harpreetsingh/github/genetic-algorithms/assignment3/results/DeJongFunctionFour/LoCrossoverLoMutation/averages.data"
                 )

plotdata(
    "/Users/harpreetsingh/github/genetic-algorithms/assignment3/results/DeJongFunctionFour/LoCrossoverLoMutation/averages.data",
    "title",
    "figure description",
    "y label"
)
