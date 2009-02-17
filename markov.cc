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

#include <iostream>
#include <fstream>
#include <time.h>
#include "markov.h"
#include "markov_dict.h"

using namespace std;


/**
 * Markov: Master markov chain class.
 */
markov::markov(int prefix_size)
{
    srand(time(NULL));

    pwords = prefix_size;
    dict = new markov_dict(pwords);
}

markov::~markov()
{
    delete dict;
}


/**
 * Add text from a file to the dictionary.
 */
void markov::read_file(char *filename)
{
    dict->read_file(filename);
}


/**
 * Print a markov chain using the prefix and suffix data in the
 * dictionary. If a positive limit is specified, the algorithm will
 * attempt to generate this many words; however, it will end if it
 * finds a prefix with no suffix.
 */
string markov::random(int limit)
{
    markov_prefix *item;
    string pre, suf, out;

    if (limit <= 0) {
        limit = -1;
    } else {
        limit -= pwords;
        if (limit < 0) limit = 0;
    }

    // Start the output with a random prefix from the dictionary
    item = dict->random();
    if (item == NULL) return "";
    pre = item->prefix;
    out = pre;

    for (int i = 0; item != NULL; i++) {
        if (limit > 0 && i >= limit) {
            break;
        } else if (limit == -1) {
            /*
              TODO:
              - No limit defined - set the limit to the
                size of the file
            */
        }

        suf = item->random();  // get a random suffix
        out += " " + suf;

        // Find the next item based on the current
        // prefix and suffix
        item = dict->next_prefix(pre, suf);
        if (item != NULL)
            pre = item->prefix;
    }

    return out;
}
