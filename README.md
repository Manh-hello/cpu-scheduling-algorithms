# CPU SCHEDULING ALGORITHMS - LÝ THUYẾT VÀ ÁP DỤNG

## PHẦN 1: LÝ THUYẾT CƠ BẢN

### 1.1 CPU Scheduling là gì?

**CPU Scheduling** là quá trình hệ điều hành quyết định process nào sẽ được CPU thực thi tiếp theo. Đây là chức năng cốt lõi của hệ điều hành, giúp tối ưu hóa việc sử dụng CPU và cải thiện hiệu suất hệ thống.

### 1.2 Process Control Block (PCB)

Mỗi process được quản lý thông qua PCB chứa các thông tin:
- **Process ID (PID)**: Định danh duy nhất
- **Process State**: Trạng thái hiện tại (new, ready, running, waiting, terminated)
- **Program Counter**: Địa chỉ lệnh tiếp theo
- **CPU Registers**: Giá trị các thanh ghi
- **CPU Scheduling Information**: Priority, arrival time, burst time
- **Memory Management Information**: Page tables, segment tables
- **I/O Status Information**: Danh sách các thiết bị I/O đang sử dụng

### 1.3 Các trạng thái của Process
```
NEW → READY → RUNNING → TERMINATED
        ↑         ↓
        ←  WAITING
```

- **New**: Process vừa được tạo
- **Ready**: Process đã sẵn sàng, chờ CPU
- **Running**: Process đang được CPU thực thi
- **Waiting**: Process chờ I/O hoặc event
- **Terminated**: Process hoàn thành

### 1.4 Các tiêu chí đánh giá Scheduling Algorithm

#### a) CPU Utilization (Mức sử dụng CPU)
- Phần trăm thời gian CPU hoạt động
- Mục tiêu: Tối đa hóa (lý tưởng 100%, thực tế 40-90%)

#### b) Throughput (Thông lượng)
- Số process hoàn thành trong một đơn vị thời gian
- Mục tiêu: Tối đa hóa

#### c) Turnaround Time (Thời gian hoàn thành)
- Tổng thời gian từ khi process đến hệ thống đến khi hoàn thành
- **TAT = Completion Time - Arrival Time**
- Mục tiêu: Tối thiểu hóa

#### d) Waiting Time (Thời gian chờ)
- Tổng thời gian process chờ trong ready queue
- **WT = Turnaround Time - Burst Time**
- Mục tiêu: Tối thiểu hóa

#### e) Response Time (Thời gian phản hồi)
- Thời gian từ khi process đến hệ thống đến lần đầu tiên được CPU thực thi
- **RT = Time of First Response - Arrival Time**
- Mục tiêu: Tối thiểu hóa (quan trọng cho interactive systems)

### 1.5 Preemptive vs Non-preemptive Scheduling

#### Non-preemptive (Không chiếm quyền)
- Process giữ CPU cho đến khi tự nguyện nhả (hoàn thành hoặc chờ I/O)
- Đơn giản, ít overhead
- Có thể gây blocking cho process khác
- Ví dụ: FCFS, SJF

#### Preemptive (Chiếm quyền)
- Hệ điều hành có thể tạm dừng process đang chạy để cho process khác chạy
- Linh hoạt, responsive hơn
- Có context switching overhead
- Cần xử lý synchronization
- Ví dụ: SRTF, RR, Priority with preemption

---

## PHẦN 2: CÁC THUẬT TOÁN CPU SCHEDULING

### 2.1 FCFS (First Come First Served)

#### Đặc điểm:
- **Non-preemptive**
- Process đến trước được phục vụ trước
- Đơn giản nhất, dùng FIFO queue
- **Ưu điểm**: Dễ hiểu, dễ implement, công bằng
- **Nhược điểm**: Average Waiting Time cao, Convoy Effect (process ngắn phải chờ process dài)

#### Convoy Effect:
Khi một process dài chiếm CPU, các process ngắn phải chờ lâu, làm giảm hiệu suất tổng thể.

---

### 2.2 SJF (Shortest Job First)

#### Đặc điểm:
- **Non-preemptive**
- Process có burst time ngắn nhất được chọn trước
- Tối ưu về Average Waiting Time
- **Ưu điểm**: Tối thiểu hóa Average WT trong non-preemptive algorithms
- **Nhược điểm**: 
  - Starvation (process dài có thể không bao giờ được chạy)
  - Khó biết trước burst time chính xác
  - Không phù hợp với interactive systems

#### Starvation:
Process có burst time dài liên tục bị process ngắn hơn vượt qua, có thể không bao giờ được thực thi.

---

### 2.3 SRTF (Shortest Remaining Time First)

#### Đặc điểm:
- **Preemptive version của SJF**
- Chọn process có thời gian còn lại ngắn nhất
- Khi có process mới đến, so sánh remaining time
- **Ưu điểm**: Tối ưu nhất về Average Waiting Time (trong tất cả algorithms)
- **Nhược điểm**: 
  - Context switching overhead cao
  - Starvation cho process dài
  - Cần biết burst time

---

### 2.4 RR (Round Robin)

#### Đặc điểm:
- **Preemptive**
- Mỗi process được phân bổ một time quantum (time slice)
- Sau khi hết quantum, process quay về cuối ready queue
- **Ưu điểm**: 
  - Công bằng, không có starvation
  - Response time tốt cho interactive systems
- **Nhược điểm**: 
  - Context switching overhead
  - Performance phụ thuộc vào time quantum
  - Average WT thường cao hơn SJF

#### Chọn Time Quantum:
- **Quá nhỏ**: Context switching overhead quá cao
- **Quá lớn**: Giống FCFS, mất tính interactive
- **Lý tưởng**: 10-100 milliseconds

---

### 2.5 Priority Scheduling

#### Đặc điểm:
- Mỗi process có một priority (số càng nhỏ priority càng cao hoặc ngược lại)
- Process có priority cao nhất được chọn
- Có thể **preemptive** hoặc **non-preemptive**
- **Ưu điểm**: Linh hoạt, phản ánh tầm quan trọng của process
- **Nhược điểm**: 
  - Starvation cho low-priority processes
  - Priority inversion problem

#### Giải quyết Starvation - Aging:
Tăng dần priority của process theo thời gian chờ. Process chờ càng lâu, priority càng cao.

---

## PHẦN 3: VÍ DỤ TỔNG HỢP

Cho **5 processes** với thông tin sau:

| Process | Arrival Time (AT) | Burst Time (BT) | Priority |
|---------|-------------------|-----------------|----------|
| P1      | 0                 | 6               | 3        |
| P2      | 2                 | 2               | 1        |
| P3      | 4                 | 8               | 4        |
| P4      | 6                 | 3               | 2        |
| P5      | 8                 | 4               | 5        |

**Lưu ý:** Priority số nhỏ = Priority cao

---

### 3.1 Áp dụng FCFS

#### Nguyên tắc:
Thực thi theo thứ tự arrival time

#### Gantt Chart:
```
| P1  | P2 | P3      | P4  | P5   |
0     6    8        16   19    23
```

#### Tính toán:

| Process | AT | BT | CT | TAT = CT-AT | WT = TAT-BT | RT = WT |
|---------|----|----|-------|-------------|-------------|---------|
| P1      | 0  | 6  | 6     | 6           | 0           | 0       |
| P2      | 2  | 2  | 8     | 6           | 4           | 4       |
| P3      | 4  | 8  | 16    | 12          | 4           | 4       |
| P4      | 6  | 3  | 19    | 13          | 10          | 10      |
| P5      | 8  | 4  | 23    | 15          | 11          | 11      |

#### Kết quả:
- **Average Waiting Time** = (0+4+4+10+11)/5 = **5.8**
- **Average Turnaround Time** = (6+6+12+13+15)/5 = **10.4**
- **Average Response Time** = **5.8**

#### Nhận xét:
P4 và P5 có WT cao vì phải chờ P3 (process dài) → Convoy Effect

---

### 3.2 Áp dụng SJF (Non-preemptive)

#### Nguyên tắc:
Tại mỗi thời điểm CPU rảnh, chọn process có BT ngắn nhất trong các process đã đến

#### Quá trình:
- **t=0**: P1 đến, chạy P1
- **t=6**: P1 xong, P2 và P3 đã đến. Chọn P2 (BT=2 < BT=8)
- **t=8**: P2 xong, P3, P4, P5 đã đến. Chọn P4 (BT=3 < BT=8,4)
- **t=11**: P4 xong. Chọn P5 (BT=4 < BT=8)
- **t=15**: P5 xong. Chọn P3

#### Gantt Chart:
```
| P1  | P2 | P4  | P5   | P3      |
0     6    8    11    15       23
```

#### Tính toán:

| Process | AT | BT | CT | TAT = CT-AT | WT = TAT-BT | RT = WT |
|---------|----|----|-------|-------------|-------------|---------|
| P1      | 0  | 6  | 6     | 6           | 0           | 0       |
| P2      | 2  | 2  | 8     | 6           | 4           | 4       |
| P3      | 4  | 8  | 23    | 19          | 11          | 11      |
| P4      | 6  | 3  | 11    | 5           | 2           | 2       |
| P5      | 8  | 4  | 15    | 7           | 3           | 3       |

#### Kết quả:
- **Average Waiting Time** = (0+4+11+2+3)/5 = **4.0**
- **Average Turnaround Time** = (6+6+19+5+7)/5 = **8.6**
- **Average Response Time** = **4.0**

#### Nhận xét:
- Average WT giảm từ 5.8 (FCFS) xuống 4.0
- P3 bị starvation (chờ lâu nhất vì BT lớn nhất)

---

### 3.3 Áp dụng SRTF (Preemptive SJF)

#### Nguyên tắc:
Tại mỗi thời điểm có process mới đến, so sánh remaining time và chọn process ngắn nhất

#### Quá trình chi tiết:
- **t=0**: P1 chạy (RT=6)
- **t=2**: P2 đến (RT=2 < 4), preempt P1, chạy P2
- **t=4**: P2 xong, P3 đến (RT=8). P1 còn RT=4 < 8, chạy P1
- **t=6**: P4 đến (RT=3 < 2), preempt P1, chạy P4
- **t=8**: P5 đến (RT=4). P4 còn RT=1, tiếp tục P4
- **t=9**: P4 xong. P1(RT=2) < P5(RT=4) < P3(RT=8), chạy P1
- **t=11**: P1 xong. P5(RT=4) < P3(RT=8), chạy P5
- **t=15**: P5 xong. Chạy P3

#### Gantt Chart:
```
| P1 | P2 | P1 | P4  | P1 | P5   | P3      |
0    2    4    6     9   11    15       23
```

#### Tính toán:

| Process | AT | BT | CT | TAT = CT-AT | WT = TAT-BT | RT |
|---------|----|----|-------|-------------|-------------|-----|
| P1      | 0  | 6  | 11    | 11          | 5           | 0   |
| P2      | 2  | 2  | 4     | 2           | 0           | 0   |
| P3      | 4  | 8  | 23    | 19          | 11          | 11  |
| P4      | 6  | 3  | 9     | 3           | 0           | 0   |
| P5      | 8  | 4  | 15    | 7           | 3           | 3   |

#### Kết quả:
- **Average Waiting Time** = (5+0+11+0+3)/5 = **3.8**
- **Average Turnaround Time** = (11+2+19+3+7)/5 = **8.4**
- **Average Response Time** = (0+0+11+0+3)/5 = **2.8**

#### Nhận xét:
- Average WT tốt nhất: 3.8
- P1 bị preempt 2 lần → context switching overhead
- P3 vẫn bị starvation nghiêm trọng

---

### 3.4 Áp dụng Round Robin (Time Quantum = 4)

#### Nguyên tắc:
Mỗi process chạy tối đa 4 đơn vị thời gian, sau đó quay về cuối queue

#### Quá trình:
- **t=0-4**: P1 chạy 4 (còn 2), vào cuối queue
- **t=4-6**: P2 đến t=2, P3 đến t=4. Queue: [P2, P3, P1]. Chạy P2 (2), xong
- **t=6-10**: Queue: [P3, P1, P4]. P4 đến t=6. Chạy P3 (4, còn 4)
- **t=10-12**: Queue: [P1, P4, P3]. P5 đến t=8. Chạy P1 (2), xong
- **t=12-15**: Queue: [P4, P3, P5]. Chạy P4 (3), xong
- **t=15-19**: Queue: [P3, P5]. Chạy P3 (4), xong
- **t=19-23**: Queue: [P5]. Chạy P5 (4), xong

#### Gantt Chart:
```
| P1 | P2 | P3 | P1 | P4  | P3 | P5   |
0    4    6   10   12   15   19    23
```

#### Tính toán:

| Process | AT | BT | CT | TAT = CT-AT | WT = TAT-BT | RT |
|---------|----|----|-------|-------------|-------------|-----|
| P1      | 0  | 6  | 12    | 12          | 6           | 0   |
| P2      | 2  | 2  | 6     | 4           | 2           | 2   |
| P3      | 4  | 8  | 19    | 15          | 7           | 2   |
| P4      | 6  | 3  | 15    | 9           | 6           | 6   |
| P5      | 8  | 4  | 23    | 15          | 11          | 11  |

#### Kết quả:
- **Average Waiting Time** = (6+2+7+6+11)/5 = **6.4**
- **Average Turnaround Time** = (12+4+15+9+15)/5 = **11.0**
- **Average Response Time** = (0+2+2+6+11)/5 = **4.2**

#### Nhận xét:
- Average WT cao hơn SJF/SRTF nhưng công bằng hơn
- Response Time tốt cho P2, P3
- Không có starvation
- Context switching nhiều (6 lần)

---

### 3.5 Áp dụng Priority Scheduling (Non-preemptive)

#### Nguyên tắc:
Tại mỗi thời điểm CPU rảnh, chọn process có priority cao nhất (số nhỏ nhất)

#### Quá trình:
- **t=0**: P1 chạy (priority=3)
- **t=6**: P1 xong. P2(1), P3(4), P4(2) đã đến. Chọn P2 (priority=1)
- **t=8**: P2 xong. P3(4), P4(2), P5(5) đã đến. Chọn P4 (priority=2)
- **t=11**: P4 xong. Chọn P3 (priority=4 < 5)
- **t=19**: P3 xong. Chọn P5

#### Gantt Chart:
```
| P1  | P2 | P4  | P3      | P5   |
0     6    8    11       19    23
```

#### Tính toán:

| Process | AT | BT | Priority | CT | TAT | WT | RT |
|---------|----|----|----------|-------|-----|-----|-----|
| P1      | 0  | 6  | 3        | 6     | 6   | 0   | 0   |
| P2      | 2  | 2  | 1        | 8     | 6   | 4   | 4   |
| P3      | 4  | 8  | 4        | 19    | 15  | 7   | 7   |
| P4      | 6  | 3  | 2        | 11    | 5   | 2   | 2   |
| P5      | 8  | 4  | 5        | 23    | 15  | 11  | 11  |

#### Kết quả:
- **Average Waiting Time** = (0+4+7+2+11)/5 = **4.8**
- **Average Turnaround Time** = (6+6+15+5+15)/5 = **9.4**
- **Average Response Time** = **4.8**

#### Nhận xét:
- Process được thực thi theo thứ tự priority: P1→P2→P4→P3→P5
- P5 có priority thấp nhất → chờ lâu nhất (starvation)
- Average WT = 4.8 (tốt hơn FCFS, RR nhưng kém SJF/SRTF)

---

### 3.6 Áp dụng Priority Scheduling (Preemptive)

#### Nguyên tắc:
Khi có process mới đến, nếu priority cao hơn process đang chạy → preempt

#### Quá trình:
- **t=0-2**: P1 chạy (priority=3)
- **t=2**: P2 đến (priority=1 < 3), preempt P1, chạy P2
- **t=4**: P2 xong, P3 đến (priority=4). P1 còn lại có priority=3 < 4, chạy P1
- **t=6**: P4 đến (priority=2 < 3), preempt P1, chạy P4
- **t=9**: P4 xong. P1(3) < P3(4) < P5(5), chạy P1
- **t=11**: P1 xong. Chạy P3
- **t=19**: P3 xong. Chạy P5

#### Gantt Chart:
```
| P1 | P2 | P1 | P4  | P1 | P3      | P5   |
0    2    4    6     9   11       19    23
```

#### Tính toán:

| Process | AT | BT | Priority | CT | TAT | WT | RT |
|---------|----|----|----------|-------|-----|-----|-----|
| P1      | 0  | 6  | 3        | 11    | 11  | 5   | 0   |
| P2      | 2  | 2  | 1        | 4     | 2   | 0   | 0   |
| P3      | 4  | 8  | 4        | 19    | 15  | 7   | 7   |
| P4      | 6  | 3  | 2        | 9     | 3   | 0   | 0   |
| P5      | 8  | 4  | 5        | 23    | 15  | 11  | 11  |

#### Kết quả:
- **Average Waiting Time** = (5+0+7+0+11)/5 = **4.6**
- **Average Turnaround Time** = (11+2+15+3+15)/5 = **9.2**
- **Average Response Time** = (0+0+7+0+11)/5 = **3.6**

#### Nhận xét:
- P2 và P4 (high priority) được ưu tiên → WT = 0
- P1 bị preempt 2 lần
- P5 (lowest priority) chờ lâu nhất → starvation
- Average WT tốt hơn non-preemptive priority (4.6 vs 4.8)
