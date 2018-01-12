import subprocess
"--wekaloc=../bin --data=../temp --machine=J48 --comparison=SVG"

machines = [
    # "BAYES",
    # "PART",
    # "RANDOM_FOREST",
    # "RANDOM_TREE",
    # "DECISION_TABLE",
    # "NEURAL_NET",
    "SVM_LINEAR",
    "SVM_RBF",
    "J48"
]
comparisons = [
    "TVG",
    "SVG",
    # "MVG",
    # "FVG",
    # "TVSVG"
]

def execute(cmd):
    print ("executing '{}'".format(cmd))
    print ("error code:", subprocess.call(cmd, shell = True))

for m in machines:
    for c in comparisons:
        config = "../cmake-build-debug/TwitterSubsetSelection --wekaloc=../bin --data=../temp --machine={} --comparison={}".format(m, c)
        outputDir = "../run_outs/{}".format(c)
        outputFile = "{}/{}.out".format(outputDir, m)

        mkdir_command = "mkdir -p {}".format(outputDir)
        tss_command = "{} > {}".format(config, outputFile)

        execute(mkdir_command)
        execute(tss_command)
