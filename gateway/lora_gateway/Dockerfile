FROM ubuntu:22.04

RUN apt-get update && \
    apt-get install -y cmake \
    g++ \
    gcc \
    libfmt-dev \
    libspdlog-dev \
    git \
    libssl-dev \
    doxygen \
    make


#todo boost
#todo pahoo

RUN mkdir /gateway/
COPY . /gateway/

RUN mkdir -p /tmp/libs

# Install Boost
RUN cd /tmp/libs && \
    git clone https://github.com/boostorg/boost.git && \
    cd boost && \
    git submodule update --init --recursive && \
    git checkout boost-1.83.0 && \
    ./bootstrap.sh && \
    ./b2 install

# Install Paho
RUN cd /tmp/libs && \
    git clone https://github.com/eclipse/paho.mqtt.c.git && \
    cd paho.mqtt.c && \
    git checkout v1.3.13 && \
    cmake -Bbuild -H. -DPAHO_ENABLE_TESTING=OFF -DPAHO_BUILD_STATIC=ON \
        -DPAHO_WITH_SSL=ON -DPAHO_HIGH_PERFORMANCE=ON && \
    cmake --build build/ --target install

RUN cd /tmp/libs && \
    git clone https://github.com/eclipse/paho.mqtt.cpp.git && \
    cd paho.mqtt.cpp && \
    cmake -Bbuild -H. -DPAHO_BUILD_STATIC=ON \
        -DPAHO_BUILD_DOCUMENTATION=ON -DPAHO_BUILD_SAMPLES=ON && \
    cmake --build build/ --target install

# Install spdlog
RUN cd /tmp/libs && \
    git clone https://github.com/gabime/spdlog.git && \
    cd spdlog && \
    git checkout v1.13.0 && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make install

RUN mkdir -p /gateway/docker_build && \
    cd /gateway/docker_build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && \
    make -j 8


WORKDIR /gateway

CMD ["/gateway/docker_build/LoraGateway --config=/gateway/resources/configuration/for_docker.json"]

