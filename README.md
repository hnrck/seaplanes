[![GitHub version](https://img.shields.io/badge/version-1.1.0-green.svg)](https://github.com/hnrck/seaplanes/releases/tag/1.1.0)
[![GitHub issues](https://img.shields.io/github/issues/hnrck/seaplanes.svg)](https://github.com/hnrck/seaplanes/issues)
[![GitHub forks](https://img.shields.io/github/forks/hnrck/seaplanes.svg)](https://github.com/hnrck/seaplanes/network)
[![GitHub stars](https://img.shields.io/github/stars/hnrck/seaplanes.svg)](https://github.com/hnrck/seaplanes/stargazers)
[![GitHub license](https://img.shields.io/github/license/hnrck/seaplanes)](https://github.com/hnrck/seaplanes/blob/master/LICENSE)

# SEAPLANES 

Scheduling of a Cyber-physcal system simulation -- Simulation Execution Architecture - Partition-based Logical processor Allocator Node with Extensible inline Scheduler

## Description

![seaplanes_pic](/res/seaplanes.png)

SEAPLANES is a distributed simulation framework using the CERTI implementation of the High-Level Architecture (HLA) to schedule and interconnect models in a simulation.

This project was used as part of a CIFRE PhD thesis :mortar_board: involving ISAE-SUPAERO and Airbus to illustrate the simulation scheduling of cyber-physical systems.

Requires CERTI >= 4.0

Links :link: to the case study, and some publications, are available below.

## Table of Contents
* [1. Usage](#1-usage)
  * [1.1. Cloning this repo](#11-cloning-this-repo)
  * [1.2. Generating the documentation](#12-generating-the-documentation)
  * [1.3. Building and using the project](#13-building-and-using-the-project)
* [2. Related work](#2-related-work)
* [3. Project structure](#3-project-structure)
* [4. Contributing](#4-contributing)
* [5. Credits](#5-credits)
* [6. License](#6-license)

## 1. Usage

### 1.1. Cloning this repo

This repo is available at :link: [hnrck/seaplanes](https://github.com/hnrck/seaplanes).

Cloning can be done in HTTPS mode:
```bash
$ git clone https://github.com/hnrck/seaplanes
$ git checkout tags/1.1.0
```

or with ssh mode:
```bash
$ git clone git@github.com:hnrck/seaplanes.git
$ git checkout tags/1.1.0
```

### 1.2. Generating the documentation
```bash
$ make doc
```
The documentation can be opened in a browser, or generated in PDF from LaTeX sources.

For browser based navigation, open ````build/doc/html/index.html````
For generating PDF, do:

```bash
$ make -C build/doc/latex
```

Generated PDF will be ```build/doc/latex/refman.pdf```

### 1.3. Building and using the project

In order to build the project, use the following command:

```bash
$ make 
```

Project can be install with:

```bash
$ make install
```

By default, the installation is local.
Installed headers and libraries can be used sourcing the local ```seaplanespathsrc```: ```install/etc/seaplanes/seaplanespathsrc```
This can be done using:
```bash
$ source install/etc/seaplanes/seaplanespathsrc
```

Absolute path can be deduced and sourced directly before use, for instance in dev environment in any .{bash,zsh,...}rc.
```bash
$ source <path/to/seaplanes>/install/etc/seaplanes/seaplanespathsrc
```

Furthermore, while installing, the absolute path of the ```seaplanespathsrc``` is given:
```bash
...
-- Installing: <path/to/seaplanes>/install/etc/seaplanes/seaplanespathsrc
```

After installation and sourcing, ```seaplanes``` library can be used as any library.
Compilation using ````seaplanes```` library might need linking, using ```-lseaplanes```.

### 1.5. Other targets

Cleaning the repo:

```bash
make clean
```

Static check on source code (requires clang-tidy):

```bash
make lint
```

Formatting of source code (requires clang-format):

```bash
make format
```

### 1.6. Alternative builds and configurations

It is possible to directly use ```cmake``` to build the project, feel free to based your command on the targets in the ```Makefile```

Furthermore, compilation options are possible from the environment, set the following variable in you shell or before your command:
```bash
$ export <VAR>=<my option>
$ make
```
or
```bash
$ <VAR>=<my option> make
```

| option | VAR | default value | alternatives |
|--------|-----|---------------|--------------|
| C++ compiler | CXX | clang++ | g++, ... |
| Generator | GENERATOR | Ninja | Unix Makefile, ...
| Build type | BUILD_TYPE | RelWithDebInfo | Release, Debug, MinSizeRel |
| Build directory | BUILD_DIR | build |  anywhere |
| Install directory | INSTALL_DIR | install |  anywhere |

## 2. Related work

* Publication of RROSACE case study using SEAPLANES  :books: available at [SEAPLANES paper](https://svn.onera.fr/schedmcore/branches/ROSACE_CaseStudy/redundant/report_redundant_rosace_matlab.pdf)

* Publications using SEAPLANES :books::
  * [Toward a formalism to study the scheduling of cyber-physical systems simulations](https://scholar.google.fr/scholar?oi=bibs&hl=fr&cluster=8309967183591919001)
  * [Coincidence Problem in CPS Simulations: the R-ROSACE Case Study](https://scholar.google.fr/scholar?oi=bibs&hl=fr&cluster=10404789473361905467)
  * [Implementation of a Cyber-Physical Systems simulation components allocation tool](https://scholar.google.fr/scholar?oi=bibs&hl=fr&cluster=809749533240704673)

## 3. Project structure
```bash
.
├── ChangeLog.md
├── cmake-extra-modules
│   └── *.cmake
├── CMakeLists.txt
├── doc
│   └── Doxyfile.in
├── include
│   └── *.h
├── LICENSE
├── Makefile
├── README.md
├── res
│   └── *
├── src
│   └── *.cpp
├── templates
│   └── *.tcc
└── VERSION
```

Important folders:
- ```include``` - the seaplanes headers.
- ```src``` - the sources of the seaplanes library.

Important files:
- ```README.md``` - this file.
- ```LICENSE``` - the license file.
- ```VERSION``` - the current version of the library.
- ```ChangeLog.md``` - list of changes between revisions.
- ```CMakeLists.txt``` - the project configuration file
- ```Makefile``` - helper for compilation and installation.


## 4. Contributing
Contributions are welcome :unlock:. Feel free to fork too :fork_and_knife:.

## 5. Credits
- Henrick Deschamps  ([:octocat: hnrck](https://github.com/hnrck) ) ([:globe_with_meridians: hnrck.io](https://hnrck.io))

## 6. License
This work is under the [MIT License](https://github.com/hnrck/seaplanes/blob/master/LICENSE). :copyright: Henrick Deschamps, 2019.
