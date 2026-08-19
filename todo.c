#include <stdio.h>
#include <string.h>

#define MAX_FILENAME 100
#define MAX_LINE 256
#define MAX_TASK 100
#define TASK_ID 1000

/* -------------------------------------------------------------------------- */
/* Struct Task                                                                */
/* -------------------------------------------------------------------------- */

struct Task {
    int id;
    char description[MAX_LINE];
    int completed;
};

struct Task tasks[MAX_TASK];
int taskCount = 0;


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
    printf("(o)pen and append an existing list\n");
    printf("(v)iew an existing list\n");
    printf("(q)uit\n");
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

/*
 * Loads tasks from a file into the tasks[] array.
 *
 * File format:
 *
 * Task id: 1000    0    Wash car
 * Task id: 1001    1    Change oil
 *
 * 0 = incomplete
 * 1 = complete
 */
int loadTasksFromFile(const char *filename)
{
    FILE *fptr = fopen(filename, "r");

    if (fptr == NULL) {
        return 0;
    }

    taskCount = 0;

    while (taskCount < MAX_TASK)
    {
        char line[MAX_LINE];

        if (fgets(line, sizeof(line), fptr) == NULL) {
            break;
        }

        if (sscanf(line,
                   "Task id: %d %d %[^\n]",
                   &tasks[taskCount].id,
                   &tasks[taskCount].completed,
                   tasks[taskCount].description) == 3)
        {
            taskCount++;
        }
    }

    fclose(fptr);

    return taskCount;
}


/*
 * Saves every task currently in the tasks[] array to a file.
 */
int saveTasksToFile(const char *filename)
{
    FILE *fptr = fopen(filename, "w");

    if (fptr == NULL) {
        return 1;
    }

    for (int i = 0; i < taskCount; i++)
    {
        fprintf(fptr,
                "Task id: %d\t%d\t%s\n",
                tasks[i].id,
                tasks[i].completed,
                tasks[i].description);
    }

    fclose(fptr);

    return 0;
}


/* -------------------------------------------------------------------------- */
/* Task operations                                                            */
/* -------------------------------------------------------------------------- */

int markComplete(void)
{
    char choice[10];

    for (int i = 0; i < taskCount; i++)
    {
        printf("Mark task %d: complete? (y/n): ",
               tasks[i].id);

        if (fgets(choice, sizeof(choice), stdin) == NULL) {
            return 1;
        }

        if (choice[0] == 'y' || choice[0] == 'Y')
        {
            tasks[i].completed = 1;
        }

        if (choice[0] == 'n' || choice[0] == 'N')
        {
            tasks[i].completed = 0;
        }

        if (choice[0] == 'q' || choice[0] == 'Q')
        {
            break;
        }
    }

    return 0;
}


int viewTasks(void)
{
    char filename[MAX_FILENAME];

    promptFilename(filename, sizeof(filename));

    if (filename[0] == '\0') {
        printf("No filename provided.\n");
        return 1;
    }

    int count = loadTasksFromFile(filename);

    if (count == 0) {
        printf("No tasks found.\n");
        printf("Press Enter to continue...");
        getchar();
        return 0;
    }

    printf("\n--- TASK LIST ---\n\n");

    for (int i = 0; i < taskCount; i++)
    {
        printf("Task ID: %d\t[%d] %s\n",
               tasks[i].id,
               tasks[i].completed,
               tasks[i].description);
    }

    printf("\nNumber of tasks: %d\n", taskCount);

    printf("Press Enter to continue or press 'c' to mark completed tasks... ");

    char choice[10];

    if (fgets(choice, sizeof(choice), stdin) == NULL) {
        return 1;
    }

    choice[strcspn(choice, "\n")] = '\0';

    if (choice[0] == 'c' || choice[0] == 'C')
    {
        markComplete();

        /*
         * Save the updated task array back to the same file.
         */
        if (saveTasksToFile(filename) != 0)
        {
            printf("Error saving file '%s'.\n", filename);
            printf("Press Enter to continue...");
            getchar();
            return 1;
        }

        /*
         * Display the updated list.
         */
        printf("\n--- UPDATED TASK LIST ---\n\n");

        for (int i = 0; i < taskCount; i++)
        {
            printf("Task ID: %d\t[%d] %s\n",
                   tasks[i].id,
                   tasks[i].completed,
                   tasks[i].description);
        }

        printf("\nTasks saved.\n");
        printf("Press Enter to return to menu...");
        getchar();
    }

    return 0;
}


/* -------------------------------------------------------------------------- */
/* Write Tasks                                                                */
/* -------------------------------------------------------------------------- */

int writeTasks(void)
{
    char filename[MAX_FILENAME];

    promptFilename(filename, sizeof(filename));

    if (filename[0] == '\0') {
        printf("No filename provided.\n");
        return 1;
    }

    /*
     * We are creating a brand-new list,
     * so start with an empty array.
     */
    taskCount = 0;

    printf("\nEnter task (press 'q' to quit)\n\n");

    while (taskCount < MAX_TASK)
    {
        tasks[taskCount].id = TASK_ID + taskCount;

        printf("Task id: %d\t",
               tasks[taskCount].id);

        if (fgets(tasks[taskCount].description,
                  sizeof(tasks[taskCount].description),
                  stdin) == NULL)
        {
            break;
        }

        if (tasks[taskCount].description[0] == 'q' &&
            tasks[taskCount].description[1] == '\n')
        {
            break;
        }

        tasks[taskCount].completed = 0;

        taskCount++;
    }

    if (saveTasksToFile(filename) != 0)
    {
        printf("Error creating file '%s'.\n", filename);
        return 1;
    }

    printf("\nSaved %d tasks.\n", taskCount);

    return 0;
}


/* -------------------------------------------------------------------------- */
/* Append Tasks                                                               */
/* -------------------------------------------------------------------------- */

int appendTasks(void)
{
    char filename[MAX_FILENAME];

    promptFilename(filename, sizeof(filename));

    if (filename[0] == '\0') {
        printf("No filename provided.\n");
        return 1;
    }

    /*
     * Load the existing tasks into our array first.
     */
    int existingTasks = loadTasksFromFile(filename);

    if (existingTasks == MAX_TASK)
    {
        printf("Task array is already full.\n");
        return 1;
    }

    printf("\nExisting tasks: %d\n", taskCount);
    printf("Enter task (press 'q' to quit)\n\n");

    while (taskCount < MAX_TASK)
    {
        tasks[taskCount].id = TASK_ID + taskCount;

        printf("Task id: %d\t",
               tasks[taskCount].id);

        if (fgets(tasks[taskCount].description,
                  sizeof(tasks[taskCount].description),
                  stdin) == NULL)
        {
            break;
        }

        if (tasks[taskCount].description[0] == 'q' &&
            tasks[taskCount].description[1] == '\n')
        {
            break;
        }

        tasks[taskCount].completed = 0;

        taskCount++;
    }

    if (saveTasksToFile(filename) != 0)
    {
        printf("Error saving file '%s'.\n", filename);
        return 1;
    }

    printf("\nSaved %d total tasks.\n", taskCount);

    return 0;
}


/* -------------------------------------------------------------------------- */
/* Program entry point                                                        */
/* -------------------------------------------------------------------------- */

int main(void)
{
    char choice;

    while (1)
    {
        displayMenu();

        choice = getChoice();

        switch (choice)
        {
            case 'w':
                writeTasks();
                break;

            case 'o':
                appendTasks();
                break;

            case 'v':
                viewTasks();
                break;

            case 'q':
                return 0;

            default:
                printf("Invalid choice.\n");
                break;
        }
    }

    return 0;
}
