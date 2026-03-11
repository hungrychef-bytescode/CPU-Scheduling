# CPU Scheduling Simulator
> CMSC 125 Laboratory Assignment 2

A discrete-event CPU scheduling simulator built in C that demonstrates how operating systems manage process scheduling. Accepts process workloads and simulates multiple scheduling algorithms, computing key performance metrics and generating Gantt charts.

---

## Group Members

| Name | Role |
|---|---|
| Angel May B. Janiola | CLI & Input Parsing, FCFS, SJF, MLFQ, Gantt Chart, Makefile |
| Myra S. Verde | Simulation Engine, STCF, Round Robin, Performance Metrics |

---

## Compilation & Usage

### Requirements
- GCC compiler
- Unix-based system (Linux / macOS) or WSL
- POSIX-compliant environment

### Compile
```
make
```

### Run
```
# Single algorithm
./schedsim --algorithm=FCFS --input=tests/workload.txt
./schedsim --algorithm=SJF  --input=tests/workload.txt
./schedsim --algorithm=STCF --input=tests/workload.txt
./schedsim --algorithm=RR   --quantum=30 --input=tests/workload.txt
./schedsim --algorithm=MLFQ --mlfq-config=docs/mlfq_config.txt --input=tests/workload.txt

# Inline processes
./schedsim --algorithm=FCFS --processes="A:0:240,B:10:180,C:20:150"

# Compare all algorithms
./schedsim --compare --input=tests/workload.txt
```

### Clean
```
make clean
```

---

## Scheduling Algorithms

| Algorithm | Type | Description |
|---|---|---|
| FCFS | Non-preemptive | Executes processes in order of arrival |
| SJF | Non-preemptive | Runs the process with the shortest burst time first |
| STCF | Preemptive | Always runs the process with the shortest remaining time |
| Round Robin | Preemptive | Cycles through processes with a fixed time quantum |
| MLFQ | Preemptive | Multiple priority queues with dynamic demotion and boosting |

---

## Input Format

```
# Process workload specification
# Format: PID ArrivalTime BurstTime
# Lines starting with # are comments
A 0 240
B 10 180
C 20 150
D 25 80
E 30 130
```

**Note:** BurstTime is used by all algorithms except MLFQ. MLFQ infers process behavior dynamically through observation alone.

---

## How It Works

```
Input (file or --processes flag)
    │
    ▼
Parser  →  Process structs [ PID | ArrivalTime | BurstTime ]
    │
    ▼
Scheduling Algorithm (FCFS / SJF / STCF / RR / MLFQ)
    │
    ▼
Metrics Calculation  →  TT = FT - AT  |  WT = TT - BT  |  RT = StartTime - AT
    │
    ▼
Output: Gantt Chart + Metrics Table + Averages
```

---

## Performance Metrics

| Metric | Formula |
|---|---|
| Finish Time (FT) | When the process completes |
| Turnaround Time (TT) | FT − Arrival Time |
| Waiting Time (WT) | TT − Burst Time |
| Response Time (RT) | First Run Time − Arrival Time |

---

## Project Structure

```
schedsim/
├── include/
│   └── process.h         # Shared structs and function prototypes
├── src/
│   ├── main.c            # Entry point and algorithm dispatch
│   ├── parser.c          # CLI argument and workload file parsing
│   ├── utils.c           # Shared helper functions
│   ├── fcfs.c            # FCFS implementation
│   ├── sjf.c             # SJF implementation
│   ├── stcf.c            # STCF implementation
│   ├── rr.c              # Round Robin implementation
│   ├── mlfq.c            # MLFQ implementation
│   ├── metrics.c         # Metrics calculation and output
│   └── gantt.c           # Gantt chart rendering
├── tests/
│   └── workload.txt      # Test workload files
├── docs/
│   └── mlfq_design.md    # MLFQ design justification
├── Makefile
└── README.md
```

---

## Task Distribution & Timeline

| Task | Owner | Week |
|---|---|---|
| Makefile & Build System | Both | 1–2 |
| `process.h` — shared structs | Both | 1 |
| CLI & Input Parsing (`parser.c`, `main.c`, `utils.c`) | Angel | 2 |
| FCFS (`fcfs.c`) | Angel | 2 |
| SJF (`sjf.c`) | Angel | 2 |
| Gantt Chart (`gantt.c`) | Angel | 2–3 |
| MLFQ (`mlfq.c`) | Angel | 3 |
| Simulation Engine (`scheduler.c`) | Myra | 2 |
| STCF (`stcf.c`) | Myra | 2 |
| Round Robin (`rr.c`) | Myra | 2–3 |
| Performance Metrics (`metrics.c`) | Myra | 2 |
| MLFQ Design Doc (`mlfq_design.md`) | Both | 3 |
| Testing & Debugging | Both | 4 |
| README & Documentation | Both | 1–4 |
| Lab Defense | Both | Week 4 (Mar 23–26) |

---

## Known Limitations

- No pipe support between commands
- MLFQ config requires a separate config file
- Gantt chart auto-scales for simulations over 100 time units

---

## Lab Defense Dates

| Activity | Date |
|---|---|
| Week 1 Progress | Mar 5 |
| Week 2 Progress | Mar 12 |
| Week 3 Progress | Mar 16 | 
| **Lab Defense** | **Mar 23** |
