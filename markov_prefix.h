/*
 * wat -- a gibberish text generator
 * Copyright (c) 2007-2009 Liam Cooke
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MARKOV_PREFIX_H
#define MARKOV_PREFIX_H 1

#include <string>
using namespace std;

struct markov_suffix
{
    string word;
    struct markov_suffix *next;
};

class markov_prefix
{
private:
    struct markov_suffix *suffix;  // linked list of suffix words
    unsigned int count;            // number of words
public:
    markov_prefix(string p);
    ~markov_prefix();

    void add_suffix(string s);          // add a suffix
    unsigned int get_count();           // count suffixes
    markov_suffix * lookup(int index);  // get suffix at index
    string random();                    // get a random suffix

    string prefix;        // prefix string
    markov_prefix *next;  // next linked list element
};

#endif
//:mode=c++:
