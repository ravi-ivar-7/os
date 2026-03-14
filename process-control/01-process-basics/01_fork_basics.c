/**
 * Exercise 1: Understanding fork()
 *
 * Learn:
 * - How fork() creates a new process
 * - Parent vs child execution
 * - Return values of fork()
 * - Memory separation between processes
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void demonstrate_fork_basics() {
    printf("=== Fork Basics ===\n");
    printf("Before fork: PID = %d\n", getpid());

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        printf("CHILD:  PID = %d, Parent PID = %d, fork() returned = %d\n",
               getpid(), getppid(), pid);
    } else {
        // Parent process
        printf("PARENT: PID = %d, fork() returned = %d (child's PID)\n",
               getpid(), pid);
        wait(NULL);  // Wait for child to finish
    }
}

void demonstrate_memory_separation() {
    printf("\n=== Memory Separation ===\n");

    int shared_var = 100;
    printf("Before fork: shared_var = %d at address %p\n", shared_var, &shared_var);

    pid_t pid = fork();

    if (pid == 0) {
        // Child modifies variable
        shared_var = 999;
        printf("CHILD:  shared_var = %d at address %p\n", shared_var, &shared_var);
        exit(0);
    } else {
        wait(NULL);  // Wait for child to finish
        // Parent's variable unchanged!
        printf("PARENT: shared_var = %d at address %p\n", shared_var, &shared_var);
        printf("Note: Same virtual address, different physical memory!\n");
    }
}

void demonstrate_multiple_forks() {
    printf("\n=== Multiple Forks (Process Tree) ===\n");
    printf("How many processes will be created?\n");

    printf("Initial process: PID = %d\n", getpid());

    fork();  // Creates 2 processes total
    fork();  // Each process forks, creating 4 total

    // This executes in ALL 4 processes!
    printf("Hello from PID = %d, Parent = %d\n", getpid(), getppid());

    // Wait to see output clearly
    sleep(1);
}

int main() {
    demonstrate_fork_basics();
    demonstrate_memory_separation();

    printf("\n=== Press Enter to see multiple forks ===\n");
    getchar();
    demonstrate_multiple_forks();

    return 0;
}

/**
 * QUESTIONS TO ANSWER:
 *
 * 1. What does fork() return in the parent? In the child?
 * 2. Why do both processes continue from the same point after fork()?
 * 3. If variables have the same address in parent and child, why don't they share values?
 * 4. In demonstrate_multiple_forks(), how many processes are created total?
 * 5. What's the formula for n fork() calls creating how many processes?
 *
 * EXPERIMENT:
 * - Remove wait() calls and see what happens
 * - Add sleep() in child vs parent
 * - Try fork() in a loop
 */
