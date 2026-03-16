# Sleep and wakeup

This branch adds the facility to let processes voluntarily give up the CPU.  For
example, `init.c` sleeps for 100 timer ticks within an infinite loop.

To realize this, we add a new process state called `SLEEPING` so that scheduler
can skip scheduling `SLEEPING` processes. In xv6, processes sleep on "channels".
Processes can call `sleep(chan)` to sleep on a channel `chan`.  At some later
stage, `wakeup(chan)` can wakeup all processes sleeping on `chan`. Waking up a
process just means setting its state to `RUNNABLE`.  Whenever, the scheduler is
invoked next, these processes can now get scheduled.

For example, the sleep system call implementation (`sys_sleep`) calls
`sleep(&ticks)` i.e., it sleeps on the `ticks` variable managed by the timer
interrupt handler in `trap.c`. Whenever a timer interrupt is triggered, the
timer interrupt handler additionally calls `wakeup(&ticks)` to wake up all the
processes who had called the `sleep` system call. After waking up, the system 
call handler `sys_sleep` re-checks if enough ticks have elapsed. Otherwise, it
sleeps on `&ticks` again.

In `ide.c`, when a process enqueues a request to read/write a disk block into a
buffer `b`, it wasted CPU cycles in `iderw(b)` just waiting for `b` to be
read/written. Now, we just sleep on the block `b` and get woken up from the
`ideintr`.

When starting an FS operation `begin_op` in `log.c`, if the log was full, the OS
used to panic. Now instead, we sleep on the log and wait for it to become free.
The log becomes free after commit, where it wakes up all the processes sleeping
on the log.

With all this sleeping, we want to be careful that the CPU scheduler itself does
not sleep, i.e., it should not be the case that there is no one left to run the
scheduler. Therefore, we move initialization of the file system `iinit` and of 
the log `initlog` to starting of the first process in `proc.c`. This is because
initializing the file system reads the super block into the buffer cache, which
may sleep in `ide.c`. Similarly, initializing the log additionally reads the log
header block and optionally recovers a pending transaction.