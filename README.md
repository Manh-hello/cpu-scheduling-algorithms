CPU SCHEDULING ALGORITHMS - L THUYT V P DNG
PHN 1: L THUYT C BN
1.1 CPU Scheduling l g?
CPU Scheduling l qu trnh h iu hnh quyt nh process no s c CPU thc thi tip theo. y l chc nng ct li ca h iu hnh, gip ti u ha vic s dng CPU v ci thin hiu sut h thng.
1.2 Process Control Block (PCB)
Mi process c qun l thng qua PCB cha cc thng tin:

Process ID (PID): nh danh duy nht
Process State: Trng thi hin ti (new, ready, running, waiting, terminated)
Program Counter: a ch lnh tip theo
CPU Registers: Gi tr cc thanh ghi
CPU Scheduling Information: Priority, arrival time, burst time
Memory Management Information: Page tables, segment tables
I/O Status Information: Danh sch cc thit b I/O ang s dng

1.3 Cc trng thi ca Process
   TERMINATEDRUNNING READY NEW 
                 
  WAITING        

New: Process va c to
Ready: Process  sn sng, ch CPU
Running: Process ang c CPU thc thi
Waiting: Process ch I/O hoc event
Terminated: Process hon thnh

1.4 Cc tiu ch nh gi Scheduling Algorithm
a) CPU Utilization (Mc s dng CPU)

Phn trm thi gian CPU hot ng
Mc tiu: Ti a ha (l ttng 100%, thc t 40-90%)

b) Throughput (Thng lng)

S process hon thnh trong mt n v thi gian
Mc tiu: Ti a ha

c) Turnaround Time (Thi gian hon thnh)

Tng thi gian t khi process n h thng n khi hon thnh
TAT = Completion Time - Arrival Time
Mc tiu: Ti thiu ha

d) Waiting Time (Thi gian ch)

Tng thi gian process ch trong ready queue
WT = Turnaround Time - Burst Time
Mc tiu: Ti thiu ha

e) Response Time (Thi gian phn hi)

Thi gian t khi process n h thng n ln u tin c CPU thc thi
RT = Time of First Response - Arrival Time
Mc tiu: Ti thiu ha (quan trng cho interactive systems)

1.5 Preemptive vs Non-preemptive Scheduling
Non-preemptive (Khng chim quyn)

Process gi CPU cho n khi t nguyn nh (hon thnh hoc ch I/O)
n gin, t overhead
C th gy blocking cho process khc
V d: FCFS, SJF

Preemptive (Chim quyn)

H iu hnh c th tm dng process ang chy  cho process khc chy
Linh hot, responsive hn
C context switching overhead
Cn x l synchronization
V d: SRTF, RR, Priority with preemption


PHN 2: CC THUT TON CPU SCHEDULING
2.1 FCFS (First Come First Served)
c im:

Non-preemptive
Process n trc c phc v trc
n gin nht, dng FIFO queue
u im: D hiu, d implement, cng bng
Nhc im: Average Waiting Time cao, Convoy Effect (process ngn phi ch process di)

Convoy Effect:
Khi mt process di chim CPU, cc process ngn phi ch lu, lm gim hiu sut tng th.

2.2 SJF (Shortest Job First)
c im:

Non-preemptive
Process c burst time ngn nht c chn trc
Ti u v Average Waiting Time
u im: Ti thiu ha Average WT trong non-preemptive algorithms
Nhc im:

Starvation (process di c th khng bao gi c chy)
Kh bit trc burst time chnh xc
Khng ph hp vi interactive systems



Starvation:
Process c burst time di lin tc b process ngn hn vt qua, c th khng bao gi c thc thi.

2.3 SRTF (Shortest Remaining Time First)
c im:

Preemptive version ca SJF
Chn process c thi gian cn li ngn nht
Khi c process mi n, so snh remaining time
u im: Ti u nht v Average Waiting Time (trong tt c algorithms)
Nhc im:

Context switching overhead cao
Starvation cho process di
Cn bit burst time




2.4 RR (Round Robin)
c im:

Preemptive
Mi process c phn b mt time quantum (time slice)
Sau khi ht quantum, process quay v cui ready queue
u im:

Cng bng, khng c starvation
Response time tt cho interactive systems


Nhc im:

Context switching overhead
Performance ph thuc vo time quantum
Average WT thng cao hn SJF



Chn Time Quantum:

Qu nh: Context switching overhead qu cao
Qu ln: Ging FCFS, mt tnh interactive
L ttng: 10-100 milliseconds


2.5 Priority Scheduling
c im:

Mi process c mt priority (s cng nh priority cng cao hoc ngc li)
Process c priority cao nht c chn
C th preemptive hoc non-preemptive
u im: Linh hot, phn nh tm quan trng ca process
Nhc im:

Starvation cho low-priority processes
Priority inversion problem



Gii quyt Starvation - Aging:
Tng dn priority ca process theo thi gian ch. Process ch cng lu, priority cng cao.

PHN 3: V D TNG HP
Cho 5 processes vi thng tin sau:
ProcessArrival Time (AT)Burst Time (BT)PriorityP1063P2221P3484P4632P5845
Lu : Priority s nh = Priority cao

3.1 p dng FCFS
Nguyn tc:
Thc thi theo th t arrival time
Gantt Chart:
| P1  | P2 | P3      | P4  | P5   |
0     6    8        16   19    23
Tnh ton:
ProcessATBTCTTAT = CT-ATWT = TAT-BTRT = WTP1066600P2228644P348161244P46319131010P58423151111
Kt qu:

Average Waiting Time = (0+4+4+10+11)/5 = 5.8
Average Turnaround Time = (6+6+12+13+15)/5 = 10.4
Average Response Time = 5.8

Nhn xt:
P4 v P5 c WT cao v phi ch P3 (process  Convoy Effectdi) 

3.2 p dng SJF (Non-preemptive)
Nguyn tc:
Ti mi thi im CPU rnh, chn process c BT ngn nht trong cc process  n
Qu trnh:

t=0: P1 n, chy P1
t=6: P1 xong, P2 v P3  n. Chn P2 (BT=2 < BT=8)
t=8: P2 xong, P3, P4, P5  n. Chn P4 (BT=3 < BT=8,4)
t=11: P4 xong. Chn P5 (BT=4 < BT=8)
t=15: P5 xong. Chn P3

Gantt Chart:
| P1  | P2 | P4  | P5   | P3      |
0     6    8    11    15       23
Tnh ton:
ProcessATBTCTTAT = CT-ATWT = TAT-BTRT = WTP1066600P2228644P34823191111P46311522P58415733
Kt qu:

Average Waiting Time = (0+4+11+2+3)/5 = 4.0
Average Turnaround Time = (6+6+19+5+7)/5 = 8.6
Average Response Time = 4.0

Nhn xt:

Average WT gim t 5.8 (FCFS) xung 4.0
P3 b starvation (ch lu nht v BT ln nht)


3.3 p dng SRTF (Preemptive SJF)
Nguyn tc:
Ti mi thi im c process mi n, so snh remaining time v chn process ngn nht
Qu trnh chi tit:

t=0: P1 chy (RT=6)
t=2: P2 n (RT=2 < 4), preempt P1, chy P2
t=4: P2 xong, P3 n (RT=8). P1 cn RT=4 < 8, chy P1
t=6: P4 n (RT=3 < 2), preempt P1, chy P4
t=8: P5 n (RT=4). P4 cn RT=1, tip tc P4
t=9: P4 xong. P1(RT=2) < P5(RT=4) < P3(RT=8), chy P1
t=11: P1 xong. P5(RT=4) < P3(RT=8), chy P5
t=15: P5 xong. Chy P3

Gantt Chart:
| P1 | P2 | P1 | P4  | P1 | P5   | P3      |
0    2    4    6     9   11    15       23
Tnh ton:
ProcessATBTCTTAT = CT-ATWT = TAT-BTRTP106111150P2224200P34823191111P4639300P58415733
Kt qu:

Average Waiting Time = (5+0+11+0+3)/5 = 3.8
Average Turnaround Time = (11+2+19+3+7)/5 = 8.4
Average Response Time = (0+0+11+0+3)/5 = 2.8

Nhn xt:

Average WT tt nht: 3.8
P1 b preempt 2  context switching overheadln 
P3 vn b starvation nghim trng


3.4 p dng Round Robin (Time Quantum = 4)
Nguyn tc:
Mi process chy ti a 4 n v thi gian, sau  quay v cui queue
Qu trnh:

t=0-4: P1 chy 4 (cn 2), vo cui queue
t=4-6: P2 n t=2, P3 n t=4. Queue: [P2, P3, P1]. Chy P2 (2), xong
t=6-10: Queue: [P3, P1, P4]. P4 n t=6. Chy P3 (4, cn 4)
t=10-12: Queue: [P1, P4, P3]. P5 n t=8. Chy P1 (2), xong
t=12-15: Queue: [P4, P3, P5]. Chy P4 (3), xong
t=15-19: Queue: [P3, P5]. Chy P3 (4), xong
t=19-23: Queue: [P5]. Chy P5 (4), xong

Gantt Chart:
| P1 | P2 | P3 | P1 | P4  | P3 | P5   |
0    4    6   10   12   15   19    23
Tnh ton:
ProcessATBTCTTAT = CT-ATWT = TAT-BTRTP106121260P2226422P348191572P46315966P58423151111
Kt qu:

Average Waiting Time = (6+2+7+6+11)/5 = 6.4
Average Turnaround Time = (12+4+15+9+15)/5 = 11.0
Average Response Time = (0+2+2+6+11)/5 = 4.2

Nhn xt:

Average WT cao hn SJF/SRTF nhng cng bng hn
Response Time tt cho P2, P3
Khng c starvation
Context switching nhiu (6 ln)


3.5 p dng Priority Scheduling (Non-preemptive)
Nguyn tc:
Ti mi thi im CPU rnh, chn process c priority cao nht (s nh nht)
Qu trnh:

t=0: P1 chy (priority=3)
t=6: P1 xong. P2(1), P3(4), P4(2)  n. Chn P2 (priority=1)
t=8: P2 xong. P3(4), P4(2), P5(5)  n. Chn P4 (priority=2)
t=11: P4 xong. Chn P3 (priority=4 < 5)
t=19: P3 xong. Chn P5

Gantt Chart:
| P1  | P2 | P4  | P3      | P5   |
0     6    8    11       19    23
Tnh ton:
ProcessATBTPriorityCTTATWTRTP10636600P22218644P3484191577P463211522P584523151111
Kt qu:

Average Waiting Time = (0+4+7+2+11)/5 = 4.8
Average Turnaround Time = (6+6+15+5+15)/5 = 9.4
Average Response Time = 4.8

Nhn xt:

Process c thc thi theo th t priority: P5P3P4P2P1
P5 c priority thp  ch lNhtu nht (starvation) 
Average WT = 4.8 (tt hn FCFS, RR nhng km SJF/SRTF)


3.6 p dng Priority Scheduling (Preemptive)
Nguyn tc:
Khi c process mi n, nu priority cao hn process ang  preemptchy 
Qu trnh:

t=0-2: P1 chy (priority=3)
t=2: P2 n (priority=1 < 3), preempt P1, chy P2
t=4: P2 xong, P3 n (priority=4). P1 cn li c priority=3 < 4, chy P1
t=6: P4 n (priority=2 < 3), preempt P1, chy P4
t=9: P4 xong. P1(3) < P3(4) < P5(5), chy P1
t=11: P1 xong. Chy P3
t=19: P3 xong. Chy P5

Gantt Chart:
| P1 | P2 | P1 | P4  | P1 | P3      | P5   |
0    2    4    6     9   11       19    23
Tnh ton:
ProcessATBTPriorityCTTATWTRTP1063111150P22214200P3484191577P46329300P584523151111
Kt qu:

Average Waiting Time = (5+0+7+0+11)/5 = 4.6
Average Turnaround Time = (11+2+15+3+15)/5 = 9.2
Average Response Time = (0+0+7+0+11)/5 = 3.6

Nhn xt:

P2 v P4 (high priority) c u  WT = 0tin 
P1 b preempt 2 ln
P5 (lowest priority) ch lu  starvationnht 
Average WT tt hn non-preemptive priority (4.6 vs 4.8)
