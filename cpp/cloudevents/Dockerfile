FROM ubuntu:22.04

RUN apt-get update && apt-get install -y g++ libboost-all-dev

COPY include /usr/include

COPY main.cpp /app/main.cpp

RUN g++ -o /app/main /app/main.cpp -lboost_system -lboost_thread -std=c++17

CMD ["/app/main"]
