FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    python3 \
    python3-pip

RUN mkdir /flask_endpoint/
COPY . /flask_endpoint/

RUN mkdir -p /tmp/libs

RUN cd /flask_endpoint && pip install -r requirements.txt

WORKDIR /flask_endpoint
