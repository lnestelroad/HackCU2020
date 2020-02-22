from dotenv import load_dotenv
import googlemaps
import os
import json
from random import randint

load_dotenv(verbose=True)
gmaps = googlemaps.Client(key=os.getenv("API_KEY"))


class AddressNode:

    def __init__(self, address=None, cache_file=None, location=None, identification=None):

        # Might need to make request if cache file true
        make_request = True

        if cache_file:
            # Check for id
            if identification == None:
                raise Exception("For cached data, please provide an identification")
            
            # Open the json file and possibly write to it
            with open(cache_file) as cache:
                cache = open(cache_file, 'rw')
                data = json.load(cache)
            
                if identification in data:
                    self.geocode = data[identification]
                else:
                    print("Cacheing data")
                    raise Exception("Error, couldn't find identification")

                
    def make_request():
        # Meta information
        geocode = gmaps.goecode(address)
        

        # Invalid request
        if len(self.geocode) == 0:
            raise Exception("Address passed was invalid for desired address node")

        self.geocode = geocode[0]


        if "formatted_address" not in self.geocode[0]:
            raise Exception("Couldn't find formatted address, double check version of google maps api")

        self.address = self.geocode[0]["formatted_address"]

        if "geometry" not in self.geocode[0]:
            raise Exception("Couldn't find geometry in response, double check version of google maps api")
    
        latitude = self.geocode[0]["geometry"]["location"]["lat"]
        longitude = self.geocode[0]["geometry"]["location"]["lng"]

        # Self position for future api calls (avoid goelocation calls)
        self.pos = (latitude, longitude)

        
    
