# Twitter Feature Subset Selection
> Daniel Lopez and Harpreet Singh
Coursework for CS - 776 (Evolutionary Computing)
University of Nevada, Reno - Fall 2017

# Prerequisites
The following packages should be on your linux box:
```bash

cmake
default-jre #java
postgresql-client-common #*-9.5
libpqxx-dev
```

# Database Configuration
This project assumes that there's a corresponding PSQL database implemented and prefilled with data in a tabled named `tss_dev.users_features`.
For more information on the creation and design of the database, please see `genetic-algorithms-submit/twitter-client`.
In evaluator you will see the following line...


# Building
This project is built using `CMake`.
```bash
mkdir build && cd build
cmake .. && make
```

# Running
```bash
./main --wekaloc=<location of weka dir>
```

# Dependencies or External Libraries
Shoutout to adishavit for their library, [Argh](https://github.com/adishavit/argh)