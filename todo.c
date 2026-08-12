#include <stdio.h>
#include <string.h>

#define MAX_FILENAME 100
#define MAX_LINE 256
#define MAX_TASK 100

/* -------------------------------------------------------------------------- */
/* Menu utilities                                                             */
/* -------------------------------------------------------------------------- */

void clearScreen(void)
{
    printf("\033[1;1H\033[2J");
}

void displayMenu(void)
{
    clearScreen();
    printf("(w)rite a new list\n");
    printf("(o)pen an existing list\n");
    printf("(v)iew an existing list\n");
}

char getChoice(void)
{
    char choice[10];

    printf("Enter your choice: ");
    if (fgets(choice, sizeof(choice), stdin) == NULL) {
        return '\0';
    }
    choice[strcspn(choice, "\n")] = '\0';
    return choice[0];
}

void promptFilename(char *filename, size_t size)
{
    printf("Enter the name of the file: ");
    if (fgets(filename, size, stdin) == NULL) {
        filename[0] = '\0';
        return;
    }
    filename[strcspn(filename, "\n")] = '\0';
}

/* -------------------------------------------------------------------------- */
/* File helpers                                                               */
/* -------------------------------------------------------------------------- */

int countTasksInFile(const char *filename)
{
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL) {
        printf("Error opening file");
		return 0; /* No file means no tasks yet. */
    }

    int count = 0;
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fptr) != NULL) {
        count++;
    }
    fclose(fptr);
    return count;
}

int printFileContents(const char *filename)
{
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL) {
        return 1;
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fptr) != NULL) {
        printf("%s", line);
    }
    fclose(fptr);
    return 0;
}

/* -------------------------------------------------------------------------- */
/* Task operations                                                            */
/* -------------------------------------------------------------------------- */

int viewTasks(void)
{
	char filename[MAX_FILENAME];
    promptFilename(filename, sizeof(filename));
    if (filename[0] == '\0') {
        printf("No filename provided.\n");
        return 1;
    }

    int taskCount = countTasksInFile(filename);
    if (printFileContents(filename) != 0) {
        printf("Error opening file '%s'.\n", filename);
        return 1;
    }

    printf("Number of tasks in the file: %d\n", taskCount);
    return 0;
}

int appendTasks(void)
{
    char filename[MAX_FILENAME];
    promptFilename(filename, sizeof(filename));
    if (filename[0] == '\0') {
        printf("No filename provided.\n");
        return 1;
    }

    int nextTaskNumber = countTasksInFile(filename) + 1;
    FILE *fptr = fopen(filename, "a");
    if (fptr == NULL) {
        printf("Error opening file '%s'.\n", filename);
        return 1;
    }

    char task[MAX_TASK];
    printf("Enter task (press 'q' to quit)\n");
    while (1) {
        printf("Task #%d: ", nextTaskNumber);
        if (fgets(task, sizeof(task), stdin) == NULL) {
            break;
        }
        if (task[0] == 'q' && task[1] == '\n') {
            break;
        }

        fprintf(fptr, "Task #%d: ", nextTaskNumber);
        fputs(task, fptr);
        nextTaskNumber++;
    }

    fclose(fptr);
    return 0;
}

int writeTasks(void)
{
    char filename[MAX_FILENAME];
    promptFilename(filename, sizeof(filename));
    if (filename[0] == '\0') {
        printf("No filename provided.\n");
        return 1;
    }

    FILE *fptr = fopen(filename, "w");
    if (fptr == NULL) {
        printf("Error creating file '%s'.\n", filename);
        return 1;
    }

    char task[MAX_TASK];
    int taskNumber = 1;

    printf("Enter task (press 'q' to quit)\n");
    while (1) {
        printf("Task #%d: ", taskNumber);
        if (fgets(task, sizeof(task), stdin) == NULL) {
            break;
        }
        if (task[0] == 'q' && task[1] == '\n') {
            break;
        }

        fprintf(fptr, "Task #%d: ", taskNumber);
        fputs(task, fptr);
        taskNumber++;
    }

    fclose(fptr);
    return 0;
}

/* -------------------------------------------------------------------------- */
/* Program entry point                                                        */
/* -------------------------------------------------------------------------- */

int main(void)
{
    displayMenu();
    char choice = getChoice();

    while (choice == 'w') {
        return writeTasks();
    }
    while (choice == 'o') {
        return appendTasks();
    }
    while (choice == 'v') {
        return viewTasks();
    }

    printf("Invalid choice.\n");
    return 1;
}
