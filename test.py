from src.foodbank import AddressNode, DistanceMatrix, db_parse_to_json
import argparse


if __name__ == '__main__':

    ap = argparse.ArgumentParser()

    ap.add_argument("-i", "--ip", required=True, type=str, default="172.22.0.2",
    help="Tells the code what the database's IP address is.")

    args = vars(ap.parse_args())

    db_parse_to_json(args["ip"])
    
    
    #
    #
    #  DM = DistanceMatrix()
    #
    #
    #  DM.push("2361 Braun Dr. Golden CO",
    #          identification="home", d_type='o')
    #
    #  DM.push("Pizza Hut",
    #          identification="pizzahut", d_type='o')
    #
    #  DM.push("Walmart",
    #          identification="walmart", d_type='o')
    #
    #  DM.push("Starbucks",
    #          identification="home", d_type='o')
    #
    #  DM.push("CU Boulder Colorado",
    #          identification='boulder', d_type='o')
    #
    #  DM.push("Target",
    #          identification="target", d_type='d')
    #
    #  DM.push("700 31st St Boulder CO",
    #          identification="house", d_type='d')
    #
    #  DM.push("Facebook",
    #          identification="facebook", d_type='d')
    #
    #
    #  DM.to_json_file("./.cache_money/graph.json")
    #


    #  a_node1 = AddressNode(address='2361 Braun Dr. Golden CO',
    #                        cache_file="./.cache_money/addresses.json",
    #                        identification="test")

    #  print(a_node1.address, a_node1.pos)
