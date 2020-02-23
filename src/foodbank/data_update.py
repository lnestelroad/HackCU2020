from Database import Database

def db_parse_to_json(ip, build):

    DM = DistanceMatrix()

    interface = Database()

    interface.connectToDatabase(ip)

    r_addresses = interface.getResterauntAddresses()
    d_addresses = interface.getDonationCenterAddresses()

    for i in 
    
    
