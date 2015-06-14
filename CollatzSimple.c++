// ----------------------------
// projects/collatz/Collatz.c++
// Copyright (C) 2015
// Glenn P. Downing
// ----------------------------

// --------
// includes
// --------

#include <cassert>  // assert
#include <iostream> // endl, istream, ostream
#include <sstream>  // istringstream
#include <string>   // getline, string
#include <utility>  // make_pair, pair
#include <climits>	// INT_MAX

#include "CollatzSimple.h"

using namespace std;

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
// collatz_eval
// ------------

int collatz_eval (int i, int j) {
	assert(i > 0);
	assert(j >= i);
    int max = 1;
    for (int k = i; k <= j; k++) {
    		int cycle = cycle_length(k);
    		if (cycle > max)
    			max = cycle;
    }
    if (max != -1)	// as long as input isn't entirely invalid and overflowing
    	assert(max > 0);
    return max;}

// ------------
// cycle_length
// ------------

int cycle_length (int n) {
	assert(n > 0);
    int count = 1;
    while (n > 1) {
    	if (n % 2 == 1) {
    		// n is odd
    		if (n > (INT_MAX - 1)/3)
    			// causes overflow, should be invalid input
    			return -1;	// produces an invalid output
    		n = 3*n + 1;
    	}
    	else
    		// n is even
    		n = n/2;
    	++count;
    }
    assert(count > 0);
    return count;}


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
