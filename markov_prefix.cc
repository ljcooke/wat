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
#include <string>
#include "markov_prefix.h"

using namespace std;


/**
 * Markov prefix: Stores an n-word prefix and all suffixes
 * which can follow it.
 */
markov_prefix::markov_prefix(string p)
{
    prefix = p;
    next = NULL;

    suffix = NULL;
    count = 0;
}

markov_prefix::~markov_prefix()
{
    delete next;
    delete suffix;
}


/**
 * Add a suffix to the linked list.
 */
void markov_prefix::add_suffix(string s)
{
    markov_suffix *suf;

    if (suffix == NULL) {
        suffix = new markov_suffix;
        suf = suffix;
    } else {
        for (suf = suffix; suf != NULL; suf = suf->next) {
            if (s.compare(suf->word) == 0)
                return;
            if (suf->next == NULL)
                break;
        }
        suf->next = new markov_suffix;
        suf = suf->next;
    }

    suf->word = s;
    suf->next = NULL;
    count++;
}


/**
 * Get the number of suffix words.
 */
unsigned int markov_prefix::get_count()
{
    return count;
}


/**
 * Get a suffix word from a specified position in the list.
 */
markov_suffix * markov_prefix::lookup(int index)
{
    markov_suffix *s = suffix;

    if (index < 0 || index >= count)
        return NULL;

    for (int i = 0; i < index; i++)
        s = s->next;

    return s;
}


/**
 * Get a random suffix word.
 */
string markov_prefix::random()
{
    markov_suffix *s = lookup(rand() % count);
    return s->word;
}
