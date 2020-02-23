from Database import Database
from foodbank import DistanceMatrix

def db_parse_to_json(ip):

    DM = DistanceMatrix()

    interface = Database()

    interface.connectToDatabase(ip)

    r_addresses = interface.getResterauntAddresses()
    d_addresses = interface.getDonationCenterAddresses()

    for i in r_addresses:
        print(type(i[0]))
        DM.push(i[0], identification=i[0], d_type='o')

    for i in d_addresses:
        DM.push(i[0], identification=i[0], d_type='d')

    DM.to_json_file("./.cache_money/graph.json")
    
    
