from dotenv import load_dotenv
import googlemaps
from datetime import datetime
import os
import json


def setup():
    load_dotenv(verbose=True)


def test():
    gmaps = googlemaps.Client(key=os.getenv("API_KEY"))

    geocode_result=gmaps.geocode('700 31st st boulder co')

    print(type(geocode_result))
    print("formatted_address" in geocode_result[0])
    print(geocode_result[0]["geometry"]["location"]["lat"])
    
    #  data = json.loads(geocode_result)
    #  print(data)
    
    
    

    now = datetime.now()
    directions_result = gmaps.directions("Sydney Town Hall",
                                     "Parramatta, NSW",
                                     mode="transit",
                                     departure_time=now)


setup()
test()
