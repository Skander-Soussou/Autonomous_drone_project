FROM python:3

RUN apt-get update && apt-get install

WORKDIR /app

COPY drone.py ./
COPY server.py ./
COPY crazy_flie_connect.py ./
COPY ./tests ./tests
COPY ./bin ./bin
COPY ./bin/requirement.txt ./bin/requirement.txt
COPY ./bin/server.sh ./bin/server.sh
COPY drone_info.py ./
COPY socket1_argos.py ./
COPY socket2_argos.py ./
RUN pip install -r ./bin/requirement.txt
RUN chmod +x ./bin/server.sh
EXPOSE 5000
RUN cd bin &&\
    ./server.sh  