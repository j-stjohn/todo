# Todo List Application Notes

Inside this document I will create a working history of my todo application, any bugs found, and solutions presented to fix said bugs.

## Project Beginning

This project began as an exploration into writing text onto a file. I built a simiple typewriter-like program that allowed the user to enter a string. That string represented a line and the program would allow the user to save the lines written into a document.

I began modifiying the idea further into making an application that prompted the user to enter in tasks to be done and then stored in a text file.

## Version 1.0

With `Version 1.0` the user can create a task file, add tasks, add onto existing task files, and view those files.
This product is very much in the early stages and along the way I have run into many issues and solutions to those issues. I notice something that with each new thing added it can create 2-3 potential problems.

### Problems

1. Tasks are not saved by the program, only in the file
    - This was solved by creating a task array so that way the program can remember what tasks are which.

2. The user is unable to alter the text file beyond writing new tasks.
    - Added in edit and delete feature.
    - I had to learn about making the program not count out the full array
    - The new array would then be saved to the text file.
    - Learned to shift the values in the array then stop counting the end of the array.

3. When deleting a task, the ids have a habit of repeating.    
    - This methodology could be used for reassigning ids or we could have a global id counter.

## Version 1.1

### Additions
I added the ability to delete and edit tasks.

### Procedure

> To create `deletingTask()`, I had to run the entire task struct index by index looking for the user's input of which `tasks[i].id` they were looking for. Once the index is found, it is *"deleted"* by overwriting it with `task[i + 1]` and then stopping the iterations of the `for loop` one less time. The whole list of tasks is then outputed to the user and overwritten to the text file.

### Things I would like to change

1. Set up the todo list to operate in the same file the entire time, rather than opening and closing the file. This should be done before the menu appears.

2. Redo the task menu to ask the user to
    ```Terminal
    
    (w)rite tasks // Meaning write tasks or append if the file existed before starting the program
    (v)iew tasks 
    (c)omplete tasks
    (e)dit tasks // Allow them to delete or edit tasks
    (q)uit

    ```
