# Makefile for OS Process Control Learning

CC = gcc
CFLAGS = -Wall -Wextra -g
PTHREAD_FLAGS = -lpthread

# Directories
DIR_BASICS = 01-process-basics
DIR_CONTEXT = 02-context-switching
DIR_SCHED = 03-schedulers
DIR_IPC = 04-ipc
DIR_SYNC = 05-synchronization

# Targets
BASICS_TARGETS = $(DIR_BASICS)/fork_basics $(DIR_BASICS)/process_states $(DIR_BASICS)/zombie_orphan
CONTEXT_TARGETS = $(DIR_CONTEXT)/measure_context_switch
SCHED_TARGETS = $(DIR_SCHED)/scheduler_simulator
SYNC_TARGETS = $(DIR_SYNC)/race_condition $(DIR_SYNC)/producer_consumer

ALL_TARGETS = $(BASICS_TARGETS) $(CONTEXT_TARGETS) $(SCHED_TARGETS) $(SYNC_TARGETS)

.PHONY: all clean basics context sched sync help

all: $(ALL_TARGETS)
	@echo "All programs compiled successfully!"
	@echo "Run 'make help' to see available commands"

# Process Basics
basics: $(BASICS_TARGETS)

$(DIR_BASICS)/fork_basics: $(DIR_BASICS)/01_fork_basics.c
	$(CC) $(CFLAGS) $< -o $@

$(DIR_BASICS)/process_states: $(DIR_BASICS)/02_process_states.c
	$(CC) $(CFLAGS) $< -o $@

$(DIR_BASICS)/zombie_orphan: $(DIR_BASICS)/04_zombie_orphan.c
	$(CC) $(CFLAGS) $< -o $@

# Context Switching
context: $(CONTEXT_TARGETS)

$(DIR_CONTEXT)/measure_context_switch: $(DIR_CONTEXT)/01_measure_context_switch.c
	$(CC) $(CFLAGS) $< -o $@

# Schedulers
sched: $(SCHED_TARGETS)

$(DIR_SCHED)/scheduler_simulator: $(DIR_SCHED)/scheduler_simulator.c
	$(CC) $(CFLAGS) $< -o $@

# Synchronization
sync: $(SYNC_TARGETS)

$(DIR_SYNC)/race_condition: $(DIR_SYNC)/01_race_condition.c
	$(CC) $(CFLAGS) $(PTHREAD_FLAGS) $< -o $@

$(DIR_SYNC)/producer_consumer: $(DIR_SYNC)/02_producer_consumer.c
	$(CC) $(CFLAGS) $(PTHREAD_FLAGS) $< -o $@

# Clean
clean:
	rm -f $(ALL_TARGETS)
	@echo "All compiled programs removed"

# Help
help:
	@echo "OS Process Control Learning - Makefile"
	@echo "======================================"
	@echo ""
	@echo "Available targets:"
	@echo "  make all      - Compile all programs"
	@echo "  make basics   - Compile process basics programs"
	@echo "  make context  - Compile context switching programs"
	@echo "  make sched    - Compile scheduler simulator"
	@echo "  make sync     - Compile synchronization programs"
	@echo "  make clean    - Remove all compiled programs"
	@echo ""
	@echo "Quick start:"
	@echo "  make all                                    # Compile everything"
	@echo "  ./01-process-basics/fork_basics            # Run fork demo"
	@echo "  ./02-context-switching/measure_context_switch  # Measure context switch cost"
	@echo "  ./03-schedulers/scheduler_simulator        # Compare schedulers"
	@echo "  ./05-synchronization/race_condition        # See race conditions"
	@echo ""
	@echo "See GETTING_STARTED.md for full learning path"
