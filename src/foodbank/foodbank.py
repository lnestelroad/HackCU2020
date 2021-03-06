from dotenv import load_dotenv
import googlemaps
import os
import json
import random
import warnings
import copy

# Load the dot env environment variable
load_dotenv(verbose=True)
gmaps = googlemaps.Client(key=os.getenv("API_KEY"))

DEFAULT_CACHE="./.cache_money/addresses.json"
MAX_CACHED=100

SINK=0
SOURCE=1

class Node:
    def __init__(self, name, type_):
        self.name = "NoName"
        self.edges = {}
        self.weight = random.randint(0, 10)
        self.name = name
        self.type = type_

    def append_vertex(self, distance: float, vertex):
        self.edges[vertex] = {"edge_weight" : distance}

    def to_json(self, json: dict):
        if self.name in json:
            raise Exception("Error, name already exists")

        json[self.name] = {"type" : self.type, "vertex_weight" : self.weight, "edges" : self.edges}

class DistanceMatrix:
    
    nodes = {}
    destinations = []
    origins = []
    ready = True

    def __init__(self, destinations=None, origins=None):
        if destinations is not None:
            for i in destinations:
                self.push_destination(i, i)
        if origins is not None:
            for i in origins:
                self.push_origin(i, i)

    ##
    # @param dm A dm response
    def parse_distance_matrix(self, dm):
        origins = dm["origin_addresses"]
        dests = dm["destination_addresses"]
        rows = dm["rows"]
        origins_self = dict({i.address : i.type for i in self.destinations}, **{i.address : i.type for i in self.origins})

        
        
        for i in range(len(origins)):
            node = Node(origins[i], origins_self[origins[i]])
            print(origins_self[origins[i]])
            for j in range(len(rows[i]["elements"])):
                node.append_vertex(rows[i]["elements"][j]["duration"]["value"], dests[j])
            self.nodes[origins[i]] = {"type" : node.type, "vertex_weight" : node.weight, "edges" : node.edges}


    def to_json_file(self, file_name):
        self.query()
        with open(file_name, 'w') as fp:
            json.dump(self.nodes, fp)

    def query(self):
        origin_endpoints = [i.address for i in self.origins]
        destination_endpoints = [i.address for i in self.destinations]
        #
        #  self.json = dict({i.address : {"type" : i.type, "vertex_weight" : random.randint(0, 10), "edges" : {}} for i in self.destinations},
        #                 **{i.address : {"type" : i.type, "vertex_weight" : random.randint(0, 10), "edges" : {}} for i in self.origins})
        #
        #  CHUNKS = 20
        #
        #  total = origin_endpoints + destination_endpoints
        #
        #  subarrays = [total[i * CHUNKS: i*CHUNKS + CHUNKS] for i in range(int(len(total) / CHUNKS) + 1)]
        #  subsubarrays = [i[0:CHUNKS /
        #
        #
        #  for i in subarrays:
        #      join(i[0:CHUNKS / 2], i[CHUNKS / 2:])
        #
        #  for i in
        #
        #
        #
        #
        #
        #  self.json = {}
    #
        #  total = origin_endpoints + destination_endpoints
    #
        #  for i in
        #
        #
        #
        #

        total = origin_endpoints + destination_endpoints
        resp = gmaps.distance_matrix(origins = total, destinations = total)
        self.parse_distance_matrix(resp)
            


    def push(self, address, identification, cache_file=DEFAULT_CACHE, d_type='d'):
        self.ready = False
        if d_type == 'd':
            self.push_destination(address, cache_file, identification)
        elif d_type == 'o':
            self.push_origin(address, cache_file, identification)
        else:
            warnings.warn("Invalid d_type option")

    def remove(self, identification, d_type='d'):
        self.ready = False
        if d_type == 'd':
            self.remove_destination(address, cache_file, identification)
        elif d_type == 'o':
            self.remove_origin(address, cache_file, identification)
        else:
            warnings.warn("Invalid d_type option")

    def push_destination(self, address, cache_file=None, identification=None):
        self.ready = False
        address_node = AddressNode(address = address, type_=SINK, cache_file = cache_file, identification = identification)
        self.destinations.append(address_node)

    def push_origin(self, address, cache_file=None, identification=None):
        self.ready = False
        address_node = AddressNode(address = address, type_ = SOURCE, cache_file = cache_file, identification = identification)
        self.origins.append(address_node)

    def remove_destination(self, identification):
        self.ready = False
        for i in destinations:
            if i.identification == identification:
                del i
                return True
        warnings.warn("Couldn't find element desired to remove")
        return False

    def remove_origin(self, identification):
        self.ready = False
        for i in origins:
            if i.identification == identification:
                del i
                return True
        warnings.warn("Couldn't find element desired to remove")
        return False

    




class AddressNode:
    
    # All the information we store is
    # here
    pos = (0, 0)
    address = ""
    geocode = {}
    identification = ""        

    ##
    # @brief Constructor
    #
    # @param address The address (In arbitrary terms think google worthy)
    # @param cache_file An optional cache file to cache queries
    # @param identification The identification value for the cached data
    def __init__(self, type_, address:str=None, cache_file:str=None, identification:str=None):

       
        self.type = type_

        if cache_file is not None:

            # An id is required for cached data
            if identification == None:
                raise Exception("For cached data, please provide an identification")

            self.identification = identification

            # Open the json file and possibly write to it
            with open(cache_file, 'r+') as cache:
                
                # A python dict with {'key' : 'geocode', 'key' : 'geocode'} format
                data = json.load(cache)

                if identification in data:
                    geocode = data[identification]
                    self.parse_geocode(geocode)
                    print("Using previously cached data")

                else:
                    print("Caching data")
                    self.make_request(address)
    
                    # Delete elements from data until data
                    # is the right size
                    while len(data) >= MAX_CACHED:
                        a = random.choice(list(data.keys()))
                        del data[a]

                    # Store the geocode element
                    data[identification] = self.geocode

                    # Write to the cache file
                    cache.seek(0)
                    json.dump(data, cache)
        else:
            self.identification = address
            self.make_request(address)

                
    ##
    # @brief Make an API request and parse geocode at the same time
    def make_request(self, address):
        print("Making Request")
        geocode = gmaps.geocode(address)

        if len(geocode) == 0:
            raise Exception("Address passed was invalid for desired address node")
        if len(geocode) > 1:
            warnings.warn("Returned array is greater in length than 1, meaning more addressess were identified, try being more specific")

        self.parse_geocode(geocode[0])
        
    ##
    # @brief Parses an incomming geocode into self attributes
    #
    # @param geocode A geocode dict (first element of the list returned on request)
    def parse_geocode(self, geocode):
        # Extract the first element
        self.geocode = geocode

        if "formatted_address" not in self.geocode or "geometry" not in self.geocode:
            raise Exception("Couldn't find all required fields from response, double check version of google maps api")
    
        # Parse actual data
        self.address = self.geocode["formatted_address"]
        latitude = self.geocode["geometry"]["location"]["lat"]
        longitude = self.geocode["geometry"]["location"]["lng"]
        self.pos = (latitude, longitude)


if __name__ == '__main__':
    a = AddressNode(address="700 31st st boulder co 80303", cache_file="./temp", identification="test")
