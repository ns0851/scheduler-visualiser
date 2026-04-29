# Scheduler Visualiser
![Go](https://img.shields.io/badge/Go-00ADD8?style=flat&logo=go)
![C](https://img.shields.io/badge/C-00599C?style=flat&logo=c)
![React](https://img.shields.io/badge/React-61DAFB?style=flat&logo=react)

This simulates scheduling algorithms and shows how processes run over time using a Gantt chart and Process table. 
The backend logic is written in C, the UI is in React, and Go is used as a bridge for JSON communication.

---

## What it does

* Takes process input (arrival time, burst time, allocated time etc.)
* Runs scheduling algorithms
* Generates execution timeline
* Visualises it as a Gantt chart
* Shows final metrics like CT, TAT, Response

---

## Algorithms implemented

* FCFS
* SJF (Preemptive and Non-Preemptive)
* Round Robin
* MLFQ (with priority queues, allotment, reset, etc.)

MLFQ was the main focus — especially handling:

* preemption
* demotion between queues
* priority reset
* correct timeline generation

---

## Tech

* C → core scheduling logic
* React → UI + visualisation
* Go → JSON bridge between backend and frontend

---

## Why I built this

Most implementations online either:

* skip edge cases
* or don’t show *how* scheduling actually happens

I wanted something where:

* I control the logic fully
* I can see every context switch
* and debug things like MLFQ properly

---

## Current state

Core algorithms are working.

Main effort went into:

* getting MLFQ correct
* generating proper timeline data for visualisation

Known issue:

- MLFQ visualisation is being actively improved - logic is correct, frontend mapping is being refined.

---

## Run locally

```bash
git clone https://github.com/ns0851/scheduler-visualiser.git
cd scheduler-visualiser
```

---

## 🟢 Backend (Go)

### 1. Install Go

https://go.dev/dl/

```bash
go version
```

---

### 2. Setup project

```bash
go mod init 'name'
go mod tidy
```

---

### 3. Install Air (for live reload)

```bash
go install github.com/air-verse/air@latest
```

Add Go bin to PATH -> env variables/system variable>Path
---

### 4. Run backend

```bash
go run main.go
```

Or with live reload:

```bash
air
```

*(Run `air init` once if `.air.toml` is not present)*

---

## 🔵 Backend (C)

```bash
gcc main.c scheduler/basic_scheduler.c coreJob/job.c scheduler/MLFQ.c queueHelpers/helper.c queueHelpers/timeline.c cJSON.c -o scheduler
./scheduler
```

---

## 🟣 Frontend

```bash
npm install
npm run dev
```

---

## Future ideas

* better UI (animations, smoother Gantt)
* improve MLFQ visualisation (reset, queue movement, etc.)
* add more algorithms
* cleaner input system
* side-by-side comparison of algorithms

---

Wish to work on this project even further to make algorithms much easier to learn VIA visualization
