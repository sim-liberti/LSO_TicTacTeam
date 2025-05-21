FROM gcc:latest AS builder

WORKDIR /app
COPY server /app/server
WORKDIR /app/server

RUN make

FROM alpine:latest

WORKDIR /app
COPY --from=builder /app/server/build/server.out .

EXPOSE 8080
CMD ["./server.out"]
