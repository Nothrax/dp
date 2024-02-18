# Deployment
Scripts for docker compose deployment.

# Content
[build-docker.sh](./buil-docker.sh) - script for building docker iamges
[docker-compose.yml](./docker-compose.yml) - docker compose file
[configs](./configs) - configuration files for programs, will be mounted to images
[storage](./storage) - permanent storage for docker images, will be mounted to containers

## Docker images
 - influxdb - timeseries database for data storage
 - mosquitto - MQTT broker
 - gateway - gateway program, generating random data with default settings
 - uploader - uploader program

# Run
```
bash ./build-docker.sh
docker compose --profile=all up
```
