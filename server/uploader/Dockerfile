FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    python3 \
    python3-pip

RUN mkdir /uploader/
COPY . /uploader/

RUN mkdir -p /tmp/libs

RUN cd /uploader && pip install -r requirements.txt

WORKDIR /uploader
