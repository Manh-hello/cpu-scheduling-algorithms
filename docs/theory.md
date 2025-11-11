# L THUYT CPU SCHEDULING

## 1. Khi nim c bn

### 1.1 CPU Scheduling l g?
CPU Scheduling l qu trnh h iu hnh quyt nh process no s c 
CPU thc thi tip theo.

### 1.2 Process Control Block (PCB)
Mi process c mt PCB cha:
- Process ID (PID)
- Process State
- Program Counter
- CPU Registers
- Scheduling Information

### 1.3 Cc trng thi Process
```
   TERMINATEDRUNNING READY NEW 
                 
  WAITING        
```

## 2. Cc tiu ch nh gi

### 2.1 CPU Utilization
Phn trm thi gian CPU hot ng.

### 2.2 Throughput
S process hon thnh trong mt n v thi gian.

### 2.3 Turnaround Time (TAT)
TAT = Completion Time - Arrival Time

### 2.4 Waiting Time (WT)
WT = Turnaround Time - Burst Time

### 2.5 Response Time (RT)
RT = Time of First Response - Arrival Time

## 3. Preemptive vs Non-preemptive

### Non-preemptive
- Process gi CPU cho n khi t nh
- t overhead
- V d: FCFS, SJF

### Preemptive
- HH c th tm dng process
- Linh hot hn
- V d: SRTF, RR

## 4. Cc thut ton chnh

### 4.1 FCFS (First Come First Served)
- Non-preemptive
- n gin, cng bng
- C th gy Convoy Effect

### 4.2 SJF (Shortest Job First)
- Non-preemptive
- Ti u Average WT
- C th gy Starvation

### 4.3 SRTF (Shortest Remaining Time First)
- Preemptive version ca SJF
- Ti u nht v Average WT
- Context switching overhead cao

### 4.4 Round Robin (RR)
- Preemptive
- S dng time quantum
- Cng bng, khng starvation
- Performance ph thuc time quantum

### 4.5 Priority Scheduling
- C th preemptive hoc non-preemptive
- Linh hot
- Cn x l starvation bng Aging

## 5. Cc vn  cn lu 

### Convoy Effect
Process di chim CPU lm process ngn phi ch lu.

### Starvation
Process c priority thp khng bao gi c chy.

### Aging
Tng priority theo thi gian ch  gii quyt starvation.

### Time Quantum trong RR
- Qu nh: Overhead cao
- Qu ln: Ging FCFS
- L ttng: 10-100ms
