// ----------------------------
// projects/collatz/Collatz.c++
// Copyright (C) 2015
// Glenn P. Downing
// ----------------------------

// -------
// defines
// -------

// toggle cache on and off
#define CACHE 1

// --------
// includes
// --------

#include <cassert>  // assert
#include <iostream> // endl, istream, ostream
#include <sstream>  // istringstream
#include <string>   // getline, string
#include <utility>  // make_pair, pair
#include <climits>	// INT_MAX

#include "Collatz.h"

using namespace std;

#ifdef CACHE
/**
 * Array cache of calculated cycle lengths from 1 to 1000000
 */
static int cycle_lengths[1000000];
#endif

// ------------
// collatz_read
// ------------

pair<int, int> collatz_read (const string& s) {
    istringstream sin(s);
    int i;
    int j;
    sin >> i >> j;
    return make_pair(i, j);}

// ------------
// cycle_length
// ------------

int cycle_length (int n) {
	assert(n > 0);
#ifdef CACHE
	// check if cycle length of n is already in cache
	if (cycle_lengths[n])
		return cycle_lengths[n];
#endif
    int count = 1;
    int m = n;
    while (m > 1) {
    	if (m % 2 == 1) {
    		// m is odd
    		if (m > (INT_MAX - 1)/3)
    			// causes overflow, should be invalid input
    			return -1;	// produces an invalid output
    		m = 3*m + 1;
    	}
    	else
    		// m is even
    		m = m/2;
    	++count;
    }
    assert(count > 0);
#ifdef CACHE
    // save cycle length of n in cache
    cycle_lengths[n] = count;
#endif
    return count;}

// ------------
// collatz_eval
// ------------

int collatz_eval (int i, int j) {
	assert(i > 0);
	assert(j >= i);
    int max = 1;
    // optimization, top half of range cycle lengths > bottom half
    if (i < (j/2 + 1))
    	i = j/2 + 1;
    for (int k = i; k <= j; k++) {
    		int cycle = cycle_length(k);
    		if (cycle > max)
    			max = cycle;
    }
    if (max != -1)	// as long as input isn't entirely invalid and overflowing
    	assert(max > 0);
    return max;}

// -------------
// collatz_print
// -------------

void collatz_print (ostream& w, int i, int j, int v) {
    w << i << " " << j << " " << v << endl;}

// -------------
// collatz_solve
// -------------

void collatz_solve (istream& r, ostream& w) {
    string s;
    while (getline(r, s)) {
        const pair<int, int> p = collatz_read(s);
        const int            i = p.first;
        const int            j = p.second;
        // pass to collatz_eval so first number < second to avoid duplicate code
        const int            v = (i < j) ? collatz_eval(i, j) : collatz_eval(j, i);
        collatz_print(w, i, j, v);}}
