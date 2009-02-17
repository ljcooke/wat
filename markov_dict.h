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

#ifndef MARKOV_DICT_H
#define MARKOV_DICT_H 1

#include "markov_prefix.h"

class markov_dict
{
private:
    markov_prefix **data;      // hash table for prefixes
    unsigned int *data_count;  // size of each list in the table

    unsigned int pwords;      // number of words per group
    unsigned short hash_max;  // maximum hash value

    // These two variables keep track of which entries in the
    // hash table contain data. This is used for picking
    // random elements.
    unsigned int *data_table;
    unsigned int data_entries;
public:
    markov_dict(int prefix_words);
    ~markov_dict();

    void read_file(char *filename);  // add text from a file

    // Add a prefix and suffix
    void insert(string prefix[], string suffix);

    markov_prefix * lookup(string prefix);  // find a prefix
    markov_prefix * random();               // random prefix

    // Get the next prefix
    markov_prefix * next_prefix(string pre, string suf);

    unsigned short hash(string s);  // hash a string
};

#endif
//:mode=c++:
