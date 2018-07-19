![](https://)# Operating System 
# 有這份筆記不代表可以不看影片喔！
[Text Book: Operating System Concepts](http://materias.fi.uba.ar/7508/OSC9/Abraham%20Silberschatz-Operating%20System%20Concepts%20(9th,2012.12).pdf)
---
[TOC]

---
# Ch 01 Introduction
### System Goal
- convenient for the user
- Efficient operation of the computer system
- 需要衡量效率和界面的比重和預測以後系統的走向。 
### Booting
- bootstrap program (開機就直接跑)
- operating system (some init, eg login,daemon)
### Interrupt (影片23:00)
#### Hardware interrupt:
- service request of I/O devices
#### Software interrupt:
- signals
- invalid memory access
- 除以0
- system call
- etc - (trap)


處理方式：interrupt vector
中斷服務一進來的時候，會去看中斷碼然後直接跳到相對應的中斷服務程式
缺點：interrupt vector 的數目是有限的

處理方式：generic handler
可以一次接受多個中斷服務

目前的系統通常兩者折衷

---
### Interrupt Handling Procedure
![](https://i.imgur.com/YQJZy4h.png)

![](https://i.imgur.com/YTljvPq.png)
:::info
老師影片語錄擷取（不要動！）
中斷來的時候，把暫時的狀態記在某個固定位址，問題是中斷服務程式在跑的時候又有另一個中斷服務進來的話，會覆蓋掉原本固定位址的東西，因此給固定位址的方式是不可行的。
如果使用stack的方式，還是有可能出問題，比如說某個中斷進來之後又被中斷，現有的資料可能會被弄壞，所以新的中斷如果會干擾現有中斷服務的時候，要先讓其他中斷不能進來。
中斷暫停的機制是使用一個晶片，上面有不同的bit，代表不同的中段型態，然後有一條線接到CPU，中斷進來的時候，相關的bit會被set(masking)，CPU能根據訊號種類是否被set決定是否被中斷。遇到的問題是，若同樣的中斷多次進來，需要另外記錄，就用個counter記錄次數......(滿複雜的，也有可能同樣的中斷好幾個要進來，就會漏掉)。有些中斷跟時間有關(timer 的 interrupt)，這種中斷進來時，中斷服務會去maintain時間，如果把這個中斷擋在外面，那麼系統時間就會出問題。

中斷服務要注意的幾點：
- 中斷目前狀態要如何記起來
- 中斷進來之後應該要跑哪個中斷服務程式？（interrupt vector, generic handler）
:::

### I/O structure
#### I/O operation
- CPU sets up specific controller registers within the controller
- Read資料: device $\rightarrow$ controller buffer $\rightarrow$ memory
- Write資料: memory $\rightarrow$ controller buffer $\rightarrow$ device
- 以上箭頭間的資料搬移，由controller進行
- 用interrupt來通知已經完成
***但是這樣是有問題的！！！***
中斷可能會很多（例如網絡卡），CPU一直被中斷，於是有了DMA
### DMA (direct memory access)
- controller負責I/O Device與Memory之間的資料傳輸，其過程完全不需CPU參與，CPU就有更多時間用在process執行
- 想法：如果你會常常中斷我，我就找一個幫我做這件事情，做完再叫我
- DMA可以想像是一個在device controller裡面的一個晶片，CPU對controller下命令說要寫東西出去，DMA controller自己去記憶體拿東西搬到自己controller的buffer裡面，重複做好幾次直到完成之後再跟CPU說做好了。

---
### Operating System Structure
- batch
    - Batch processing is the execution of a series of jobs in a program on a computer without manual intervention.
- spooling (Simultaneously Peripheral Operation On-line)
    - Replace sequential-access devices with random-access devices
    - In computing, spooling is a specialized form of multi-programming for the purpose of copying data between different devices. In contemporary systems it is usually used for mediating between a computer application and a slow peripheral, such as a printer.
    - For example, when a large organization prepares payroll checks, the computation takes only a few minutes or even seconds, but the printing process might take hours. If the payroll program printed checks directly, it would be unable to proceed to other computations until all the checks were printed.
    - (投影片內容)讀卡機和CPU的速度差太多，為了有效運用，我們在兩者間放了disk，讀卡機讀的內容寫入disk，CPU在一次讀取。同樣的狀況也能用在CPU和printer之間。這個概念在現在的電腦也以不同的方式留下來且實現。
- multi-programming
    - 增加CPU效率，在面對眾多工作時，我們用多功(multi-programing)來安排工作間的交換與優先度。
        - job scheduling and CPU scheduling
        - efficient use of scarce resources
- time sharing (multitasking) 不停切換工作
    - logical extension of multi-programming
    - an interactive (hand-on) computer system，藉著CPU快速切換工作，同時每個程式都在進行。
    - 可以讓人開始互動式地使用電腦（開始有online file system）
- virtual memory
  - 之後討論
#### Hardware Protection
- Goal: prevent errors and misuse (一個人的程式弄坏system 或者是下一個程式)
- Dual mode operation - a mode bit
    - user mode
    - monitor mode (system/privileged/supervisor mode)
- Virtual mode (for virtual machine manager)
    - Virtual user mode
    - Virtual monitor mode

- virtual machine
    - 早期的概念：多設立一層VM software，藉著快速切換，讓上層系統與應用程式以為自己是真的在印體上的一台獨立系統。
    - 最接近硬體的一定是monitor mode
    - virtual monitor/user mode 實體上都是 user mode
    - ![](https://i.imgur.com/A5mkqdx.png)

- System calls: 使用者可自行進行加減乘除運算，但當要進行I/O之類的行動時，要藉著trap請OS執行privileged mode.
- 為了保護硬體等設備，當使用者要I/O，不能以user mode直接進行，要跟OS請求，由OS切到system mode再I/O

#### Memory Protection

![](https://i.imgur.com/3M7JSyk.png)

- user program不能修改別人的memory，不能修改OS的code，不能擅自搬移出自己被分配的位置

#### CPU Protection

![](https://i.imgur.com/h7UJPmz.png)

- timer 的 reading 不需要是 privileged instruction，但 timer 的修改是
- context switch 把一個程式停下來 load 另一個程式進來

---

### Process Management

- Process is an active entity
    - Physical and logical resourses: Memory, IO buffer, data
    - Data structure: Program counter, Stack, data section, CPU registers...

---

### Caching

![](https://i.imgur.com/kaHrluv.png)


- information is copied to a faster storage system on a temporary basis (assumption: data will be used again soon)
- Cache Management
    - Size and Replacement Policy
    - 避免把常用的東西推出去，把不常用的東西load進來
- 問題: multitasking 會有cache還沒寫回去的狀況
- 理論上不同層之間某一筆資料看到的值可能是不一樣的，系統看值的時候是從上往下看(2:31:00左右)所以不會有問題
- 但是不同thread的時候可能會出問題，所以就有Coherency and Consistency 的問題
- Coherency and Consistency(讀到資料必須一樣)
    - 垂直：multitasking, unitasking 
    - 水平：multiprocessor, distributed systems
### Protect and Security

- 目標: authorized program 只能拿取合法的資料 (資料衹能被合法的程序拿取)
- Protect: 系統定義了資料的所有權，然後要有一個機制來規定誰能拿取那些資料
    - 要protect 的資料
        - file system
        - CPU, memory space, I/O device(hardware protection)
    - 分類users : ID,GID (用setuid to gain extra permission) 
- Security: 防止來自外部或內部的攻擊
    - virus
    - denial of service

---

### Computing Environment --- Virtual Machine
- Implementation Issue
    - 模擬硬體環境 ex: 一張網路卡當兩張用
    - Modes:
        - (Physical) Moniter Mode: 虛擬機軟體
        - (Physical) User Mode: virtual moniter mode & virtual user mode
- kernel 和 hardware 都以為對方是真實存在，其實中間的指令( trap / system call ) 是經過中間的VM軟體控制
- VM上的kernel變成virtual monitor mode時仍屬於physical user mode，是藉由VM軟體進行切換才會變成physical monitor mode.
- 缺點
    - 慢: 但是能模擬過氣的OS(obsolete hardware)、一台電腦跑多個VM，需要時再把其中一個系統搬到獨立電腦
    - 沒有真實的硬體: 但是在VM間進行交流可以不經過硬體、網路卡之類的可以共用
- 優點
    - 完全保護: 因為獨立封閉，不怕污染電腦
    - 系統合併(System Consolidation)
    - Extensions to Multiple Personalities, such as Mach (software emulation)
        - Emulations of Machines and OS’s, e.g., Windows over Linux
    - System Consolidation


### Computing Environment --- Cloud Computing

- Cloud Computing - Delivers computing,
storage, and even applications as a
service across a network
- Types
    - Public, Private and Hybrid Clouds
    - Software as a service (SaaS), e.g. Gmail.
    - Platform as a service (PaaS), e.g. database server.
    - Infrastructure as a service (IaaS), e.g. storage for backup.

---

# Ch02 System Structure
### OS services
- 如果下很多對硬碟讀寫的命令的話，他會做個排序，磁碟機的磁頭就不需要一直來回移動

### User OS Interface – Command Interpreter

- 指令執行的兩種方法
    - 一個大的Command Interpreter存了所有指令和其code，快速但是它將會非常大，而且更新時整個檔案要修改
    - 每個指令都有一個執行檔但是有幾個缺點
        - 參數傳遞(-a -l ...)
        - 慢 （fork一個process去執行exec）
        - 執行檔放的位置不對，會跑錯版本
        - cd 是在interpreter裡，不然路徑會出錯

### User OS Interface - GUI

- 開始有界面，熒幕
- 問：GUI 是不是應該要屬於作業系統的一部分？答：不是，unix&linux 有很多不同種的 GUI。 
- Trend, Mixture of command line 和 GUI.

### System call

- Parameter passing
    - Register(很貴): 太多東西要傳會很沒有效率，e.g. 路徑
    - Register point to blocks: linux
    - Stack: 依序push進去，再拿出來

### System Design & Implementation

- Policy & Mechanism: e.g.  timer
    - Policy: 誰的程式能跑多一點時間
    - Mechanism: 如何實作
- 用assembly language 寫: 當要進行快速的硬體等。
- 用高階語言編寫
    - 優點: 易懂、易寫
    - 缺點: 檔案大、比較沒效率

### OS Structure

- MS-DOS

![](https://i.imgur.com/OO3XiZo.png)

- UNIX

![](https://i.imgur.com/3LV5RTN.png)

- Layered
    - 優點: 易debug、模組化
    - 缺點: overhead、那些功能要放哪層

- Microkernel
    - 把大部分功能從kernel移到user program
    - OS小易搬移、kernel較安全(不會因為部分功能被攻擊，系統就crush)
    - 通常一定會放在kernel內的功能: process management、communication、memory management

- Modules
    - 和layer剛好相反，平行並列

- Hybrid System
![](https://i.imgur.com/7pedtQf.png)
![](https://i.imgur.com/uXGpqmx.png)

### Operating System Debugging

- Performance Tuning: A Procedure that seeks to Improve Performance by Removing Bottlenecks.
- Core Dump: A Capture of the Memory of a Process or OS
- Crash: A Kernel Failure

### System Generation(SYSGEN)
- Ask and probe for information concerning the specific configuration of a hardware system.


# Ch03 Process Concept
### Process
- PID 0 – Swapper (i.e., the scheduler)
    - Kernel process
    - No program on disks correspond to this process
- PID 1 – init responsible for bringing up a Unix system after the kernel has been bootstrapped. (/etc/rc* & init or /sbin/rc* & init)
    - User process with superuser privileges
- PID 2 - pagedaemon responsible for paging
    - Kernel process

- Process: 正在執行的程式
- Active Entity: Process
- Passive Entity: code
- Process Control Block(PCB)
    - OS為執行Process management，所以將process所有相關的資訊集中，建立一個集合(table)表示，稱為PCB
    - PID, program counter, 優先權......
    - Unix分兩部分
        - proc[i]: process的重要資訊
        - .u: process執行時才需要知道的, e.g. signal, file......
![](https://i.imgur.com/TfAs7zR.png)

### Process Scheduling – Schedulers
- long-term(/JOB) Schedule: IO-bound 和 CPU-bound間的安排
- short-term(/CPU) Schedule: 根據優先權，選擇當下誰來用CPU(e.g. swappere)
- mid-term Schedule: Swap processes in and out memory to
control the degree of multiprogrammingSwap processes in and out memory to control the degree of multiprogramming

### Process Scheduling – Context Switches
- 把程式暫停，換另外一個程式跑

### Interprocess Communication – Buffering
- Zero capacity(no buffering)
    - Msg transfer must be synchronized – rendezvous!
    - 送出訊息要馬上讀
- Bounded capacity
    - Sender can continue execution without waiting till the link is full
- Unbounded capacity
    - Sender is never delayed!
    - 永遠不會滿的mainl box，不會block

### Interprocess Communication – Exception Conditions
- Scrambled Messages: Usually OS adds checksums, such as CRC, inside messages & resend them as necessary!

```C
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
int shm_open(const char *name, int oflag, mode_t mode);
int shm_unlink(const char *name);
```
- 在/dev/shm下建立文件(share memory)

```C
#include <unistd.h>
#include <sys/types.h>
int truncate(const char *path, off_t length);
int ftruncate(int fd, off_t length);
```
- 改變文件大小，太多直接刪掉，太少用'\0'補

```C
#include <sys/mman.h>
void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
int munmap(void *addr, size_t length);
```
- 把文件(fd)map到kernel內的一塊記憶體，之後IO文件比較快

### Communication in Client-Server Systems
- Remote Procedure Call (RPC)
    - A way to abstract the procedure-call mechanism for use between systems with network connection.
- Needs:
    - Ports to listen from the RPC daemon site and to return results, identifiers of functions to call, parameters to pack, etc.
    - Stubs at the client site
        - One for each RPC
        - Locate the proper port and marshall parameters.
    - Stubs at the server site
        - Receive the message
        - Invoke the procedure and return the results.
- Issues for RPC
    - Data representation
        - External Data Representation (XDR) 轉成一個共通標準，傳到後自己再轉回來
            - Parameter marshalling
    - Semantics of a call
        - History of all messages processed 防止同一個指令傳兩次(記住已經做過了)
    - Binding of the client and server port
        - Matchmaker – a rendezvous mechanism : port的查詢系統
![](https://i.imgur.com/uwIRuY2.png)
    - Remote Method Incocation(RMI)
        - allow a thread to invoke 一個 method 在你控制的平臺。
        - client 端呼叫stub
        - server 端呼叫 skeleton(用於解組參數和調用方法以及發送包裹)
    - Parameter Passing (在java 上有一點點不同)
        - Local (pass-by-copy)
        - Remote objects (different java virtual machine JVM) passing "stub"
- Pipes
    - unidirectional(單向)，bidirectional (雙向)。
    - half duplex(一邊講一邊聼，ex 對講機), full duplex(可以兩邊同時講和聼)
    - 需不需要有關係（例如 parent&child）
    - 只能在内部還是也可以供應外部？
    - Ordinary Pipes, (用在 parent & child) 當parent 在fork 之前就先開一個pipe, 然後parent 端關掉read, child 關掉write. 在windows 叫做 Anonymous Pipe.
    - Named Pipes (Bidirection Pipes) 有名字，所以不一定要是 parent 和child 的關係。可以很多人access. 例如：對講機。 windows(createnamepipe,full duplex), Unix(mkfifo, half duplex)
    - consider as message passing, indirect communication
    - reader: synchronous. writer: asynchronous 
---

# Ch04 Multithreading Programming

- 在同一個process裡面創造新的thread比創造新的process快30倍，ㄙcontext swicth 快5倍，因為share code segment 跟 data segment
- 一個程式有很多的thread就有排程的問題，資源分配的好才能達到最佳平行化效率
- Thread 別名 lightweight process(LWP).

### Multicore Programming
- Data Parallelism
    - Distribute data over cores to execute the same operation
        - 100萬個數字分成十等分分別先sort完再merge
- Task Parallelism
    - Distribute work over cores over the same or different data
        - 100萬個數字找最大、最小、平均、中位數......，分給不同core做

### User level Thread
- User-level threads are implemented by a thread library at the user level
    - Context switch is very fast because it doesn't need kernel services
    - 如果現在有個user-level thread 叫用system call 跳到作業系統裡面block住的話，作業系統看不到其他的user-level thread 因此全部就被block住
### Kernel level Thread
- Kernel-level threads are provided a set of system calls similar to those of processes
    - kernel 要 switch, 所以 context switch稍微慢一點
    - 每個作業系統都能看到thread，某一個block住不影響其他的
### Thread Models
- 1:1 Model 每個user level thread 都對應到一個 kernel level thread
    - 優點：one system call blocks one thread
    - 缺點：overhead in creating kernel thread
- N:1 Model 所有的user level thread都對應到一個kernel level thread
    - 優點：效率高(Context switch 在 user level)
    - 缺點：一個block全block，kernel 看不到也就沒有 multiple processors 的平行化
- M:N Model M個user level thread對應到N個kernel level thread（N通常比較小）
    - 優點：平行化 and 效率可自由選擇
### Thread Library
![](https://i.imgur.com/Znuf8bY.png)

### Implicit Threading
- Objective: Transfer the creation and management of the threading from the application developers to compilers and run-time libraries.
- thread pool
    - 動態創造有限的thread
    - awake and pass a request to a thread in the pool
    - 好處: Faster for service delivery, 限制住thread數量

### Threading Issue
- Thread cancellation
    - two scenarios:
        - Asyncronous cancellation: 收到取消要求立刻取消
        - Deferred cancellation: 可能因為在修改一些資料，必須等到安全時才取消

### Scheduler Activation
![](https://i.imgur.com/UrmK3Mi.png)
- kernel提供一種特別的功能，當user level thread有要block住時，kernel通知 'upcall handler' 去創造新的kernel level thread，接收user level thread，避免全部都被block.

# Ch5 Process Scheduling

### Process Execution
- CPU-bound programs: 有幾個很長的CPU bursts.
- IO-bound programs: 有很多短短的CPU bursts.

### CPU Scheduling
![](https://i.imgur.com/ZJkF42Z.png)
- Nonpreemptive Scheduling
    - 非侵占式，跑到結束(I/O or terminate)，easy implement.
- Preemptive Scheduling
    - 侵占式
    - In computing, preemption is the act of temporarily interrupting a task being carried out by a computer system, without requiring its cooperation, and with the intention of resuming the task at a later time. Such changes of the executed task are known as context switches. It is normally carried out by a privileged task or part of the system known as a preemptive scheduler, which has the power to preempt, or interrupt, and later resume, other tasks in the system.
- Dispatcher
    - 功能: context switch


## Scheduling Algorithm

### First-Come, First-Served Scheduling (FCFS)
- Properties:
    - Non-preemptive scheduling
    - CPU might be hold for an extended period.
- Convoy Effect: 有很多程式等待某個大程式完成之後，才能繼續工作。
### Shortest-Job-First Scheduling(SJF) (Non-preemptive)
- 時間0時都準備好，就是optimal
    - average waiting time 最短
- 實際上我們不知道需要跑多久
    - short-term scheduler: exponential average
    - $\tau_{n+1}=\alpha t_n+(1-\alpha)\tau_{n}$ , $0\le\alpha\le1$
    - $t_n$ : 過去的紀錄（第n段要跑的時間）. $\tau_n$ : 現在的紀錄
    - $\tau_{n+1}$ : 預測下一段要跑多久, $\alpha$ : weight

### Shortest-Job-First Scheduling(SJF) (Preemptive)
- arrival time 不一樣，時間0的時候不是所有人都ready

![](https://i.imgur.com/YC8ckuL.png)

![](https://i.imgur.com/aAvpusv.png)
- 看起來 preemptive 比較好，但是 context switch 也要考慮進去

### Priority Scheduling
- Priority Assignment
    - Internally defined – use some measurable quantity, such as the # of open files.
    - Externally defined – set by criteria external to the OS, such as the criticality levels of jobs.
- Preemptive or Non-Preemptive?
    - Preemptive scheduling: 當有新的工作進入QUEUE或是CPU被running process釋出，才會調用CPU scheduling.
    - Non-preemptive scheduling: 當CPU被running process釋出，才會調用CPU scheduling.
- Major Problem
    - Indefinite Blocking (/Starvation)
        - 低優先度的可能永遠等不到機會
    - 解決方法: Aging，等越久的priority會越來越高。

### Round-Robin Scheduling (RR)
- 每個程式執行一段time slice後就放到queue最後面
- Goal: 公平，time sharing
- Turnaround Time: 從第一次開始到全部結束的時間
![](https://i.imgur.com/f6KM5qt.png)

### Multilevel Queue Scheduling
- 根據類型分成很多個ready queue
- Intra-queue scheduling(queue內)
    - 不同類型的queue有自己的規則
- Inter-queue scheduling(queue之間)
    - Fixed-priority preemptive scheduling
        - e.g., 前景queue永遠優先於背景queue 
    - Time slice between queues
        -  e.g., 80% CPU is given to foreground processes, and 20% CPU to background processes.
:::info
### Foreground-background (補充)
Foreground-background is a scheduling algorithm that is used to control an execution of multiple processes on a single processor.  ***It is based on two waiting lists, the first one is called foreground because this is the one in which all processes initially enter, and the second one is called background because all processes, after using all of their execution time in foreground, are moved to background.*** (所以有在queue之間移動？？？)

When a process becomes ready it begins its execution in foreground immediately, forcing the processor to give up execution of the current process in the background and execute the newly created process for a predefined period. This period is usually 2 or more quanta. If the process is not finished after its execution in the foreground it is moved to background waiting list where it will be executed only when the foreground list is empty. After being moved to the background, the process is then run longer than before, usually 4 quanta. The time of execution is increased because the process obviously needs more than 2 quanta to finish (this is the reason it was moved to background). This gives the process the opportunity to finish within this newly designated time. If the process does not finish after this, it is then preempted and moved to the end of the background list.

The advantage of the foreground-background algorithm is that it gives the process the opportunity to execute immediately after its creation, but scheduling in the background list is pure round-robin scheduling.
:::
### Multilevel Feedback Queue Scheduling
- 和Multilevel Queue Scheduling的差異在於可以將工作在queue之間搬移
- 配置參數：
    - number of queues
    - scheduling algorithm for each queue
    - The method to determine when to upgrade(demote) a process to a higher(lower) priority queue
    - The method to determine which queue a new process will enter

https://www.geeksforgeeks.org/operating-system-multilevel-queue-scheduling/
https://www.geeksforgeeks.org/multilevel-feedback-queue-scheduling/


### Thread Scheduling
- Two Scopes:
    - Process Contention Scope (PCS): m:1 or m:n (投影片寫m:m？？？跟1:1差在哪？？)
        - Priority-Driven
    - System-Contention Scope (SCS): 1:1
    - M:1 通常是 user level , 1:1 是 kernel level, M:n 是 light weight process (Ex thread 要怎么連之類的)
- Pthread Scheduling
    - PCS and SCS

### Multiple-Processor Scheduling 
[詳細介紹](https://www.geeksforgeeks.org/operating-system-multiple-processor-scheduling/)

- 多顆CPU的系統
- A Homogeneous System
    - 每顆CPU一模一樣
- A Heterogeneous System
    - 可能有不同CPU、GPU，不同CPU編譯的程式不能交換執行
- Load Sharing – Load Balancing
    - 每個processor各有各的queue
        - Self-Scheduling – Symmetric Multiprocessing
    - 所有processors共用queue
        - Self-Scheduling
            - Need synchronization to access common data structure, e.g., queues.
        - Master-Slave – Asymmetric Multiprocessing (適用於CPU超多時)
            - One processor accesses the system structures
            - no need for data sharing
- Load Balancing
    - Push migration: 有一個process專門把太忙的processor的工作分給空閒的processor
    - Pull migration: processor主動把別的processor的工作分過來做
    - Processor affinity vs imbalance threshold
- [Processor Affinity](https://en.wikipedia.org/wiki/Processor_affinity)
    - Processor affinity, or CPU pinning, enables the binding and unbinding of a process or a thread to a central processing unit (CPU) or a range of CPUs, so that the process or thread will execute only on the designated CPU or CPUs rather than any CPU.
    - The system might avoid process migration because of the cost in invalidating or repopulating caches把資料搬移進process的成本可能太大，不如相同的工作集中給同樣的process不要分給其他人
    - cache populate: 把資料從memory裡經過一層層cache往上搬
    - soft affinity:process可以在processor之間轉移
    - hard affinity:process不能在processor之間轉移
- [Symmetric(Simultaneously) Multithreading (SMT)](http://blog.logicalincrements.com/2017/10/what-is-hyper-threading-simultaneous-multithreading/), i.e., Hyperthreading 
    - 由硬體提供的特色
    - Several logical processors per physical processor
    - Each has its own architecture state, including registers
    ![](https://i.imgur.com/60S3sXu.png)
    - process的state往下層長，但有時候不會把一整層的unit用完，右邊是同時跑多個process，一起往下長，用其他空閒的unit平行工作
    - [superscalar](https://en.wikipedia.org/wiki/Superscalar_processor)
        - a superscalar processor can execute more than one instruction during a clock cycle by simultaneously dispatching multiple instructions to different execution units on the processor
    - [Instruction pipelining](https://en.wikipedia.org/wiki/Instruction_pipelining)
- Memory Stall: processers等待資料搬來(OS做排程)
    - Multiple Hardware Threads: 如上面的圖，紅色區域在等待就先給黃色做
    - Coarse-Grained Multithreading: 有人需要等待cache就先讓別人做
        - Thread execution until a long latency
    - Fine-Grained Multithreading: 隨時一直換人工作，如同RR
        - Better architecture design for switching
- Two-Levels of Scheduling(硬體做排程)
    - OS chooses to run a software thread.
    - Each core decides to run which hardware thread – round robin (UltraSPARC T1) or dynamic urgency (Itanium)

### Virtualization and Scheduling
請看影片[1:54:00](https://www.youtube.com/watch?v=autw3JQLmms&feature=youtu.be)左右

### Real-Time CPU Scheduling
- Soft Real-Time Systems: 讓有即性的process有較高優先權，重要的有較高優先權
- Hard Real-time Systems: 對完成時間有嚴格的要求，死線前一定要完成
- Event Latency: 從event發生到系統對於event反應之間的時間
    - Interrupt Latency 
        - Interrupt latency refers to the period of time from the arrival of an interrupt at the CPU to the start of the routine that services the interrupt.
    - Dispatch Latency 
        - The amount of time required for the scheduling dispatcher to stop one process and start another.
    ![](https://i.imgur.com/rff9io0.png)
- periodic process: require the CPU at constant intervals (periods)
- Once a periodic process has acquired the CPU, it has a fixed processing time t, a deadline d by which it must be serviced by the CPU, and a period p. 
- 0 ≤ t ≤ d ≤ p
- rate of a periodic task: $\frac{1}{p}$
- admission-control algorithm:
    1. admits the process, guaranteeing that the process will complete on time
    2. rejects the request as impossible if it cannot guarantee that the task will be serviced by its deadline
#### Priority-Based Scheduling
- Priority-Based Preemptive Scheduler: 不管怎樣一定要跑到的重要process
    - It might be ok for soft real-time processes.
    - Hard real-time systems need a guarantee!


#### Rate Monotonic Scheduling
- 固定優先權 (static priority policy with preemption)
- period(執行週期)越短，越常要做的，優先權越高 (each periodic task is assigned a priority inversely based on its period)
- Processing time of a periodic process is the same for each CPU burst. That is, every time a process acquires the CPU, the duration of its CPU burst is the same.
- Properties
    - Optimal Fixed-Priority Scheduler for Independent Processes(independent:隨時可以侵占式排程)
        - if a set of processes cannot be scheduled by this algorithm, it cannot be scheduled by any other algorithm that assigns static priorities. 
    - Achievable Utilization Factor: $N(2^{1/N}-1)$ for N processes, where the utilization of $\tau_i = c_i /p_i$
    - $N \rightarrow \infty$, utilization = 69%
        - e.g. 四個CPU最高75%，周期4執行時間2的使用率是50%，週期5執行時間1的使用率是20%，加起來70%，再高的話一定會有人miss掉deadline.
    - Critical Instant
        - An instance at which the process is requested simultaneously with requests of all higher priority processes . (worst case: 高優先權的程式全部一起來)
        - 如果 worst case都能完成，其他case就可以達成
    - Fully Utilization of the Processor Power (不能再增加process time，不然有人死線來不及)
#### Earliest Deadline First Scheduling
- 動態優先權
- 誰最快到他的deadline，越高的優先順序
- does not require that processes be periodic, nor must a process require a constant amount of CPU time per burst.
- process announce its deadline to the scheduler when it becomes runnable
- Properties
    - Optimal(theoretically optimal) Scheduler for Independent Processes
    - Achievable Utilization Factor: 100%
    - It can schedule processes so that each process can meet its deadline requirements and CPU utilization will be 100 percent. In practice, however, it is impossible to achieve this level of CPU utilization due to the cost of context switching between processes and interrupt handling.
- 缺點: 1.interrupt應該要高優先權2.不可能每一秒都在改優先權
#### Proportional Share Scheduling
- 總共CPU時間是Ｔ，有N個要分享，$N＜T$，保證每個應用程序將收到的總處理器時間$N/T$，事前把我們資源比例分配好

#### POSIX Real-Time Scheduling
- POSIX defines two scheduling classes for real-time threads:
    - SCHED_FIFO
    - SCHED_RR
    - SCHED_OTHER 
        - undefined
```C
pthread attr getsched policy(pthread attr t *attr, int *policy)
pthread attr setsched policy(pthread attr t *attr, int policy)

```
- The first parameter to both functions is a pointer to the set of attributes for the thread. The second parameter is either (1) a pointer to an integer that is set to the current scheduling policy (for pthread attr getsched policy()) or (2) an integer value (SCHED FIFO, SCHED RR, or SCHED OTHER) for the pthread attr setsched policy() function. Both functions return nonzero values if an error occurs.
#### Operating-System Examples
- Process Local Scheduling
    - thread scheduling is done locally to each application.
    - (就是說它的排程是application 自己定的,例如 IE, Google chrome)
    - 最佳例子， those user-level threads.
- System Global Scheduling
    - 是kernel 決定内部要怎麽排程。
    - 最佳例子, those kernel-level threads.
- Linux Ver. 2.5+ (考古題有考)
    - O(1) 因爲用紅黑樹的implementation. (若要找程式來跑就直接拿樹的左下方。但沒把整理樹的時間算下去，因爲整理樹不可能是O(1),所以衹是唬人罷了)
    - SMP，load balencing，processor affinity(對單一核心有親和力，比較希望同一個程式在同一核心上跑)
    - Completely Fair Scheduler (CFS) 
    - Numeric Priority (Real time tasks: 0~99,other tasks: 100~139)
    - nice value: -20~+19 越nice表示越容易把CPU讓給別人, 越nice,位置就會越在樹的右手邊，所以越難跑。
    - Targeted Latency (An interval to run the task at least once)
    - vruntime: how long the task has run
- Windows
    - Priorit Based Preemtive Scheduling
    - 有一dispatcher,讓 process 一直跑直到
        - 有一個higher-priority process 過來，
        - 他跑完了。
        - time quantum 結束了。
        - calls a blocking system call
    - Windows 7 introduces user-mode scheduling(thread scheduling without kernel 介入)
    - time quantum (different for I/0 devies), Favor foreground processes,ex: mouse.
- Solaris
    - priority-based process scheduling
        - Real-Time (100~159)越大越先跑
        - system (60~99)kernel-service processes
        - time sharing (0~59)
        - interactive (0~59), 
        - solaris 9 才有的class: Fair share (0~59), Fixed priority (0~59)
    - each LWP 跟他的parent 一樣class
    - Real-Time: guaranteed response
    - system: priorities of system processes are fixed
    - time-sharing: 越快跑完下次priority 越高，若time quantum 用完下次priority 越低
    - RR is used when several threads have same priority.
    - 160~169 留給 interrupts service.
### Algorithm Evaluation
#### Deterministic modeling
- 舉例，計算
- 快、簡單
- 需要算很多類型，且需要實際數字
#### Queuing models
- 用數學模型計算，e.g. distribution
- 實際上可能數學模型算不出來，而且常常會有一些太理想的假設(Poisson distribution)
#### Simulation
- 自己寫模擬code，用電腦跑
- code可能複雜，自己產生的workload可能不符合現實
#### Implementation
- 給user跑，看回饋
- user會很不爽被當白老鼠

# Ch6 Synchronization

- race condition:結果會depend on程式執行順序
- 解決方法:利用critical section
### The Critical-Section Problem
- Three Requirements
    - Mutual Exclusion:只有一個process可以進入critical section
    - Progress:
      - 除了在remainder section以外的程式可以決定誰可以進入critical section
      - selection不能等待無限久
    - Bounded waiting:一個process只能等待有限次就要能夠進入critical section
- Single Process ---- Peterson' s solution
    - ![](https://i.imgur.com/7nKMvec.png)
    - ![](https://i.imgur.com/RtacOUJ.png)
        - fails progress requirement
    - problem: P0若停止，P1永遠等不到turn輪到他
    - ![](https://i.imgur.com/iEzwEUB.png)
    - problem: 兩邊同時在flag寫入true，兩邊會同時被卡住
    - ![](https://i.imgur.com/V6lV4Tm.png)
    - 成功的重點: turn不是i就是j，不會卡住

- Multiple Process ---- Bakery Algorithm
    - 拿一個號碼，號碼最小的就能進入critical section
    - choosing[i]: Pi被選擇
    - ![](https://i.imgur.com/2tO4ktp.png)
### Synchronization Hardware
- 使用硬體的feature來同步
- Test and Set
    - ![](https://i.imgur.com/s0CmfD4.png)
    - bounding waiting 會失敗
- Swap
    - ![](https://i.imgur.com/OaicJ7a.png)
    - bounding waiting 會失敗
- 兩者結合
    - ![](https://i.imgur.com/4AjL81Q.png)
    - 離開的方法比較特殊
        - 沒人在等: lock = false
        - 有人在等: 他的waiting變成false，就能讓他優先進入critical section

### Mutex
- Atomic exection of require() and release()
```C
acquire() {
while (!available);
    /* busy wait */
    available = false;
}

release() { 
    available = true;
}

do {
    acquire(lock)
        critical section
    release(lock)
        remainder section 
} while (true);
```
- 缺點: busy waiting在while內等待
- 優點: 由於是busy waiting，不會context switch

### Semaphores
![](https://i.imgur.com/ljn5ywt.png)
- 好處: 能控制數量與工作間的先後
- synch = 0;
- ![](https://i.imgur.com/7ldd5jd.png)

![](https://i.imgur.com/eR7m71D.png)
- 用stuct實作，等待的工作放進queue
- |```S.value```|就是等待的個數
- 如果是用priority queue會違反bounded waiting
### Dead Lock and Starvation
- Dead Lock: ![](https://i.imgur.com/lMhF2n2.png)
- Starvation: queue strategy不是FIFO

### Priority Invertion
- Definition: '低優先權程式'鎖住資源，擋住'高優先權程式'

### Binary Semaphore
![](https://i.imgur.com/kJ1wfqH.png)
- 初始值：
    - S1 = S3 = 1
    - S2 = 0
    - C = 2
- 思考四個process的狀況，老師說會稍微修改後問你哪裡有問題
- 如果把```wait(S3)```、```signal(S3)```拿掉？？
```C
if(C < 0){
    signal(S1);
    wait(S2); //這兩個對調會有什麼影響？
}
```
### Bounded-Buffer Problem

- initial:
```C
    int n;
    semaphore mutex = 1;
    semaphore empty = n;
    semaphore full = 0
```
- the pool consists of n buffers, each capable of holding one item
- The empty and full semaphores count the number of empty and full buffers.
```C
//producer process.
do { . . .
    /* produce an item in next produced */
    ... 
        wait(empty);
        wait(mutex);
    ...
    /* add next produced to the buffer */
    ... 
    signal(mutex); 
    signal(full);
} while (true);

// consumer process.
do { 
    wait(full);
    wait(mutex);
    ...
    /* remove an item from buffer to next consumed */
    ... 
    signal(mutex);
    signal(empty);
    ...
    /* consume the item in next consumed */
    ...
} while (true);
```
- We can interpret this code as the producer producing full buffers for the consumer or as the consumer producing empty buffers for the producer.
### Classical Synchronizing Prooblem ---- Reader and Writer
- Assumption: Reader(share lock) Writer(exclusive lock)
- The first reader-writers problem
    - readers一直看，writer寫不了
- The second reader-writers problem
    - writer一直寫，reader看不了
- queue 裡面 Reader優先權要比 writer 高
 ![](https://i.imgur.com/wvrZ2VY.png)

```wait(mutex)```保護 ```readcount```
### Classical Synchronizing Prooblem ---- Dining Philosophers
![](https://i.imgur.com/g343txt.png)
![](https://i.imgur.com/s0Vpgkx.png)
- Dead Lock: 大家一起拿左邊的筷子
- Solution
    - 殺一個philosopher
    - 一次就要拿兩個
    - 奇數先拿右，偶數先拿左
- 但還是會Starvation，CPU可能永遠輪不到某人

### Critical Regions/Monitor
- 用高階語言避免寫```wait()```和```signal()```的失誤
- Region: 保證{}內區域一次做完不中斷
- Monitor: {}內含的一定一次一個跑而已
    - 去看關於dining-philosopher問題的monitor版本
    - 不會dead lock但會starvation
- Monitor(implementation by semaphores)
  ![](https://i.imgur.com/m2bbRij.png)
  - 用mutex保護monitor
  - next:程式等待的地方
  - 看到```next-count>0```代表有人在等待, 則呼叫```signal(next)```
- Process-Resumption Order:Queuing Mechanisms for a monitor and its condition variables
  - ![](https://i.imgur.com/p1CEErq.png)
  - 宣告一個monitor負責resource allocation,像之前semaphores一樣,在access resourse之前呼叫acquire,結束呼叫release
  - time可以決定這個process的resumption order

# Ch 07 Deadlock
- ==Deadlock== : A set of process is in a deadlock state when every process in the set is waiting for an event that can be caused by only another process in the set.
### System Model
- a process may utilize a resource in only the following sequence:
    1. **Request**. The process requests the resource. If the request cannot be granted immediately (for example, if the resource is being used by another process), then the requesting process must wait until it can acquire the resource.
    2. **Use**. The process can operate on the resource (for example, if the resource is a printer, the process can print on the printer).
    3. **Release**. The process releases the resource.
- A system table records whether each resource is free or allocated. 
- For each resource that is allocated, the table also records the process to which it is allocated. 
- If a process requests a resource that is currently allocated to another process, it can be added to a queue of processes waiting for this resource.
```C
/* thread one runs in this function */
void *do work one(void *param)
{
    pthread mutex lock(&first mutex);
    pthread mutex lock(&second mutex);
    /*
        * Do some work
    */
    pthread mutex unlock(&second mutex);
    pthread mutex unlock(&first mutex);
    pthread exit(0);
}

/* thread two runs in this function */
void *do work two(void *param)
{
    pthread mutex lock(&second mutex);
    pthread mutex lock(&first mutex);
    /*
        * Do some work
    */
    pthread mutex unlock(&first mutex);
    pthread mutex unlock(&second mutex);
    pthread exit(0);
}
```
:::info
In this example, thread one attempts to acquire the mutex locks in the order (1) first mutex, (2) second mutex, while thread two attempts to acquire the mutex locks in the order (1) second mutex, (2) first mutex.
Deadlock is possible if thread one acquires first mutex while thread two acquires second mutex.
Note that, even though deadlock is possible, it will not occur if thread one can acquire and release the mutex locks for first mutex and second mutex before thread two attempts to acquire the locks. And, of course, the order in which the threads run depends on how they are scheduled by the CPU scheduler.
This example illustrates a problem with handling deadlocks: it is difficult to identify and test for deadlocks that may occur only under certain scheduling circumstances.
:::

### Necessary Conditions
- A deadlock situation can arise if the following four conditions hold simultaneously in a system:
    1. **Mutual exclusion**. At least one resource must be held in a nonsharable mode; that is, only one process at a time can use the resource. If another process requests that resource, the requesting process must be delayed until the resource has been released.
    2. **Hold and wait**. A process must be holding at least one resource and waiting to acquire additional resources that are currently being held by other processes.
    3. **No preemption**. Resources cannot be preempted; that is, a resource can be released only voluntarily by the process holding it, after that process has completed its task.
    4. **Circular wait**. A set $\{ P_0 , P_1 , ..., P_n \}$ of waiting processes must exist such that $P_0$ is waiting for a resource held by $P_1$, $P_1$ is waiting for a resource held by $P_2$, ..., $P_{n−1}$ is waiting for a resource held by $P_n$, and $P_n$ is waiting for a resource held by $P_0$.
- The circular-wait condition implies the hold-and-wait condition, so the four conditions are not completely independent.


### Resource Allocation Graph
- 節點 : process 和 resource
- 邊 : request edge 和 assignment edge
- 如果每個resource都只有一個instance,找到一個loop就可以確定有deadlock,但是如果有多個instance要繼續看其他的loop
### Methods for Handling Deadlocks
- Make sure that the system never enters a deadlock state
  - Deadlock Prevention : 只要有一個loop就fail,但是這樣可能導致效率不佳
  - Deadlock Avoidance : 要使用資源時先宣告,系統可以避免發生deadlock
- Do recovery if the system is deadlocked : 可以允許deadlock發生,但是發生之後可以recovery解決掉deadlock
- Ignore the possibility of deadlock occurrences : 忽略deadlock,當發現系統慢下來的時候restart系統,或是kill程式,但是可能系統只是frozen而已
### Deadlock Prevention
- fail四種condition的其中一種
  - Mutual Exclusion : 很難避免,因為很多東西不能同時share來用
  - Hold and Wait : 一次全拿不然就不要拿,但是一次全拿的難度較高,而且會有Low Resource Utilization的問題
  - no preemption : 搶走別人的resource要save&restore別人的狀態,有一些東西可以搶有一些不能 eg CPU register & memory space 可搶， printers ，tape drives 不行。兩個Approach 
      - Approach 1, resource request, satisfied 就拿，否者就搶，然後要save & restore 別人的狀態。
      - Approach 2, 多了一點，若resource request unsatisfied, 查查看要的resources 是不是waiting 狀態，是的話就搶，不是的話就再等待一陣子才判斷要不要搶。所以Aprroach 2 比1 好一點。
  - circular wait : 將resource編號,每次只能往高樓層拿,如果要回頭拿東西,擇要釋放掉那個樓層以上的資源才能回頭往上。困難點，有沒有一個normal order of resource usages in a system. 很難決定resource 的順序,feasiblity 有問題。 
### Deadlock Avoidance
- 並不是每個人都會宣告資源,但是如果是一群認識的人寫作業系統,就比較容易實現
- 宣告有多少available resources, allocated resources, max demands of processes
- safe sequence : $\forall P_i,need(P_i)\leq Available+\sum\limits_{j<i}Allocated(P_j)$
- Resource-Allocation Graph Algorithm
  - ![](https://i.imgur.com/44EAsu0.png)
  - 分成3種edge,想要資源的時候先出現edge,分配之後就變成assignment edge
  - graph中的cycle detection等同於有沒有safe sequence
- Banker's Algorithm
  - 找到need$\leq$work的process,然後將allocated加進work中,直到全部的finish變成T
  - Resource-Request Algorithm
    - need$\geq$request
    - request$\leq$available
    - 減去要求之後看是否safe
### Deadlock Detection
![](https://i.imgur.com/oRooqU5.png)
- 只有一個instance
  - 簡化graph : $P_i \to R_q \to P_j$變成$P_i \to P_j$,找cycle會變得比較簡單
  - detect an cycle in O(n^2)
- 多個instance
  - 只檢查現在有沒有deadlock,因此做比較樂觀的假設,只要$work \geq request$,就當作可以做完,把allocation放入work中
  - m= # of resource type, n= # of processes.
  - complexity= O(m*n^2)
### Deadlock Recovery
- 可能是operator來處理或是由system來處理
- abort某些程式來解決deadlock,或是preempt resources from deadlocked processes
- Process Termination
  - abort所有deadlocked process
  - 一次殺一個,直到deadlock解決,但是每次都要跑一次algorithm
- 選取被殺掉的程式
  - priority
  - CPU time : 跑最久的先不要殺掉
  - resource type : 拿到比較難拿到的resource的process先不要殺掉
  - interactive process : 可能會讓user frustrated
  - 要被abort的process數量
- Resource Preemption
  - Selecting a victim
  - Roll-Back : 有的process可能很難回復到原本state
  - Starvation : 每次都preempt同一個程式
### Combined Approaches
- 把resource分群,控制群內的deadlock和群間的deadlock
# Ch 08 Memory-Management Strategies
### Background
確保只 access 合法的位址：
- base register: holds the smallest legal physical memory address
- limit register: specifies the size of the range.
![](https://i.imgur.com/7JgnDp5.png)
- only the operating system can load the base and limit registers
![](https://i.imgur.com/QkbHMzo.png)
#### addresses
- relocatable:
    - "Get the byte located 12 bytes from this instruction"
    - “14 bytes from the beginning of this module”
- absolute: 
    - "Get the byte located at address 255"
#### binding
- address binding : binding of instructions and data to memory addresses
- 三種 bindind time
  - ==compile time==
      - If you know at compile time where the process will reside in memory, then absolute code can be generated. For example, if you know that a user process will reside starting at location R, then the generated compiler code will start at that location and extend up from there. If, at some later time, the starting location changes, then it will be necessary to recompile this code.
  - ==load time==
      - If it is not known at compile time where the process will reside in memory, then the compiler must generate relocatable code. In this case, final binding is delayed until load time. If the starting address changes, we need only reload the user code to incorporate this changed value.
  - ==execution time==
      - If the process can be moved during its execution from one memory segment to another, then binding must be delayed until run time. Most general-purpose operating systems use this method.
### Logical(Virtual) Versus Physical Address
![](https://i.imgur.com/uTE0kE1.png)
- ==logical address==: An address generated by the CPU.
- ==physical address==: An address seen by the memory unit—that is, the one loaded into the memory-address register of the memory.
- ==logical address space==: The set of all logical addresses generated by a program.
- ==physical address space==: The set of all physical addresses corresponding to these logical addresses.
- CPU不斷送出logical address,透過MMU(hardware) map到physical address
- 如果是在compile time或load time進行binding的程式就不需要MMU來進行mapping
- **The compile-time and load-time address-binding methods generate identical logical and physical addresses.**
- **The execution-time address-binding scheme results in differing logical and physical addresses.**
- The base register is now called a ==relocation register==. The value in the relocation register is added to every address generated by a user process at the time the address is sent to memory (見上圖).
- The user program never sees the real physical addresses.
### Dynamic Loading
- routine不會被load直到被呼叫,這樣可以使記憶體使用效率較好
- 可以節省記憶體空間,而且library會自動call最新版本
### Dynamic Linking and Shared Libraries
- ==Dynamically linked libraries==: System libraries that are linked to user programs when the programs are run.
- ==static linking==: System libraries are treated like any other object module and are combined by the loader into the binary program image.
- ==dynamic linking==: Similar to dynamic loading. Here, though, linking, rather than loading, is postponed until execution time.
- ==stub==: A small piece of code that indicates how to locate the appropriate memory-resident library routine or how to load the library if the routine is not already present.
    - When the stub is executed, it checks to see whether the needed routine is already in memory.
    - If it is not, the program loads the routine into memory. Either way, the stub replaces itself with the address of the routine and executes the routine.
    - Thus, the next time that particular code segment is reached, the library routine is executed directly, incurring no cost for dynamic linking.
    - Under this scheme, all processes that use a language library execute only one copy of the library code.
### Overlays
![](https://i.imgur.com/Fsix6r3.png)
- 建立Symbol table, common routines, overlay driver之後分別load每個區段的程式
- 把程式切割成幾個區塊,到下一個stage的時候再load到相同位址,節省記憶體,但是要每次都做I/O，所以change at the cost of run-time I/O.
- it is not easy to program a overlay since it is hard to 切割程式。 
### Swapping
- A process, however, can be swapped temporarily out of memory to a backing store and then brought back into memory for continued execution.
- Swapping makes it possible for the total physical address space of all processes to exceed the real physical memory of the system, thus increasing the degree of multiprogramming in a system.
- The system maintains a ready queue consisting of all processes whose memory images are on the backing store or in memory and are ready to run.
- Whenever the CPU scheduler decides to execute a process, it calls the dispatcher.
- The dispatcher checks to see whether the next process in the queue is in memory.
- If it is not, and if there is no free memory region, the dispatcher swaps out a process currently in memory and swaps in the desired process.

![](https://i.imgur.com/zaHBZPA.png)
- Process P1讓出主記憶體而被暫時swap到磁碟機,load P2進入主記憶體
- 只有binding是在rum time的程式才可以放到剛剛P1的位置
- context switch會花很多時間,但是現在通常不會把整個程式丟進丟出
- 如果是正在做I/O的process被swap出去但是DMA卻不知道可能會有覆蓋的問題,因此通常I/O都在OS的memory做
:::info
更詳細解釋這個問題
A process may be waiting for an I/O operation when we want to swap that process to free up memory. However, if the I/O is asynchronously accessing the user memory for I/O buffers, then the process cannot be swapped. Assume that the I/O operation is queued because the device is busy. If we were to swap out process $P_1$ and swap in process $P_2$, the I/O operation might then attempt to use memory that now belongs to process $P_2$.

解決辦法：
1. Never swap a process with pending I/O, or execute I/O operations only into operating-system buffers.
2. Transfers between operating-system buffers and process memory then occur only when the process is swapped in. Note that this double buffering itself adds overhead. We now need to copy the data again, from kernel memory to user memory, before the user process can access it.
:::
- Modified versions of swapping, however, are found on many systems, including UNIX, Linux, and Windows. In one common variation, swapping is normally disabled but will start if the amount of free memory (unused memory available for the operating system or processes to use) falls below a threshold amount.
- Swapping is halted when the amount of free memory increases.
- Another variation involves swapping portions of processes—rather than entire processes—to decrease swap time.
- 把磁碟機分成file system和swapping,把搬進搬出的程式放在swapping裡面比較近的地方(放在相近的的cylinder，減少seeking time )可以減少delay

![](https://i.imgur.com/EryCFd9.jpg)

### Contiguous Allocation
![](https://i.imgur.com/Nhtr8dY.png)

- Single user
  - 檢查CPU給的logical address是否超過limit,合法的話通過MMU map到memory
  - 比較浪費CPU和resource,因為一次只有一支程式在跑，沒有multiprogramming

![](https://i.imgur.com/1UO0zfO.png)
- Multiple Users(fixed partitions)
  - 把memory切成好幾塊,看哪一塊可以放入這支程式就放入,不然就要等,CPU一直在這些process中switch 
  - 有可能有process沒有用完整個區域,會有fragmentation
  - 檢查方式和single user差不多
![](https://i.imgur.com/2q8xnaK.png)
- Multiple Users(dynamic partitions) 
  - 每個process自己切需要的大小,用資料結構紀錄used和free
  - free要照位置排還是照大小排?
  - 照位置排比較容易merge,照大小排比較容易找到最適合大小的，只是每次找位置放就需要O(n)
  - 沒有internal fragment
### Fragmentation – Dynamic Partitions
:::info
- ==Internal fragmentation== is the wasted space within each allocated block because of rounding up from the actual requested allocation to the allocation granularity.
- ==External fragmentation== is the various free spaced holes that are generated in either your memory or disk space.
:::
- ==First fit==. Allocate the first hole that is big enough. Searching can start either at the beginning of the set of holes or at the location where the previous first-fit search ended. We can stop searching as soon as we find a free hole that is large enough.
- ==Best fit==. Allocate the smallest hole that is big enough. We must search the entire list, unless the list is ordered by size. This strategy produces the smallest leftover hole.
- ==Worst fit==. Allocate the largest hole. Again, we must search the entire list, unless it is sorted by size. This strategy produces the largest leftover hole, which may be more useful than the smaller leftover hole from a best-fit approach.
- Simulations have shown that both first fit and best fit are better than worst fit in terms of decreasing time and storage utilization.

- First-fit會產生約$\frac{1}{3}$的memory unusable
- 利用compaction挪動程式來產生更大的連續memory(只有binding是execution time才可以移動)
  - 缺點:複雜度很高,有n!種排列組合
- 解決compaction的問題:利用paged memory,讓空間不用連續
### Paging
![](https://i.imgur.com/99expUo.png)
- user看起來是連續的空間,但是放到physical裡面不一定是連續的
- logical memory切成一塊一塊稱為page,physical memory被切成一塊一塊稱為frame
- CPU送出logical address包含兩部分(p, d),p利用page table查表找到是第f個frame,因此對應到physical address第f個frame的第d個位置
- 不必然logical address和physical address哪一個比較大
- 解決external fragmentation的問題,但是還是有internal fragmentation的問題（平均每個process浪費半個page） 
- paging會增加context-switch的時間
- page table可以存放在register或memory中
- ||register|memory|
  |-|-|-|
  |優點|直接查register|context-switch只要讓PTBR指向新的page table|
  |缺點|context-switch的時候要flush掉再換別人|兩次access(access memory和page table)|
- 改善Page Tables on Memory的缺點
  - Translation Look-aside buffers(TLB) [TLB動畫影片](https://www.youtube.com/watch?v=95QpHJX55bM)
  - 記住最近查到的號碼寫入TLB中
  - 一個array,每個位置都有一個比較器,可以在constant time查找
  - 如果TLB hit可以節省大約90%的時間
  - TLB可以連process id一起記住就不用context-switch就flush TLB
- Effective Memory Access Time 
    - Hit-Ratio * (TLB lookup + a mapped memory access) + (1 – Hit-Ratio) * (TLB lookup + a page table lookup + a mapped memory access)
    - Hit Ratio = the percentage of times that a page number is found in the TLB 
- Protection
  - page table可以記錄很多內容,例如valid/invalid,檔案是否被修改過,read/write/execute
  - 沒有用到的page table可以設定invalid bit,也可以利用Page-Table Length Register(PTLR)來設定page table大小

### Multi-level Paging
- 為什麼要Multi-level Paging？
    - large logical address space
    - page table 可能變得非常大
    - don't want to allocate the page table contiguously in main memory
#### two-level paging algorithm
![](https://i.imgur.com/CMiPhDb.png)
- p1 is an index into the outer page table and p2 is the displacement within the page of the inner page table
![](https://i.imgur.com/feGFsqp.png)
- Because address translation works from the outer page table inward, this scheme is also known as a forward-mapped page table.
#### N-level paging algorithm
- n+1個memory access
- Effective Memory Access Time 沒有差很多
### Hash Page Table
![](https://i.imgur.com/paP1qIU.png)

### Inverted Page Table
- 意義也是爲了減少page table.在下一章較明顯。
- 每個process 都共用同一個page table. 因此要記錄 process id,
- virtual address: <process id, Page number, Offset>
- 請幫忙截圖
- ##### 我是圖片 ####

- 有趣吊軌的地方，回傳是用 frame number 來送，所以是用linear search. 
- 在這一張裏面，在page table 一定要找到，若找不到就是系統的問題。但是在virtual memory 的話可以不用找到。
- 有很多machine 都用 inverted page table.
- Advantage
    - 所有程式都用同一個page table, 所以減少很多量的 memory 來存每個需要的page table.
- Disadvantage
    - 是用 frame 號碼回傳，所以是要linear search (exhaustive search). 所以要跑很久。
    - 結合hash table 一起來用。lookup time: 1HASH + 1IPT
    - 所以流程是，先找TLB, 找不到就找 Hash table, 再找不到才去找inverted page table.
    - difficult to implement with shared page/memory. 可能需要串成link list 或是比較大的entry.
- inverted page table 比較make sense 的地方是程式在執行的時候不需要全部進主記憶體，所以這時候就想要把它的page table 丟出去。（在virtual memory 的時候，但在這一章所有主程式都要在主記憶體裏。）

### Segmentation
 
- 和paging 一樣的想法爲了解決 external fragmentation 的問題。
- 和paging 一樣把程式切割，但是不一樣的是paging 是一樣大小的切，而segment 則是用邏輯單位把程式切割，例如說mainroutine, subroutine, stack, symbol table, sqrt 等等。
- 請截圖。
- #####我是圖片######
- 爲了簡潔所以把名字改成號碼<segment name, offset> -> <segment-number, offset>
- 這裏也要截一張，2：23：02
- #####我是圖片######
- 基本上和paging 一樣，不一樣的地方只是不一樣size 所以要比較limit size.
- ||register|memory|
  |-|-|-|
  |優點|直接查register|context-switch只要讓PTBR指向segment table|
  |缺點|size 是有限的不能有太多register 因爲太貴|當換一個新的 segment table 時 TLB must be flushed|
- Advantage:
    - 因爲是分邏輯切割的，所以同樣一段都算"homogeneous"，因此比較好維護。
    - 和page table 一樣也是可以share code. sharing 發生在 segment level.
- Potential Problems.
    - 因爲是不同大小，所以External Fragmentation 可能還是會存在
    - 因爲goto等的事情，可以goto 到另一個segment, 所以要放在對應的segment 才不會有問題。
- external fragmentation 也會發生但是比paging之前的來説比較小問題，best-fit ,first-fit, worst-fit 等等還是可以用。
- internal fragmentation 還是有可能會發生比(paging 小)，只是看你想不想去維護罷了。

### Segmentation with paging

- 原理，segmentation 較明顯有 external fragmentation的問題，paging 則較明顯有 internal fragmentation 的問題，所以把兩個結合。把大塊的segment 切成page. 變成 "page" Segments

#### Oracle SPARC Solaris

- solaris over a 64-bit SPARC CPU computer
- 用 hash table for kernel, 只用一個給全部processes
- 也有 TLB， TLB miss 就去看 TSB(Translation Storage buffer), 再沒有才去看 hash table 查完後再update TLB 和 TSB.

#### IA-32 Segmentation

- 混合 paging 和 segmentation.
- 8K private segments + 8k Public Segments
- 請截圖 2: 32 : 47
- ####我是圖片####
- 'g' 分兩個addressing space,
    - Local Descriptor Table (LDT) user addressing space
    - Global Descriptor Table (GDT) kernel addressing space
- selector= s(13),+ g(1,user space, kernel space)+ p(2 user mode or kernel mode) s =13bit, 2^13 就是剛好 8k. 
- segment offset 32 bit 代表segment size 可高達 4GB.
- 接著把segment paging 化，分成兩層的paging
- d=12bit = 4kb page size.
- 分大page 和小page
    - 大page p2+d 共 22 bit = 4MB
    - 小page d 共 4kb
- 請截圖 2: 36: 11
- ####我是圖片 #####

- IA-32 Paging
    - page address Extension, 3 levels
    - 前面兩個bit 當作最外層的page

#### IA-32 and Linux

- 因爲 linux 是 OS, 爲了support 各個不同machines, 所以主要用 GD2-level TLB  micro etlb（global descriptor table）就好
- 只用下面六個各一個獨立的segment.kernel code, kernel data, user code, user data, task state segment, default LDT.
- Protection: user and kernel modes
- Paging 方面 IA-32 用兩層的paging linux 32 用三層，所以linux 中間那一層不用，是直接把3層software 套到2層的machine 的做法。


#### x86-64 

- 64bit machine，大概只用了 48 個bit
- 用4層paging(一層9bit) 前面 16 個 bit 沒有用。
- page sizes= 4kb, 2mb, 1GB
- 需要的時候可以切換大page 和小page

#### 32-bit ARM

- 2-level paging 4kb or 16kb pages
- 1-level paging 1mb or 16mb sections
- 2-level TLB: micro tlbs,main tlb 

# CH9 Virtual-Memory Management

### Virtual memory.
- virutal memory- a technique that allows the execution of a process that may not be completely in memory.
- 因爲不用每個程式都要放在memory
- advantage:
    - 不用考慮physical memory 的大小。
    - level of multiprogramming increases
    - each user program may run faster because of less i/o for loading or swapping.

- implementation:
    - demand paging
    - demand segmentation

### Demand Paging - Lazy swapping
- swapping 一次搬動整個程式出去或進來
- lzy swapper only swaps in a page when it is needed.
- page fault 的發生是當你要用一個程式的一個部分但是發現它不再記憶體裏面，英文：occurs when a process references a non-memory-resident page.
- 怎麽知道page fault.
- 請截圖 ---4：22---
- ###我是圖片###
- 在page table 裏有valid 和 invalid bit.
    -  valid : 在主記憶體，
    -  invalid: 不在，記錄在哪裏。
- page fault procedure: 請截圖8：00
    - 1. reference 一段程式
    - 2. tlb miss+ page table invalid, = trap
    - 3. 去physical 裏面找，
    - 4. 找到了拿回來，注意這段時間會花很久所以cpu要 reschuler
    - 5. reset page table.
    - 6. 重新算過程式

- pure demand paging: 缺哪個時才搬哪個。
- Pre-paging: 順便般其他東西進來。用locality of reference, 若猜對效果很好，猜錯效果爛。

#### Hardware support for demand paging
- new bits in page table, to indicate that a page is now in memory or not.
- swap space in the backing store


#### Crucial issues
- cost in restarting an insturction.
- 會page fault 時段：
    - decode
    - fetch operands
    - execute
    - save
- example 1:例如處理 add (a,b,c) 都在不同page, 乾脆全部都搬進來會比較好。
- example 2: mvc x,y ,256 可能有重叠等問題，所以建議都要全部先touch 才能確保不會做到一半的時候page fault 導致要重做而嚷道data self-modifying.

#### performance of Demand Paging
- Effective Access Time:
    - ma: memery acces time for paging , =100ns
    - p: probability of page fault
    - pft: page fault time 
    - total : (1-p)*ma+p*pft
    - pft is super long 8ms = 8000000ns
    - restart & service for page-fault 10^3~10^5 ns
    - so total is 100ns+7999900ns*p
- when p= 1/1000, Effect Access Time = 8000ns
- slow down by 80 times
- so p < 1/799,900  80萬分之一，才能達到 10% slow-down.
- Way to reduce page fault time 
    - preload processes into the swap space before they start up. (sticky bits)
    - Demand paging from file system. 邊做邊load. do page replacement to the swap space. BSD UNIX.

#### Copy-on-write. 寫入時複製。
- 資料有動到才會改。
- It’s a technique to reduce overhead on fork(). 
- The forked process initially shares some memory pages. 
- The kernel allocates new pages only when write attempts are made on these pages.
- 有用到才複製所以很快。

### Page Replacement
- 爲了降低page fault, ways: 把主記憶體加大，大到全部process 都塞得下。
- 爲了不把程式殺掉，所以要做page replacement.
- A page-fault service.
    - find the desired page on the disk
    - find a free frame
        - select a victim and write the victim out when there is no free frame.
    - read the desired page into the selected frame.
    - update the page and frame tables and restart the user process.
- two page transfer per page fault if no frame is available.
- 把不dirty 的page 丟出去
    - dirty = modified
    - 因爲如果改過的話還要save 了才丟，浪費時間。
- 用 modify bit 記錄是不是dirty, 就可以reduce I/O time by one-half. 
- 其實這個做法可以很快，因爲執行檔exe，code segment 這些都不會modified.

#### page replacement algorithms

- calculate the number of page faults on strings of memory references.
- sources of reference string.
- goal reduce the number of data
- only the page numbers if the page size is fixed
    - reduce memory references into page references.
- demand page offset 沒用，只關係到page number.所以可以變成page references.
- 連續的page reference 可以只砍成一個。
- 實作，請截圖43：06

#### Fifo algorithm
- each page is given a time stamp when it is brought into memory.
- select the oldest page for replacement.
- 檢討，不是把最老的丟出去，而是把等下要用的留下來。
- belady's anomaly: for some algorithm, the page fault rate may increases as the number of allocated frames increases.
- 如圖片，48:15 請截圖。special case.

#### Optimal Algorithm （OPT）
- 把最近想要的留下來
- replace the page that will not be used for the longest period of time. <--> future prediction.
- 不會有belady's anomaly

#### Least-Recently-Used Algorithm(LRU)

- 把過去像鏡子一樣做反向然後來預測未來。
- 剛剛會碰的預測未來會中。
- LRU is like OPT which "looks backward in time"
- 不能用軟體來做 LRU queue, 會太慢，所以要用硬體來做。
- implementations: counters 把時間記下來。
    - update the "time-of-use"
    - overflow prevention: 只要知道兩個相差不可能超過某某值就好了。
- implementations: stack 請截圖1：01：25
    - maintain LRU queue 就要改 6個memory.
    - 就很慢所以要。

### LRU approximation algorithm

- 因爲stack 和 queue 很難hardware support.所以要想另外一個方法。

#### Additional-Reference-Bits algorithm
- keep a history of reference bits.
- 固定時間history array bits 往右shift.
- 把這些8bit轉成 unsigned integer.

#### second-chance (clock) algorithm
- 把page 串起來變成一個cycle.
- 是一的話就不抓是0就會被當victim. 
- 若全部都是 1 的話就會回到原點，就抓原點。
- 經得起search 因爲 search 是 10ns左右。

#### Enhanced Secodn-chance algorithm
- set priority. (00,01,10,11)
    - 0,0 最近沒用且not dirty
    - 0,1 最近沒用但dirty
    - 1,0 最近有用但 dirty
    - 1,1 最近有用且dirty
- 看到 1,0 就變成 0,0 看到 1,1 變成 0,1 如果是 0，0 就是找到victim 了。
- macintosh virtual memory management 就是用這個方法。

#### Counting-Based Algorithm
- Least Frequently used Algorithm (LFU)
- Aging. 
- Most Frequently used Algorithm (MFU) 
- they do not approximate OPT very well

### Page Buffering
- basic idea 幫page fault service.
- variation 1
    - 創造一個改過pages list, 
    - 當你的paging device 是空閑的時候就把modified page 寫出去然後reset他的 ‘modify bit’
- variation 2
    - 記錄哪一個page 是在pool 裏面。
    - 若page fault 的話就去那邊找，可以有效減少disk io 的時間。
    - VAM/ VMS with the file 

### Frame Allocation- single user
- 給你足夠的frame 讓你不要常常page fault.
- 當process 想要frame 就從frame list 給他。用到free frame 不夠的時候就做page replacement.
- Variations
    - os 省一點給你frame
    - 做page buffering

### Frame Allocation- Multiple Users
- multiple user 可以劫富濟貧，但是這樣可能會導致你的系統變差。
- Fixed allocation 固定一個方式分配
    - Equal allocation ： 每個人一樣多，m frames, n processes, m/n frames per process.
    - proportional allocation: 根據size 或者是 priority 來給 frmae 的數量。
- Dynamic allocation 
    - alloced frames 根據 multiprogramming level.
    - 根據 others
- 程式給太少frame 會跑不動會餓死。
- the minimum number of frames:
    - ADD A,B,C 'add' 一個,abc 各一個。所以4frames needed
    - ADD (A),(B),(C) = 7 因爲 (A) 是 indirect pointer 要指到A 和別的page 所以是兩個。 所以至少要7 frame.
    - 若 (A)指出去，但是指到 另一個(A') 然後以此類推，就需要很多frace, virtual memory 會炸。 
    - MVC x,y,256 -> 2+2+2=6
- Global allocation
    - 程式可以從別人那邊搶到frame. high-priority processes can increase its frame allocation 通過搶low-priority 的processes.
- local allocation
    - 程式只能從自己分配到的frame 那邊拿要多少frame.
    - 可避免一個high-priority 的程式瘋狂來搶我們的frame.
- Remarks
    - global 就比較像dynamic allocation,而且generally 來説有better system throughput.
    - 但是可能有一個程式壞掉了去專門搶別人的frame 就會導致別人壞掉。
    - 然問題還是在于到底要給程式幾個frame.

### Non-uniform Memory Access (NUMA)
- 有些memory section access might be faster than another. 
- 是因爲一些外層和内層的關係。
- 一般來説access global 會比access local 慢，這主要是架構的問題。
- 或者是多層buffer, 每一層buffer access 速度都不一樣。
    
### Thrashing 
- is a high paging activity.
- 請截圖1：36：02 
- 就是一開始cpu 低的時候加入新的process但是，一旦process 高的時候記憶體不夠所以就開始搶Frame。
- 然後變成大家都page fault,去做io.
- 所以cpu utilization 又降低，導致更多的process又進來，惡性循環。
- Solutions:
    - decrease multiprogramming level.
    - use local page-replacement. 但page fault service of other proccesses 也變慢因爲磁碟機只有一個。
    - 主要問題還是要給程式足夠的frame.

### locality model
- 就是要給程式多少frame 才夠。
- 在程式編譯時就先檢查要給他多少frame.但是這個還不算完整，因爲有時候很難計算。

### Working-set model
- 設置一個working-set window 來讀之前的program locality.
- 但是working-set window 的大小很難確定。
- way1
    - D= 這個working-set 裏面用了多少frame.
    - M 是total number of available frames
    - D>M suspend some processes
    - D<=M safe, 視page fault 的次數來決定要不要initiate new processes.
- way2 
    - 因爲 maintence of working sets is expensive 
    - 用一個timer 和 reference bit 來記錄。這個做法比較便宜。

### Page-Fault Frequency
- 用page-fualt rate來決定要給你多少frame.
- 低page-fault rate, 就給少一點frame.
- 高page-fault rate 就給多一點frame.
- 請截圖1：46：11
- 但是如果page-fault rate 還是很高的話就要suspended and swapped out 程式。
- 另外一個方法就是直接看page-fault rate 給你frame.不用去猜locality

### Memory-Mapped Files
- 就是把一些記憶體之間溝通做好，例如piped 變成named piped
- 把read(),write() 都變成直接在你的主記憶體裏面完成。
- 可以讓大家shared 和 dump.

### Memory-Mapped I/o
- frequently used devices.
- programmed i/o - polling io or interrupt driven handling

### Kernel Memory allocation
- 和user-mode memory allocation 不一樣。
- kernel 會request 不一樣大小的memory. often less than a page in size. malloc, new 等指令。
- interact directly with physical memory, 在硬體裏accessed memory 都要是連續的。

- The buddy System
    - 以2的次方來切，切到剛好比他大。
    - 假設一塊記憶體是 256kb, 程式要求要 31kb, 所以不斷除2，所以最後給他32kb.
    -  advantage: quick coalescing algorithm. merge 時候很好merge.
    -  disadvantage: 因爲17kb 你也給他32kb,所以internal fragmentation 嚴重。

- Slab allocation
    - slab : one or more physically 連續的pages
    - cache : 多個slabs
    - kernel objects 跟caches 拿memory, cache 裏面放不同的slab. 
    - slab states, full, empty, partial.

- slab allocator
    - 要一個free 的object 速度很快，因爲就是slab 拿一個走。
    - 若裏面的slab 都是full slab 就那一個新的走。
    - no space wasted in fragmentation,
    - memory requests are quick.
    - solaris 2.4 kernel , linux version 2.2+ 
    
### Other Consideration
#### Pre-paging
- 先搬很多進來
- 要衡量到底划不划算，因爲不是所有prepaging 的都會用到。所以要做很好的分析。
- 把pre-paging 的cost 和 cost of page fault service 變得平衡， 但是implement較難。
#### page size
- 在better locality internal fragmentation 和 better i/o 中找平衡
- page size 越大，i/o 越快，page table 越小，但internal fragmentation 越多,locality 不好。page size 小反之。
- 現在的主流-large page size.
- 因爲moore laws, cpu speed and the memory capacity grow much faster than the disk.
    
#### TLB Reach
- 目標是 working set 都存在tlb.
- reach= TLB-entry-number * page-size
- solution, increate page size 
- have multiple page size 像 ultrasparc II (8kb~4mb),solaris 2 (8kb or 4mb)
- 大page 小page 同時存在。

#### inverted page table 
- 就不需要把全部page table 存在主記憶體，這樣就可以節省空間，等到要的時候再去拿就好了

#### Program structure
- 這個是在描述如果在讀一個二維陣列的時候每個row 都放在不同的page, 若frame小於row size, 那麽我們在讀 for i, j  A[j,i] 的時候就會發生很多次page fault.
- 所以如果是一個row wise 的系統的話讀的時候就盡量避免這個問題。
- 組要還是frame 要給多。

#### program structures
- 以data structures 來看
    - Locality 分成stack, hash tbale 等。
    - search speed 會因此不同
- 以programmming language 來看。
    - Pascal 是data structures 的language, 所以locality 比較好。
    - Lisp 是pointer ,所以可能會指到別的page ,page fault 量增加。
- compiler & loader
    - 較聰明的compiler 會把 code 和data 分好避免code 或data 跨page 的行爲
    - 也會設一個比較好的routine.

#### I/O interlock
    
    
    
# CH 10 File Systems
### File-System Mounting
- a file system must be mounted before it can be available to processes on the system
- ==mount point==: the location within the file structure where the file system is to be attached

### File Sharing
#### Multiple Users
#### Remote File Systems
- ftp: manually transferring files between machines via programs
    - ftp is used for both anonymous and authenticated access
    - ==Anonymous access== allows a user to transfer files without having an account on the remote system.
- distributed file system (DFS): remote directories are visible from a local machine
- World Wide Web: A browser is needed to gain access to the remote files, and separate operations (essentially a wrapper for ftp) are used to transfer files.
    - 幾乎都用anonymous file exchange
- The Client–Server Model:
    - server: the machine containing the files
    - client: the machine seeking access to the files
- Distributed Information/naming Systems
    - domain name system (DNS): provides host-name-to-network-address translations for the entire Inter- net
    - Sun Microsystems introduced yellow pages ( network information service, or NIS): centralizes storage of user names, host names, printer info
        - unsecure authentication methods, including sending user passwords unencrypted (in clear text) and identifying hosts by IP address
    - common Internet file system (CIFS)
        - used in conjunction with user authentication (user name and password) to create a network login that the server uses to decide whether to allow or deny access to a requested file system
    - lightweight directory-access protocol (LDAP)
        - secure single sign-on: enter their authentication information once for access to all computers
- Failure Modes
    - disk failure, corruption of the directory structure or other disk-management information (collectively called metadata), disk-controller failure, cable failure, host-adapter failure, user or system-administrator failure, network failure
    - state information be maintained on both the client and the server => recover from a failure
    - the server crashes but must recognize that it has remotely mounted exported file systems and opened files, NFS takes a simple approach, implementing a stateless DFS
#### Consistency Semantics
- specify how multiple users of a system are to access a shared file simultaneously
- In particular, they specify when modifications of data by one user will be observable by other users.
- need light-weighted implementation of process synchronization
- ==file session==: The series of accesses between the open() and close() operations.
- UNIX Semantics
    - Writes to an open file by a user are visible immediately to other users who have this file open.
    - One mode of sharing allows users to share the pointer of current location into the file. Thus, the advancing of the pointer by one user affects all sharing users.
- Session Semantics -- Andrew file system
    - Writes to an open file by a user are not visible immediately to other users that have the same file open.
    - Once a file is closed, the changes made to it are visible only in sessions starting later. Already open instances of the file do not reflect these changes.
- Immutable-Shared-Files Semantics
    - Once a file is declared as shared by its creator, it cannot be modified.
        - name may not be reused
        - contents may not be altered
### Protection
- When information is stored in a computer system, we want to keep it safe from physical damage (the issue of reliability) and improper access (the issue of protection).
- Reliability: duplicate copies of files
- Protection: 怎麼樣access
#### Types of Access
- Read: Read from the file.
- Write: Write or rewrite the file.
- Execute: Load the file into memory and execute it.
- Append: Write new information at the end of the file.
- Delete: Delete the file and free its space for possible reuse.
- List: List the name and attributes of the file.

- higher-level functions: enaming, copying, and editing the file
    - protecting 通常是在 low level
#### Access Control
- ==access-control list (ACL)==: specifying user names and the types of access allowed for each user
    - When a user requests access to a particular file, the operating system checks the access list associated with that file. If that user is listed for the requested access, the access is allowed. Otherwise, a protection violation occurs, and the user job is denied access to the file.
    - 缺點：
        - list length
        - tedious construction: Constructing such a list may be a tedious and unrewarding task, especially if we do not know in advance the list of users in the system.
        - variable directory entry: The directory entry, previously of fixed size, now must be of variable size, resulting in more complicated space management.
    - condense the length of the access-control list: 把user分成三類
        - Owner：The user who created the file is the owner.
        - Group：A set of users who are sharing the file and need similar access is a group, or work group.
        - Universe：All other users in the system constitute the universe.
#### Other Protection Approaches
- associate a password with each file
- associate a password with a subdirectory
- 缺點：密碼會很多

---
# CH 12 I/O Systems
- ==device drivers==: present a uniform device- access interface to the I/O subsystem
### I/O Hardware
- ==port==: device communicates with the machine via a connection point
- ==bus==: A bus is a set of wires and a rigidly defined protocol that specifies a set of messages that can be sent on the wires.
- ==daisy chain==: device A has a cable that plugs into device B, and device B has a cable that plugs into device C, and device C plugs into a port on the computer
- ==controller==: a collection of electronics that can operate a port, a bus, or a device
    - controller has one or more registers for data and control signals
    - processor communicates with the controller by reading and writing bit patterns in these registers
- ==memory-mapped I/O==
    - the device-control registers are mapped into the address space of the processor
    - The CPU executes I/O requests using the standard data-transfer instructions to read and write the device-control registers at their mapped locations in physical memory
    - writing millions of bytes to the graphics memory is faster than issuing millions of I/O instructions
- I/O port typically consists of four registers:
    - data-in register: read by the host to get input
    - data-out register: written by the host to send output
    - status register: contains bits indicating states that can be read by the host
    - control register: written by the host to start a command or to change the mode of a device
#### Polling
- reading the status register over and over until the busy bit becomes clear
#### Direct Memory Access
- Many computers avoid burdening the main CPU with programmed I/O (PIO) by offloading some of this work to a special-purpose processor called a direct-memory-access (DMA) controller.
### Application I/O Interface
#### Block and Character Devices
- ==block-device interface==: captures all the aspects necessary for accessing disk drives and other block-oriented devices
    - read() and write(), seek()
    - ==raw I/O==: access a block device as a simple linear array of blocks
    - ==memory-mapped file access==: A file is mapped to the virtual memory, actual data transfers are done when needed to satisfy access to the memory image.
- ==character-stream interface==: libraries can be built that offer line-at-a-time access, with buffering and editing services
    - get(), put()
    - ex: keyboards, mice
#### Network Devices
- network socket interface
    - the system calls in the socket interface enable an application to:
        - create a socket
        - connect a local socket to a remote address
        - listen for any remote application to plug into the local socket
        - send and receive packets over the connection
        - select(): manages a set of sockets, returns information about which sockets have a packet waiting to be received and which sockets have room to accept a packet to be sent
- Clocks and Timers
    - hardware clocks and timers that provide three basic functions:
        - Give the current time.
        - Give the elapsed time.
        - Set a timer to trigger operation $X$ at time $T$.
    - ==programmable interval timer==: The hardware to measure elapsed time and to trigger operations.
#### Non-blocking and Asynchronous I/O
- ==Blocking system calls==: The suspension of an application execution due to a varying or unpredictable amount of time in performing physical actions to some I/O devices or better understanding in application programming.
- ==non-blocking system calls==:
    - A non-blocking call does not halt the execution of the application for an extended time. Instead, it returns quickly, with a return value that indicates how many bytes were transferred.
    - An asynchronous call returns immediately, without waiting for the I/O to complete. The application continues to execute its code. The completion of the I/O at some future time is communicated to the application, either through the setting of some variable in the address space of the application or through the triggering of a signal or software interrupt or a call-back routine that is executed outside the linear control flow of the application.
    - ==non-blocking read()== returns immediately with whatever data are available—the full number of bytes requested, fewer, or none at all
    - ==asynchronous read()== call requests a transfer that will be performed in its entirety but will complete at some future time. 完整地完成，可是可能在未來才會完成

***課本似乎把non-blocking當作synchronous***
![](https://i.imgur.com/VhcZFM3.png)
# Ch 13 Protection, Security, Distributed Systems
- ==Distributed Systems==: A collection of loosely coupled processors interconnected by a communication  network.
#### Distributed Synchronization
- challenges: no common clocks and memory
- locking: two-phase locking
- timestamp

