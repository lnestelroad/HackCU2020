#!/bin/bash

# docker network create postgres_backend 
# docker run --name postgres \
#   -e POSTGRES_PASSWORD=liam \
#   -e POSTGRES_USER=Liam \
#   -e POSTGRES_DB=HackCU \
#   -d --restart always \
#   -v postgres:/var/lib/postgresql/data \
#   --net postgres_backend \
#   postgres:9.6
docker-compose up -d

IP=$(docker inspect -f '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' database_db_1)

python3 fake_data.py -b 1 -i $IP