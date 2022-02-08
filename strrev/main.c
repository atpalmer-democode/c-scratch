#include <stdio.h>
#include <string.h>

/*
 * Reverse a string in place.
 */
void string_reverse(char *s)
{
    char *back = s + strlen(s) - 1;
    char *front = s;

    while (front < back) {
        char tmp = *front;
        *front = *back;
        *back = tmp;
        ++front;
        --back;
    }
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "%s: Please provide a string argument\n", *argv);
        return -1;
    }
    char *s = argv[1];

    printf("Before: %s\n", s);
    string_reverse(s);
    printf("After: %s\n", s);
}
