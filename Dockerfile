FROM gcc:latest

WORKDIR /app

RUN apt-get update && apt-get install -y valgrind && rm -rf /var/lib/apt/lists/*

COPY *.h *.cpp Makefile ./

RUN make && make clean

CMD [ "campusGuard" ]

