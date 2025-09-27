# Learning Horn formulas
				   
## Table of contents

* [Brief Description](#brief-description)
* [Compilation and Installation](#compilation-and-installation)
* [Examples](#examples)


## Brief Description

The  C++  programs  implement  the  online and offline versions of the
algorithm  for  learning  a  Horn  formula  from positive and negative
samples.   Each sample is a tuple of positive integer values separated
by commas,  preceded  by an indicator (first value on the line).   The indicator is 0 for a negative and 1 for a positive sample.

### Invocation

The offline and online algorithms are invoked by the commands

	./offline <csv-file>
	./online  <csv-file>

To run both algorithms on all csv files, run the command
```bash
bash test.sh
```

## Compilation and Installation
* [Compilation](#compilation)
* [Execution](#execution)

### Compilation

A C++ compiler satisfying at least the C++11 revision is necessary to
successfully compile the algorithms. Only the standard C++ library is
used.  The g++ GNU Project compiler is used in the `Makefile`. If you
have a different compiler, please modify the *CXX* variable in the
`Makefile` according to your installation.

### Execution

To build the executables, use the command
```Makefile
   make
```
in the root directory. You can also compile just one of the algorithms
via `make offline` or `make online`.

To install the executables, use the command
```Makefile
   make install
```
This will rename the executables to `offline-horn` and `online-horn`, respectively, and copy them to the directory `/usr/local/bin`. You
need superuser priviledges to write to this folder. For installing
the binaries elsewhere, modify the *EXECUTABLES* variable accordingly.
If you have write permission and do not need the `sudo` command,
change the *SUDO* variable.

## Examples

The distribution also contains sample data in the following CSV files.

 - **accent-fr.csv**  (identifying English speaker with French accent)
 - **banknotes.csv**  (identifying forged banknotes)
 - **cancer.csv**     (identifying malignant tumors in breast cancer Wisconsin example)
 - **graph.csv**      (DSAA 2023 competition example)
 - **graphFT.csv**    (sorted **graph.csv** example, negative samples first)
 - **graphTF.csv**    (sorted **graph.csv** example, positive samples first)
 - **mushroom.csv**   (identifying edible mushrooms)
 - **shuttle.csv**    (identifying a bypass for the shuttle)

To run both algorithms on all CSV files, use the command

```bash
bash test.sh
```

Optionally, you can let the script check the output of the algorithms
using the Python program `check-horn`. This may take some time.

EOF
