# OS Demos

## Programs

| Binary | Source | Description |
|--------|--------|-------------|
| `threads` | `threads.cpp` | 5 threads print to stdout concurrently — interleaved output demonstrates lack of synchronization |
| `threads_semaphore` | `threads_semaphore.cpp` | 8 threads protected by a named semaphore — serializes stdout access |
| `mem_hang` | `mem_hang.cpp` | `mmap`s a page, touches it, then reads `/proc/self/pagemap` to print the virtual → physical address translation |
| `oom` | `oom.cpp` | Allocates and touches 1 MiB at a time in a loop until the OOM killer terminates it |

---

## Build (native)

```sh
g++ -o threads            threads.cpp            -std=c++20 -pthread
g++ -o threads_semaphore  threads_semaphore.cpp  -std=c++20 -pthread
g++ -o mem_hang           mem_hang.cpp
g++ -O0 -o oom            oom.cpp
```

## Run (native)

```sh
./threads
./threads_semaphore
sudo ./mem_hang          # root required to read /proc/self/pagemap
./oom
```

---

## Docker — build images

```sh
docker build -t mem_hang -f Dockerfile     .
docker build -t oom      -f Dockerfile.oom .
```

## Docker — run with 8 MiB memory limit

### threads

```sh
docker run --rm --memory=8m --memory-swap=8m \
  $(docker build -q -f - . <<'EOF'
FROM alpine:3.21
RUN apk add --no-cache g++ musl-dev
WORKDIR /app
COPY threads.cpp .
RUN g++ -o threads threads.cpp -std=c++20 -pthread
CMD ["./threads"]
EOF
)
```

### threads\_semaphore

```sh
docker run --rm --memory=8m --memory-swap=8m \
  $(docker build -q -f - . <<'EOF'
FROM alpine:3.21
RUN apk add --no-cache g++ musl-dev
WORKDIR /app
COPY threads_semaphore.cpp .
RUN g++ -o threads_semaphore threads_semaphore.cpp -std=c++20 -pthread
CMD ["./threads_semaphore"]
EOF
)
```

### mem\_hang

Requires `--privileged` to disable ASLR and read `/proc/self/pagemap`.

```sh
docker build -t mem_hang -f Dockerfile .
docker run --rm --memory=8m --memory-swap=8m --privileged mem_hang
```

### oom

```sh
docker build -t oom -f Dockerfile.oom .
docker run --rm --memory=8m --memory-swap=8m oom
```

`--memory-swap=8m` matches the memory cap so swap is disabled, letting the OOM killer fire as expected. The container will be killed and exit with code 137 once it exceeds 8 MiB.
