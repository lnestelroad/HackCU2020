#!/bin/bash

docker network create postgres_backend 
docker run --name postgres \
  -e POSTGRES_PASSWORD=liam \
  -e POSTGRES_USER=Liam \
  -e POSTGRES_DB=HackCU \
  -d --restart always \
  -v postgres:/var/lib/postgresql/data \
  --net postgres_backend \
  postgres:9.6

IP=$(docker inspect -f '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' postgres)

python3 db_interface.py -b 1 -i $IP