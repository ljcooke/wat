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
#include "markov.h"

void help_me(char *progname);
int param_int(char *param, unsigned int skip);


int main(int argc, char **argv)
{
    char *filename = NULL;  // source file
    int pwords = 3,         // prefix words
        limit = 0,          // maximum words to output
        tmp = -1;           // temporary value
    bool ignore = false;    // ignore the next parameter

    // Read command-line arguments in the form '-option value'.
    // The space is optional (e.g. -p3 sets the number of prefix
    // words to 3). The first parameter which doesn't begin with
    // a hyphen is assumed to be the filename.
    for (int i = 1; i < argc; i++) {
        if (ignore) {
            ignore = false;
            continue;
        }

        if (strcmp(argv[i], "--help") == 0) {
            help_me(argv[0]);
        } else if (argv[i][0] == '-' && argv[i][1] != '-') {
            // Get the value associated with the
            // parameter if available
            switch (argv[i][1]) {
            case 'l':
            case 'p':
                if (argv[i][2] == '\0') {
                    tmp = param_int(argv[i+1], 0);
                    ignore = true;
                } else {
                    tmp = param_int(argv[i], 2);
                }
            }
            switch (argv[i][1]) {

            // -h: Print help text and exit
            case 'h':
                if (argv[i][2] == '\0') help_me(argv[0]);
                break;

            // -l: Limit the number of words outputted
            case 'l':
                if (tmp != -1) limit = tmp;
                break;

            // -p: Prefix words per group
            case 'p':
                if (tmp != -1) pwords = tmp;
                break;
            }
        } else {
            if (filename == NULL)
                filename = argv[i];
        }
    }

    // Load the file into the markov class and
    // print some random text.
    if (filename != NULL) {
        markov *m = new markov(pwords);
        m->read_file(filename);
        cout << m->random(limit) << endl;
    } else {
        help_me(argv[0]);
    }
}


/**
 * Program usage information
 */
void help_me(char *progname)
{
    cout << "usage:\t" << progname << " " << "[options] filename"
         << endl << "\t-l num     : "
         << "limit the number of words outputted (default: 0)"
         << endl << "\t-p num     : "
         << "words per prefix (1-5, default: 3)"
         << endl << "\t-h, --help : "
         << "display this message and exit" << endl;
    exit(0);
}


/**
 * Get an int from a command-line parameter
 */
int param_int(char *param, unsigned int skip)
{
    int val = 0;

    if (param == NULL) return -1;
    for (int i = skip; param[i] != '\0'; i++) {
        if (param[i] >= 0x30 && param[i] <= 0x39) {
            val *= 10;
            val += param[i] - 0x30;
        } else {
            return -1;
        }
    }

    return val;
}
