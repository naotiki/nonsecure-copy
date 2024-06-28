FROM debian:11.9
LABEL authors="naotiki"
RUN apt update && apt install -y build-essential
RUN apt install -y cmake gdb
RUN apt install -y git
ENTRYPOINT ["top", "-b"]