/**
 * Exercise 4: Zombie and Orphan Processes
 *
 * Learn:
 * - What happens when parent doesn't wait() for child (zombie)
 * - What happens when parent dies before child (orphan)
 * - How the OS handles these edge cases
 * - Why wait() is important
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void create_zombie() {
    printf("=== Creating Zombie Process ===\n");
    printf("Parent PID: %d\n", getpid());

    pid_t pid = fork();

    if (pid == 0) {
        // Child process - exits immediately
        printf("Child PID: %d - Exiting now\n", getpid());
        exit(0);
    } else {
        // Parent doesn't wait() - child becomes zombie
        printf("Parent: Created child %d, but NOT calling wait()\n", pid);
        printf("\nIn another terminal, run: ps aux | grep Z\n");
        printf("You should see child as '<defunct>' or 'Z' state\n");
        printf("Parent sleeping for 30 seconds...\n");
        sleep(30);
        printf("\nParent: Now calling wait() to reap zombie\n");
        wait(NULL);
        printf("Zombie reaped! Check ps again - should be gone\n");
    }
}

void create_orphan() {
    printf("\n=== Creating Orphan Process ===\n");
    printf("Parent PID: %d\n", getpid());

    pid_t pid = fork();

    if (pid == 0) {
        // Child process - parent will die, leaving child orphaned
        sleep(2);  // Wait for parent to die
        printf("Child PID: %d, Parent PID: %d\n", getpid(), getppid());
        printf("Notice: My parent is now PID 1 (init/systemd)!\n");
        printf("I've been adopted by the init process\n");
        sleep(10);
        exit(0);
    } else {
        // Parent exits immediately, orphaning child
        printf("Parent: Created child %d\n", pid);
        printf("Parent: Exiting immediately (orphaning child)\n");
        exit(0);
    }
}

void demonstrate_wait_importance() {
    printf("\n=== Why wait() Matters ===\n");

    for (int i = 0; i < 5; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            printf("Child %d (PID %d) finished\n", i, getpid());
            exit(0);
        }
    }

    printf("\nParent created 5 children\n");
    printf("Check zombie count: ps aux | grep defunct | wc -l\n");
    printf("Sleeping for 10 seconds...\n");
    sleep(10);

    printf("\nNow reaping all children...\n");
    for (int i = 0; i < 5; i++) {
        int status;
        pid_t child = wait(&status);
        printf("Reaped child %d\n", child);
    }
    printf("All zombies cleaned up!\n");
}

void demonstrate_waitpid() {
    printf("\n=== Using waitpid() for Specific Child ===\n");

    pid_t child1 = fork();
    if (child1 == 0) {
        printf("Child 1 (PID %d) sleeping 5 seconds...\n", getpid());
        sleep(5);
        exit(42);
    }

    pid_t child2 = fork();
    if (child2 == 0) {
        printf("Child 2 (PID %d) sleeping 2 seconds...\n", getpid());
        sleep(2);
        exit(99);
    }

    printf("Parent: Waiting specifically for child 2 (%d)\n", child2);
    int status;
    pid_t finished = waitpid(child2, &status, 0);
    printf("Child %d finished with exit code %d\n", finished, WEXITSTATUS(status));

    printf("Parent: Now waiting for child 1 (%d)\n", child1);
    finished = waitpid(child1, &status, 0);
    printf("Child %d finished with exit code %d\n", finished, WEXITSTATUS(status));
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Zombie and Orphan Process Demonstration\n\n");
        printf("Usage:\n");
        printf("  %s zombie    - Create zombie process\n", argv[0]);
        printf("  %s orphan    - Create orphan process\n", argv[0]);
        printf("  %s wait      - Demonstrate importance of wait()\n", argv[0]);
        printf("  %s waitpid   - Demonstrate waitpid() usage\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "zombie") == 0) {
        create_zombie();
    } else if (strcmp(argv[1], "orphan") == 0) {
        create_orphan();
        // Keep main process alive to observe orphan
        sleep(15);
    } else if (strcmp(argv[1], "wait") == 0) {
        demonstrate_wait_importance();
    } else if (strcmp(argv[1], "waitpid") == 0) {
        demonstrate_waitpid();
    }

    return 0;
}

/**
 * KEY INSIGHTS:
 *
 * ZOMBIE PROCESS:
 * - Child has exited but parent hasn't called wait()
 * - Process still has PCB in kernel (holds exit status)
 * - Shows as <defunct> or state 'Z'
 * - Wastes kernel resources (PID, PCB entry)
 * - wait() cleans up the zombie
 *
 * ORPHAN PROCESS:
 * - Parent exits before child
 * - Child gets re-parented to init (PID 1)
 * - Init automatically reaps orphans
 * - Not a resource leak (unlike zombies)
 *
 * WHY THIS MATTERS:
 * - Server processes that fork must wait() for children
 * - Otherwise, zombie processes accumulate
 * - Can exhaust PID space (max processes)
 * - Signal handling (SIGCHLD) for async reaping
 *
 * EXPERIMENTS:
 *
 * 1. Create many zombies without reaping:
 *    (fork many times without wait, see PID exhaustion)
 *
 * 2. Watch orphan adoption:
 *    ./zombie_orphan orphan &
 *    ps -ef | grep <child_pid>
 *    See PPID change to 1
 *
 * 3. Check exit status:
 *    Modify child to exit with different codes
 *    Use WEXITSTATUS() macro to read them
 */
