#!/user/bin/env python3

import db_interface as db
import argparse

 # construct the argument parser and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-b", "--build", required=True, type=int, default=0,
    help="Tells the code whether to make new tables of keep the existing ones")
ap.add_argument("-i", "--ip", required=True, type=str, default="172.22.0.2",
    help="Tells the code what the database's IP address is.")
args = vars(ap.parse_args())

print ("Hello, World!")

# Database add check
interface = db.Database()
interface.connectToDatabase(args["ip"])

interface.addRestaurant("Carelli's", "645 30th St, Boulder, CO 80303")
interface.addRestaurant("Cosmo's Pizza", "659 30th St, Boulder, CO 80303")
interface.addRestaurant("May Wah", "2500 Baseline Rd, Boulder, CO 80305")
interface.addRestaurant("Cafe Mexicali", "2850 Baseline Rd, Boulder, CO 80303")
interface.addRestaurant("Moe's Original BBQ - Boulder", "675 30th St, Boulder, CO 80303")
interface.addRestaurant("Taj Indian Cuisine", "2630 Baseline Rd, Boulder, CO 80305")
interface.addRestaurant("Dark Horse", "2922 Baseline Rd, Boulder, CO 80303")
interface.addRestaurant("Oregano's Italian Joint", "2690 Baseline Rd, Boulder, CO 80305")
interface.addRestaurant("Under The Sun Eatery and Pizzeria", "627 South Broadway Street, Boulder, CO 80305")
interface.addRestaurant("Tandoori Grill", "619 S Broadway, Boulder, CO 80305")
interface.addRestaurant("Pho Kitchen Bar & Grill", "unit 3, 2314, 2900 Baseline Rd, Boulder, CO 80303")
interface.addRestaurant("Chautauqua Dining Hall", "900 Baseline Rd, Boulder, CO 80302")
interface.addRestaurant("Hot Pot Noodle House", "4800 Baseline Rd, Boulder, CO 80303")
interface.addRestaurant("South Side Walnut Cafe", "673 S Broadway St, Boulder, CO 80305")
interface.addRestaurant("Starbucks", "2400 Baseline Rd, Boulder, CO 80305")
