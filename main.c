#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 10
#define MAX_INPUT 1024

typedef struct {
    char **lines;
    int count;
    int capacity;
} Document;

/* Initialize document */
void initDocument(Document *doc)
{
    doc->lines = malloc(INITIAL_CAPACITY * sizeof(char *));

    if (doc->lines == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    doc->count = 0;
    doc->capacity = INITIAL_CAPACITY;
}

/* Free all memory */
void freeDocument(Document *doc)
{
    for (int i = 0; i < doc->count; i++) {
        free(doc->lines[i]);
    }

    free(doc->lines);
}

/* Make a copy of a string */
char *duplicateString(const char *text)
{
    char *copy = malloc(strlen(text) + 1);

    if (copy == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    strcpy(copy, text);

    return copy;
}

/* Increase array size when needed */
void ensureCapacity(Document *doc)
{
    if (doc->count < doc->capacity) {
        return;
    }

    int newCapacity = doc->capacity * 2;

    char **newLines = realloc(
        doc->lines,
        newCapacity * sizeof(char *)
    );

    if (newLines == NULL) {
        printf("Could not increase document capacity.\n");
        exit(1);
    }

    doc->lines = newLines;
    doc->capacity = newCapacity;
}

/* INSERT A LINE */
void insertLine(Document *doc, int lineNumber, const char *text)
{
    if (lineNumber < 1 || lineNumber > doc->count + 1) {
        printf("Error: Invalid line number.\n");
        printf("Valid range: 1 to %d\n", doc->count + 1);
        return;
    }

    ensureCapacity(doc);

    int index = lineNumber - 1;

    /* Shift lines down */
    for (int i = doc->count; i > index; i--) {
        doc->lines[i] = doc->lines[i - 1];
    }

    /* Add new line */
    doc->lines[index] = duplicateString(text);

    doc->count++;

    printf("Line inserted successfully.\n");
}

/* DELETE A LINE */
void deleteLine(Document *doc, int lineNumber)
{
    if (lineNumber < 1 || lineNumber > doc->count) {
        printf("Error: Invalid line number.\n");

        if (doc->count == 0) {
            printf("The document is empty.\n");
        } else {
            printf("Valid range: 1 to %d\n", doc->count);
        }

        return;
    }

    int index = lineNumber - 1;

    /* Free selected line */
    free(doc->lines[index]);

    /* Shift lines up */
    for (int i = index; i < doc->count - 1; i++) {
        doc->lines[i] = doc->lines[i + 1];
    }

    doc->count--;

    printf("Line deleted successfully.\n");
}

/* DISPLAY DOCUMENT */
void displayDocument(const Document *doc)
{
    if (doc->count == 0) {
        printf("\n[Document is empty]\n");
        return;
    }

    printf("\n----- DOCUMENT -----\n");

    for (int i = 0; i < doc->count; i++) {
        printf("%d: %s\n", i + 1, doc->lines[i]);
    }

    printf("--------------------\n");
}

/* SAVE FILE */
void saveFile(const Document *doc, const char *filename)
{
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        printf("Error: Could not open file.\n");
        return;
    }

    for (int i = 0; i < doc->count; i++) {
        fprintf(file, "%s\n", doc->lines[i]);
    }

    fclose(file);

    printf("Document saved to '%s'.\n", filename);
}

/* CLEAR DOCUMENT */
void clearDocument(Document *doc)
{
    for (int i = 0; i < doc->count; i++) {
        free(doc->lines[i]);
    }

    doc->count = 0;
}

/* LOAD FILE */
void loadFile(Document *doc, const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("Could not open '%s'.\n", filename);
        return;
    }

    clearDocument(doc);

    char buffer[MAX_INPUT];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {

        /* Remove newline */
        buffer[strcspn(buffer, "\n")] = '\0';

        ensureCapacity(doc);

        doc->lines[doc->count] = duplicateString(buffer);

        doc->count++;
    }

    fclose(file);

    printf("Document loaded from '%s'.\n", filename);
}

/* SEARCH */
void searchDocument(const Document *doc, const char *word)
{
    if (doc->count == 0) {
        printf("The document is empty.\n");
        return;
    }

    int found = 0;

    for (int i = 0; i < doc->count; i++) {

        if (strstr(doc->lines[i], word) != NULL) {

            printf("Found on line %d: %s\n",
                   i + 1,
                   doc->lines[i]);

            found = 1;
        }
    }

    if (!found) {
        printf("No matches found.\n");
    }
}

/* HELP */
void showHelp(void)
{
    printf("\n========== LINE EDITOR HELP ==========\n");

    printf("insert <line> <text>\n");
    printf("    Example: insert 1 Hello World\n\n");

    printf("delete <line>\n");
    printf("    Example: delete 2\n\n");

    printf("display\n");
    printf("    Display the document\n\n");

    printf("save <filename>\n");
    printf("    Example: save document.txt\n\n");

    printf("load <filename>\n");
    printf("    Example: load document.txt\n\n");

    printf("search <word or phrase>\n");
    printf("    Example: search hello\n\n");

    printf("help\n");
    printf("    Display this help\n\n");

    printf("quit\n");
    printf("    Exit the editor\n");

    printf("======================================\n");
}

/* MAIN PROGRAM */
int main(void)
{
    Document doc;

    initDocument(&doc);

    char input[MAX_INPUT];

    printf("=====================================\n");
    printf("        SIMPLE C LINE EDITOR         \n");
    printf("=====================================\n");

    printf("Type 'help' to see commands.\n");

    while (1) {

        printf("\n> ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        /* Remove newline */
        input[strcspn(input, "\n")] = '\0';

        /* Ignore empty input */
        if (strlen(input) == 0) {
            continue;
        }

        /* QUIT */
        if (strcmp(input, "quit") == 0 ||
            strcmp(input, "exit") == 0) {

            printf("Goodbye!\n");
            break;
        }

        /* HELP */
        if (strcmp(input, "help") == 0) {
            showHelp();
            continue;
        }

        /* DISPLAY */
        if (strcmp(input, "display") == 0) {
            displayDocument(&doc);
            continue;
        }

        /* INSERT */
        if (strncmp(input, "insert ", 7) == 0) {

            int lineNumber;
            char text[MAX_INPUT];

            if (sscanf(input + 7, "%d %[^\n]",
                       &lineNumber, text) == 2) {

                insertLine(&doc, lineNumber, text);

            } else {
                printf("Usage: insert <line> <text>\n");
            }

            continue;
        }

        /* DELETE */
        if (strncmp(input, "delete ", 7) == 0) {

            int lineNumber;

            if (sscanf(input + 7, "%d", &lineNumber) == 1) {

                deleteLine(&doc, lineNumber);

            } else {
                printf("Usage: delete <line>\n");
            }

            continue;
        }

        /* SAVE */
        if (strncmp(input, "save ", 5) == 0) {

            char filename[256];

            if (sscanf(input + 5, "%255s", filename) == 1) {

                saveFile(&doc, filename);

            } else {
                printf("Usage: save <filename>\n");
            }

            continue;
        }

        /* LOAD */
        if (strncmp(input, "load ", 5) == 0) {

            char filename[256];

            if (sscanf(input + 5, "%255s", filename) == 1) {

                loadFile(&doc, filename);

            } else {
                printf("Usage: load <filename>\n");
            }

            continue;
        }

        /* SEARCH */
        if (strncmp(input, "search ", 7) == 0) {

            char word[MAX_INPUT];

            if (sscanf(input + 7, "%[^\n]", word) == 1) {

                searchDocument(&doc, word);

            } else {
                printf("Usage: search <word or phrase>\n");
            }

            continue;
        }

        /* UNKNOWN COMMAND */
        printf("Unknown command. Type 'help'.\n");
    }

    freeDocument(&doc);

    return 0;
}