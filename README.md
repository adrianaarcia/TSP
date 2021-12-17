# TSP
Solution to the Traveling Salesperson Problem. Last Updated: 12/12/2019

## Introduction
The Travelling Salesperson Problem is the problem of, given a set of locations and the distances between every pair of them, determining the shortest tour that starts and ends at the same city and visits each other city exactly once. It is a very common problem in logistics: consider, for example, a delivery driver who needs to make stops at a list of 50 addresses and wants to minimize fuel use along the way.

## Overview
This program calculates routes between cities by various methods. The file containing the cities and their locations and the methods to use to calculate the routes should be given as command-line arguments. For each method, the output is the method used, the total distance, and the route.

The name of the file containing the cities and their geographic coordinates should be the first command-line argument. That file should contain the following (and nothing else):
* on the first line, the number of cities in a format readable by `atoi`;
* on the second line, a list of distinct three-character labels separated by single spaces (no label will contain a space or other whitespace character);
* one line per city containing the latitude and longitude of the city in degrees in a format readable by `atof` and separated by a single space.

The methods to construct the tours (should be given as the subsequent command-line arguments. They can contain any number of the following, in any order, and possibly with repeats:
* `-nearest`, in which case the route is calculated starting from the first city listed in the input file and proceeding to the closest remaining city by distance (with ties broken arbitrarily), then back to the first city.
* `-insert` followed by either `nearest` or `farthest`, in which case the route is constructed by starting with a 2-city tour containing only the nearest or farthest pair of cities, then repeatedly selecting the nearest/farthest remaining city to any city already in the tour (minimize/maximize over all pairs of cities (_i_,_j_) the distance from _i_ to _j_, where _i_ is a city not yet in the tour and _j_ is a city already in the tour). Once a city is selected, it is inserted at the point in the tour that minimizes the total length of the tour after that step. Ties are broken arbitrarily. Finally, the tour is reordered so that it starts and ends at the first city in the input.
* `-greedy` is implemented with an adjacency-list-based undirected graph. It first generates a list of all pairs of cities, sorts that list in order of increasing distance (breaking ties arbitrarily), and starts with an empty partial tour. Then  for each pair of cities, if they are not already connected by the partial tour and if each city in the pair is included in at most one other pair added previously, a segment is added between the two cities in the pair to the partial tour. 
 

### Output
When the input is valid, the output is written to standard output and consists of one line per method giving the method (including nearest or farthest for the -insert method), the total distance in kilometers, and the route, starting with the first city in the input and ending with that city again. Output for multiple methods is given in the order the methods appeared on the command line.

## Examples
```
$ ./TSP 8_cities.in -nearest -insert nearest -insert farthest
-nearest        :  10067.11 SEA YVR MCI STL DAL IAH DCA BWI SEA
-insert nearest :   9320.21 SEA YVR MCI STL DCA BWI IAH DAL SEA
-insert farthest:   9306.22 SEA YVR MCI STL BWI DCA IAH DAL SEA
```
Where the file 8_cities.in contains
```
8
SEA YVR MCI STL BWI DCA IAH DAL
47.450000 -122.311667
49.194722 -123.183889
39.297500 -94.713889
38.748611 -90.370000
39.175278 -76.668333
38.851944 -77.037778
29.984444 -95.341389
32.847222 -96.851667
```
```
$ ./TSP /Tests/8_cities.in -greedy
-greedy         :   9711.84 SEA YVR BWI DCA STL MCI DAL IAH SEA
```
```
$ for i in `seq 1 17`; do ./Unit $i; done
PASSED
PASSED
PASSED
PASSED
PASSED
PASSED
PASSED
PASSED
PASSED
PASSED
PASSED
PASSED
PASSED
PASSED
PASSED
PASSED
PASSED
```
