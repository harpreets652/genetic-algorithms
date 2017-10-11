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
    np.savetxt(output_filename, average_of_runs, ['%d', '%.8f', '%.8f', '%.8f'])

    return


def runga(number_of_times, outputDirectory, tspFile):
    for i in range(number_of_times):
        args = ("/Users/harpreetsingh/github/genetic-algorithms/assignment4/cmake-build-debug/GeneticAlgorithm",
                tspFile,
                str(random.randrange(1, 1000)),
                outputDirectory + "run" + str(i + 1)
                )

        process = subprocess.Popen(args, stdout=subprocess.PIPE)
        process.wait()
        output = process.stdout.read()
        print output
        print "completed " + str(i)

    return


def quantitative_analysis(resultsDir, optimalTourDistance, numberOfGenerations, percentOptimalRunThreshold):
    averagesData = np.loadtxt(resultsDir + "averages.avg", float)
    avgMinTourLengthFound = 1.0 / averagesData[numberOfGenerations - 1][3]

    percentOfOptimalOfAMTL = (optimalTourDistance / avgMinTourLengthFound) * 100

    minTourLengthEachRun = []
    numOfGenToATML = []
    highestFitnessSoFar = 0.0
    bestFitFileName = ""
    numOfRunsWithinQuality = 0
    for file_name in glob.glob1(resultsDir, "*.data"):
        data = np.loadtxt(resultsDir + file_name, float)

        HighestFitnessOfRun = data[numberOfGenerations - 1][3]
        minTourLenOfRun = 1.0 / HighestFitnessOfRun
        percentOfOptOfRun = (optimalTourDistance / minTourLenOfRun) * 100

        # standard dev calc
        minTourLengthEachRun.append(minTourLenOfRun)

        # best fitness
        if (HighestFitnessOfRun > highestFitnessSoFar):
            highestFitnessSoFar = HighestFitnessOfRun
            bestFitFileName = file_name


        # number of runs within quality
        if (percentOfOptOfRun >= percentOptimalRunThreshold):
            numOfRunsWithinQuality += 1

        # how many generations it took to get to percentOptimal of AMTL
        if (minTourLenOfRun <= avgMinTourLengthFound):
            for gen in data:
                tourLen = 1.0 / gen[3]
                if (tourLen <= avgMinTourLengthFound):
                    numOfGenToATML.append(gen[0])
                    break

    standardDev = np.std(minTourLengthEachRun, dtype=float)
    avgNumOfGensToATML = np.average(numOfGenToATML)
    medOfNumOfGensToATML = np.median(numOfGenToATML)

    print "Optimal tour length = " + str(optimalTourDistance)
    print "Average minimal tour length found= " + str(avgMinTourLengthFound)
    print "Quality: optimal/avgMinTourLength = " + str(percentOfOptimalOfAMTL)
    print "Quality: standard deviation = " + str(standardDev)
    print "Quality: Run with best fitness = " + bestFitFileName
    print "Reliability: " + str((numOfRunsWithinQuality / 30.0) * 100.0) + "% of runs within " \
          + str(percentOptimalRunThreshold) + "% of optimal"
    print "Speed: Avg # of gens to avg-min-tour-length(runs reaching ATML) = " + str(avgNumOfGensToATML)
    print "Speed: Median of # of gens to avg-min-tour-len(runs reaching ATML) = " + str(medOfNumOfGensToATML)
    return


runga(30,
      "/Users/harpreetsingh/github/genetic-algorithms/assignment4/results/eil76/",
      "/Users/harpreetsingh/github/genetic-algorithms/assignment4/tspFiles/eil76.tsp"
      )

# todo: configure precision as well
averageoverfiles(1000,
                 2600,
                 "/Users/harpreetsingh/github/genetic-algorithms/assignment4/results/eil76/",
                 "/Users/harpreetsingh/github/genetic-algorithms/assignment4/results/eil76/averages.avg"
                 )

plotdata(
    "/Users/harpreetsingh/github/genetic-algorithms/assignment4/results/eil76/averages.avg",
    "eil76",
    "figure description",
    "y label"
)

# quantitative_analysis("/Users/harpreetsingh/github/genetic-algorithms/assignment4/results/berlin52/",
#                       7542,
#                       800,
#                       85)