#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count_escapees(const char *htmltext) {
    int count = 0;
    for (int i = 0; i < strlen(htmltext); i++) {
        if (htmltext[i] == '<' || htmltext[i] == '>') {
            count += 1;
        }
    }
    return count;
}

void doescape(const char *htmltext, char *expandedtext) {
    int j = 0;
    for (int i = 0; i < strlen(htmltext); i++) {
        if (htmltext[i] == '<') {
            strcpy(&expandedtext[j], "&lt;");
            j += 4;
        } else if (htmltext[i] == '>') {
            strcpy(&expandedtext[j], "&gt;");
            j += 4;
        } else {
            expandedtext[j] = htmltext[i];
            j += 1;
        }
    }
}

char *escapehtml(const char *htmltext) {
    int count = count_escapees(htmltext);
    int origlen = strlen(htmltext);
    int expandedlen = origlen + count * 4 + 1;
    char *expandedtext = malloc(sizeof(char) * expandedlen);
    doescape(htmltext, expandedtext);
    return expandedtext;
}

int main() {
    const char *orig = "<a href=\"badurl\">a link!</a>";
    char *escaped = escapehtml(orig);
    printf("Original: %s\n", orig);
    printf("Escaped: %s\n", escaped);
    free(escaped);
    return 0;
}