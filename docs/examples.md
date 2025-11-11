# V D CHI TIT

## D liu u vo

| Process | Arrival Time | Burst Time | Priority |
|---------|--------------|------------|----------|
| P1      | 0            | 6          | 3        |
| P2      | 2            | 2          | 1        |
| P3      | 4            | 8          | 4        |
| P4      | 6            | 3          | 2        |
| P5      | 8            | 4          | 5        |

Lu : Priority s nh = cao

## 1. FCFS (First Come First Served)

### Gantt Chart
```
| P1  | P2 | P3      | P4  | P5   |
0     6    8        16   19    23
```

### Bng kt qu

| Process | AT | BT | CT | TAT | WT | RT |
|---------|----|----|-------|-----|-----|-----|
| P1      | 0  | 6  | 6     | 6   | 0   | 0   |
| P2      | 2  | 2  | 8     | 6   | 4   | 4   |
| P3      | 4  | 8  | 16    | 12  | 4   | 4   |
| P4      | 6  | 3  | 19    | 13  | 10  | 10  |
| P5      | 8  | 4  | 23    | 15  | 11  | 11  |

### Kt qu
- Average WT: 5.8
- Average TAT: 10.4
- Average RT: 5.8

### Nhn xt
P4 v P5 ch lu do Convoy Effect t P3.

## 2. SJF (Shortest Job First)

### Qu trnh thc thi
- t=0: Chy P1
- t=6: Chn P2 (BT=2 < 8)
- t=8: Chn P4 (BT=3 < 8, 4)
- t=11: Chn P5 (BT=4 < 8)
- t=15: Chy P3

### Gantt Chart
```
| P1  | P2 | P4  | P5   | P3      |
0     6    8    11    15       23
```

### Bng kt qu

| Process | AT | BT | CT | TAT | WT | RT |
|---------|----|----|-------|-----|-----|-----|
| P1      | 0  | 6  | 6     | 6   | 0   | 0   |
| P2      | 2  | 2  | 8     | 6   | 4   | 4   |
| P3      | 4  | 8  | 23    | 19  | 11  | 11  |
| P4      | 6  | 3  | 11    | 5   | 2   | 2   |
| P5      | 8  | 4  | 15    | 7   | 3   | 3   |

### Kt qu
- Average WT: 4.0
- Average TAT: 8.6
- Average RT: 4.0

### Nhn xt
P3 b starvation v BT di nht.

## 3. SRTF (Shortest Remaining Time First)

### Qu trnh chi tit
- t=0-2: P1 chy
- t=2-4: P2 preempt P1, chy xong
- t=4-6: P1 tip tc
- t=6-9: P4 preempt P1
- t=9-11: P1 chy xong
- t=11-15: P5 chy
- t=15-23: P3 chy

### Gantt Chart
```
| P1 | P2 | P1 | P4  | P1 | P5   | P3      |
0    2    4    6     9   11    15       23
```

### Bng kt qu

| Process | AT | BT | CT | TAT | WT | RT |
|---------|----|----|-------|-----|-----|-----|
| P1      | 0  | 6  | 11    | 11  | 5   | 0   |
| P2      | 2  | 2  | 4     | 2   | 0   | 0   |
| P3      | 4  | 8  | 23    | 19  | 11  | 11  |
| P4      | 6  | 3  | 9     | 3   | 0   | 0   |
| P5      | 8  | 4  | 15    | 7   | 3   | 3   |

### Kt qu
- Average WT: 3.8 (TT NHT)
- Average TAT: 8.4
- Average RT: 2.8

### Nhn xt
P1 b preempt 2 ln, P3 vn b starvation.

## 4. Round Robin (Time Quantum = 4)

### Qu trnh thc thi
- t=0-4: P1 (4u, cn 2u)
- t=4-6: P2 (2u, xong)
- t=6-10: P3 (4u, cn 4u)
- t=10-12: P1 (2u, xong)
- t=12-15: P4 (3u, xong)
- t=15-19: P3 (4u, xong)
- t=19-23: P5 (4u, xong)

### Gantt Chart
```
| P1 | P2 | P3 | P1 | P4  | P3 | P5   |
0    4    6   10   12   15   19    23
```

### Bng kt qu

| Process | AT | BT | CT | TAT | WT | RT |
|---------|----|----|-------|-----|-----|-----|
| P1      | 0  | 6  | 12    | 12  | 6   | 0   |
| P2      | 2  | 2  | 6     | 4   | 2   | 2   |
| P3      | 4  | 8  | 19    | 15  | 7   | 2   |
| P4      | 6  | 3  | 15    | 9   | 6   | 6   |
| P5      | 8  | 4  | 23    | 15  | 11  | 11  |

### Kt qu
- Average WT: 6.4
- Average TAT: 11.0
- Average RT: 4.2

### Nhn xt
Cng bng, khng starvation, nhng c 6 context switches.

## 5. Priority Scheduling (Non-preemptive)

### Qu trnh thc thi
Th t theo priority: P5P3P4P2P1

### Gantt Chart
```
| P1  | P2 | P4  | P3      | P5   |
0     6    8    11       19    23
```

### Bng kt qu

| Process | AT | BT | Priority | CT | TAT | WT | RT |
|---------|----|----|----------|-------|-----|-----|-----|
| P1      | 0  | 6  | 3        | 6     | 6   | 0   | 0   |
| P2      | 2  | 2  | 1        | 8     | 6   | 4   | 4   |
| P3      | 4  | 8  | 4        | 19    | 15  | 7   | 7   |
| P4      | 6  | 3  | 2        | 11    | 5   | 2   | 2   |
| P5      | 8  | 4  | 5        | 23    | 15  | 11  | 11  |

### Kt qu
- Average WT: 4.8
- Average TAT: 9.4
- Average RT: 4.8

### Nhn xt
P5 (lowest priority) b starvation.

## 6. Priority Scheduling (Preemptive)

### Qu trnh chi tit
- t=0-2: P1
- t=2-4: P2 preempt P1
- t=4-6: P1 tip tc
- t=6-9: P4 preempt P1
- t=9-11: P1 chy xong
- t=11-19: P3
- t=19-23: P5

### Gantt Chart
```
| P1 | P2 | P1 | P4  | P1 | P3      | P5   |
0    2    4    6     9   11       19    23
```

### Bng kt qu

| Process | AT | BT | Priority | CT | TAT | WT | RT |
|---------|----|----|----------|-------|-----|-----|-----|
| P1      | 0  | 6  | 3        | 11    | 11  | 5   | 0   |
| P2      | 2  | 2  | 1        | 4     | 2   | 0   | 0   |
| P3      | 4  | 8  | 4        | 19    | 15  | 7   | 7   |
| P4      | 6  | 3  | 2        | 9     | 3   | 0   | 0   |
| P5      | 8  | 4  | 5        | 23    | 15  | 11  | 11  |

### Kt qu
- Average WT: 4.6
- Average TAT: 9.2
- Average RT: 3.6

### Nhn xt
High priority processes (P2, P4) c WT=0.

## So snh tng hp

| Algorithm      | Avg WT | Avg TAT | Avg RT | Context Switches |
|----------------|--------|---------|--------|------------------|
| FCFS           | 5.8    | 10.4    | 5.8    | 0                |
| SJF            | 4.0    | 8.6     | 4.0    | 0                |
| SRTF           | 3.8    | 8.4     | 2.8    | 5                |
| RR (q=4)       | 6.4    | 11.0    | 4.2    | 6                |
| Priority (NP)  | 4.8    | 9.4     | 4.8    | 0                |
| Priority (P)   | 4.6    | 9.2     | 3.6    | 4                |

### Kt lun
- **Ti u WT**: SRTF (3.8)
- **Cng bng nht**: RR v FCFS
- **n gin nht**: FCFS
- **Thc t nht**: RR
