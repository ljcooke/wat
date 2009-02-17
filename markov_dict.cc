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
#include <string>
#include "markov_dict.h"
#include "markov_prefix.h"

using namespace std;


/**
 * Markov dictionary: Stores and controls groups of n-word prefixes.
 */
markov_dict::markov_dict(int prefix_words)
{
    if (prefix_words < 1)
        prefix_words = 1;
    else if (prefix_words > 5)
        prefix_words = 5;
    pwords = prefix_words;

    // Determine the highest hash value - this must fit inside
    // a short int, but cannot be greater than RAND_MAX.
    hash_max = (unsigned short)(-1);
    if (RAND_MAX < (unsigned int)hash_max)
        hash_max = RAND_MAX;

    // Initialise the hash table
    data = new markov_prefix*[hash_max];
    data_count = new unsigned int[hash_max];
    data_table = new unsigned int[hash_max];
    for (int i = 0; i < hash_max; i++) {
        data_count[i] = 0;
        data_table[i] = 0;
    }
    data_entries = 0;
}

markov_dict::~markov_dict()
{
    delete[] data;
    delete data_count;
    delete data_table;
}


/**
 * Add a group of prefixes and a suffix to the dictionary
 */
void markov_dict::insert(string prefix[], string suffix)
{
    // Only insert if there are enough prefix words.
    if (prefix[0].length() <= 0) return;

    string s = "";
    for (int i = 0; i < pwords; i++) {
        if (i > 0) s += " ";
        s += prefix[i];
    }

    markov_prefix *item = lookup(s);
    unsigned short h = hash(s);
    if (item == NULL) {
        if (data_count[h] == 0) {
            // Create a new linked list entry
            data[h] = new markov_prefix(s);
            item = data[h];
            data_table[data_entries++] = h;
        } else {
            // Add this item to a linked list
            item = data[h];
            while (item->next != NULL) {
                item = item->next;
            }
            item->next = new markov_prefix(s);
            item = item->next;
        }

        data_count[h]++;
    }
    item->add_suffix(suffix);
}


/**
 * Parse a file for groups of prefixes and suffixes and
 * add them to the dictionary.
 */
void markov_dict::read_file(char *filename)
{
    ifstream in;                   // input stream
    string pre[pwords], suf = "";  // prefix array & suffix
    char c, c_next;                // input characters
    bool token = false, wspace = false;

    in.open(filename);
    if (!in || in.bad()) {
        cerr << "file not found: " << filename << endl;
        return;
    }

    c = in.get();
    while (!in.eof()) {
        c_next = in.get();
        wspace = (c==' ' || c=='\t' || c=='\n' || c=='\r');
        token = (!wspace || token);

        if (!wspace) suf += c;

        // If this is the end of a token, insert the prefixes and
        // suffix into the dictionary
        if ((wspace && token) || (!wspace && in.eof())) {
            insert(pre, suf);

            for (int j = 1; j < pwords; j++)  // shift left
                pre[j-1] = pre[j];
            pre[pwords-1] = suf;
            suf = "";

            token = false;
        }
        c = c_next;
    }
    in.close();
}


/**
 * Search the hash table for a prefix string.
 */
markov_prefix * markov_dict::lookup(string prefix)
{
    unsigned short h = hash(prefix);
    markov_prefix *p;

    if (data_count[h] == 0)
        return NULL;

    for (int i = 0; i < data_count[h]; i++) {
        if (i == 0) p = data[h];
        else p = p->next;

        if (p->prefix == prefix)
            return p;
    }

    return NULL;
}


/**
 * Get a random prefix from the dictionary.
 */
markov_prefix * markov_dict::random()
{
    unsigned int d, p;
    markov_prefix *pre = NULL;

    if (data_entries > 0) {
        // Pick a random list in the hash table
        d = data_table[rand() % data_entries];

        // Now pick a random element from this list
        p = rand() % data_count[d];
        pre = data[d];
        for (int i = 1; i < data_count[d]; i++) {
            pre = pre->next;
        }
    }

    return pre;
}


/**
 * Get the prefix string which follows a given prefix and suffix.
 */
markov_prefix * markov_dict::next_prefix(string pre, string suf)
{
    string s = pre + " " + suf;
    int e = s.find_first_of(' ', 0) + 1;

    if (e < 0 || e >= s.length())
        return NULL;

    s.erase(0, e);

    return lookup(s);
}


/**
 * Get the hash value of a string.
 */
unsigned short markov_dict::hash(string s)
{
#ifdef DEBUG
    // Use a small hash value when testing for collisions
    return (s.at(0) / 4);
#else
    unsigned short hash = 11, len = s.length();
    for (int i = 0; i < len; i++) {
        hash *= 97;
        hash += s.at(i);

        while (hash > hash_max) {
            hash -= hash_max;
        }
    }

    return hash;
#endif
}
