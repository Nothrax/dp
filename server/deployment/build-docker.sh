#!/bin/bash

echo "Building uploader"
docker build -t uploader ../uploader

echo "Building gateway"
docker build -t gateway ../../gateway/lora_gateway


echo "Building flask endpoint"
docker build -t flask_endpoint ../flask_endpoint
