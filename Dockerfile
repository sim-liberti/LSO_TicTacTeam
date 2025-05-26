FROM gcc:latest AS builder

WORKDIR /app
COPY server /app/server
RUN mv /app/server/Makefile.docker /app/server/Makefile
WORKDIR /app/server

RUN make

FROM alpine:latest

WORKDIR /app
COPY --from=builder /app/server/build/server.out .

EXPOSE 8080
CMD ["./server.out"]
