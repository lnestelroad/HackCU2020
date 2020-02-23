#!/usr/bin/env sh

update_database() {
        #docker-compose up -d
        IP=$(docker inspect -f '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' database_db_1)
        
        python3 test.py -i $IP
        # python3 test.py -i $IP
	./graph
}

create_database() {
        IP=$(docker inspect -f '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' database_db_1)
        python3 ./src/Database/fake_data.py -b 1 -i $IP 
}

update_database
