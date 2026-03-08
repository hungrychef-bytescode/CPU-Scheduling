# CPU Scheduling

A CPU scheduling simulator that demonstrates operating system scheduling algorithms like FCFS, SJF, STCF, Round Robin, and MLFQ

---

# Group Members

- Janiola, Angel May
- Verde, Myra

---

# Scheduling Algorithms

- FCFS (First Come First Serve) - executes process in order of arrival
- SJF - runs process with the shortest burst time
- SCTF - executes process with shortest remaining time
- Round Robin - cycle processes with fixed time quantum
- MLFQ - use multiple queue with priority adjustments

---

read input from file/shell (format: PID, arrival time, burst time (not used by mlfq))          # --comments
-> scheduling algorithm ->  calculate metrics -> output gantt chart, metrics/notes acc to algorithm

-accept workloads via cmd line arg

---
# Task Distribution and Project Timeline:
- Makefile   --week 1-2  (both will edit)
- Readme     -- 1-4       (both)
- mlfq docs (both)  -- week 3     (both)
- gantt chart       -- week 2/3       (angel)
- sjf               -- week 2          (angel)
- sctf              -- week 2           (myra)
- fcfs              -- week 2          (angel)
- rr                -- week 2/3         (myra)
- mlfq              -- week 3           (both)
- testing           -- week 4          (both)
- debugging         -- week 4         (both)
- file/input parsing  -- week 2     
- overall na pagcombine and execute ng scheduler simulator    (myra)
- lab defense   --week 4
//ask -> if one input of data multiple sched algo ba or isa isa ang pagexecute depending on command
- comparison -- all workload
//add test cases
