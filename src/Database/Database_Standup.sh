#!/bin/bash

docker-compose up -d
IP=$(docker inspect -f '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' database_db_1)
python3 fake_data.py -b 1 -i $IP

# To see that the containers IP address is, run docker inspect -f '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' database_db_1