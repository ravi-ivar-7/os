#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define JOURNAL_FILE "journal.log"
#define FAKE_FS_FILE "filesystem.dat"
#define MAX_OP_LEN 256

// Journal entry
typedef struct {
    int transaction_id;
    char operation[MAX_OP_LEN];
    int committed;
} JournalEntry;

int next_txn_id = 1;

void write_to_journal(const char *operation, int commit) {
    FILE *f = fopen(JOURNAL_FILE, "a");
    if (!f) { perror("Cannot open journal"); return; }

    JournalEntry entry = {
        .transaction_id = next_txn_id,
        .committed = commit
    };
    strncpy(entry.operation, operation, MAX_OP_LEN - 1);
    fprintf(f, "TXN=%d | COMMITTED=%d | OP: %s\n",
            entry.transaction_id, entry.committed, entry.operation);
    fclose(f);
}

void apply_to_filesystem(const char *operation) {
    FILE *f = fopen(FAKE_FS_FILE, "a");
    if (!f) { perror("Cannot open filesystem"); return; }
    fprintf(f, "[APPLIED] TXN=%d: %s\n", next_txn_id, operation);
    fclose(f);
}

void perform_write(const char *filename, const char *data) {
    printf("\n--- Transaction %d: write('%s', '%s') ---\n", next_txn_id, filename, data);

    // Step 1: Write intent to journal
    char op[MAX_OP_LEN];
    snprintf(op, MAX_OP_LEN, "write(file='%s', data='%s')", filename, data);
    printf("  [1] Writing to journal (uncommitted)...\n");
    write_to_journal(op, 0);

    // Step 2: Simulate actual disk write
    printf("  [2] Applying change to filesystem...\n");
    apply_to_filesystem(op);

    // Step 3: Mark journal entry as committed
    // (In a real system, this would update the specific record)
    printf("  [3] Marking journal entry as committed...\n");
    write_to_journal(op, 1);

    printf("  Transaction %d complete.\n", next_txn_id);
    next_txn_id++;
}

void simulate_crash_and_recover() {
    printf("\n\n=== Simulating Crash Mid-Transaction ===\n");
    printf("--- Transaction %d: write('important.db', 'new_record_123') ---\n", next_txn_id);

    char op[MAX_OP_LEN] = "write(file='important.db', data='new_record_123')";
    printf("  [1] Writing to journal (uncommitted)...\n");
    write_to_journal(op, 0);

    printf("  [2] Applying to filesystem...\n");
    // Filesystem write started...
    apply_to_filesystem(op);

    printf("  [CRASH! Power lost before journal commit was written.]\n");
    // Transaction is NOT committed in journal

    // Recovery
    printf("\n=== System Restart: Running Journal Recovery ===\n");
    FILE *f = fopen(JOURNAL_FILE, "r");
    if (!f) { printf("No journal found. Clean state.\n"); return; }

    char line[512];
    int found_incomplete = 0;
    printf("Scanning journal...\n");
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "COMMITTED=0")) {
            printf("  Found uncommitted entry: %s", line);
            found_incomplete = 1;
        }
    }
    fclose(f);

    if (found_incomplete) {
        printf("\nRecovery: Rolling back uncommitted transactions.\n");
        printf("Filesystem is now in a consistent state (data was not fully written).\n");
    } else {
        printf("All transactions were committed. Filesystem is consistent.\n");
    }
}

int main() {
    // Clear old simulation files
    remove(JOURNAL_FILE);
    remove(FAKE_FS_FILE);

    printf("=== Journaling Filesystem Simulation ===\n");

    perform_write("config.txt", "theme=dark");
    perform_write("userdata.db", "user_id=42");
    perform_write("log.txt", "event=login,ts=1710000000");

    simulate_crash_and_recover();

    printf("\n\nDone. Inspect '%s' and '%s' to see output.\n", JOURNAL_FILE, FAKE_FS_FILE);
    return 0;
}
