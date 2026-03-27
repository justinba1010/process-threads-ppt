FROM alpine:3.21
RUN apk add --no-cache g++ musl-dev
WORKDIR /app
COPY mem_hang.cpp .
RUN g++ -o mem_hang mem_hang.cpp
CMD ["sh", "-c", "echo 0 > /proc/sys/kernel/randomize_va_space && ./mem_hang"]
