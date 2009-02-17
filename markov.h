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

#ifndef MARKOV_H
#define MARKOV_H 1

#include "markov_dict.h"

class markov
{
private:
    markov_dict *dict;    // prefix dictionary
    unsigned int pwords;  // number of words per prefix
public:
    markov(int prefix_size);
    ~markov();

    void read_file(char *filename);  // add text from a file
    string random(int limit);        // get a markov chain string
};

#endif
//:mode=c++:
