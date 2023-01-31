# FROM ubuntu:18.04
FROM registry.gitlab.com/polytechnique-montr-al/inf3995/20213/equipe-105/inf3995:latest
LABEL argos-example.version="0.1"

# copies the source code into the app 
COPY ./examples/ ./root/examples/

EXPOSE 6001

# Build your code (here examples)
RUN cd /root/examples &&\
    mkdir build && cd build &&\
    cmake -DCMAKE_BUILD_TYPE=Debug .. &&\
    make -j $(nproc)

## Remove logging errors 
ENV QT_LOGGING_RULES='*=false'
CMD export QT_LOGGING_RULES