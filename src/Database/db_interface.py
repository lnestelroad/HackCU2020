#!/usr/bin/env python3

import os
import psycopg2 as psql
import argparse
import datetime

class Database():
    """
        Summary: This class is used for interaction with the postgresql database. Since postgresql has nice integration with 
            python, it is the perfect solution for incorporating a database.

        Inputs:None
        Outputs: None
    """
    def __init__(self):
        self.cxn = None
        self.cursor = None

################## Database Configuration #####################################
    def connectToDatabase(self):
        """
            Summary: Here is where python makes a connection with the database file. If no connection can 
                be made, the file errors out.

            Inputs: None
            Outputs: A sqlite cursor for interaction with the database.
        """
        try:
            self.path = os.path.dirname(os.path.abspath(__file__))

            # Create a connection to the database
            self.cxn = psql.connect(host="172.21.0.2",database="HackCU", user="Liam", password="liam")
            print("Opening Connections to database")

            # Create a cursor from the database connection
            self.cursor = self.cxn.cursor()

        except psql.Error as error:
            print("Error connecting to database. " + str(error))

    def setupTables(self):
        """
            Summary: Because creating the schema though the command line is a massive pain in the ass, this function will do it for me. 
                The other benifit is for readability as anyone can now see exactly what was done to make this database.

            Inputs: None
            Outputs: Initial database
        """

        # Creates a table for Pictures
        Food = "CREATE TABLE IF NOT EXISTS Foods( \
            FoodID SERIAL PRIMARY KEY, \
            FoodType VARCHAR (100) NOT NULL, \
            ShelfLife DATE, \
            Present BOOL,\
			Parishable BOOL \
            );"
        self.cursor.execute(Food)

				# Creates a table for Restaurants
        Restaurant = "CREATE TABLE IF NOT EXISTS Restaurant( \
            RestaurantID SERIAL PRIMARY KEY, \
            Name VARCHAR (100) NOT NULL, \
            Address VARCHAR(100) NOT NULL \
            );"
        self.cursor.execute(Restaurant)

        Donation_Center = "CREATE TABLE IF NOT EXISTS Donation_Center( \
            DonationCenterID SERIAL PRIMARY KEY, \
            Address VARCHAR(100) NOT NULL, \
            Capacity INTEGER NOT NULL, \
            Requested INTEGER NOT NULL \
            );"
        self.cursor.execute(Donation_Center)

        Trucks = "CREATE TABLE IF NOT EXISTS Trucks( \
            TruckID SERIAL PRIMARY KEY, \
            City VARCHAR(100) NOT NULL, \
            Capacity INTEGER NOT NULL \
            );"
        self.cursor.execute(Trucks)

        # Creates a table for Donations
        Donations = "CREATE TABLE IF NOT EXISTS Donations( \
            DonationsID SERIAL PRIMARY KEY, \
            FoodID INTEGER NOT NULL, \
            RestaurantID INTEGER NOT NULL, \
            DonationCenterID INTEGER NOT NULL, \
            QuantityDonated INTEGER NOT NULL, \
            FOREIGN KEY(RestaurantID) REFERENCES Restaurant(RestaurantID), \
            FOREIGN KEY(FoodID) REFERENCES Foods(FoodID), \
            FOREIGN KEY(DonationCenterID) REFERENCES Donation_Center(DonationCenterID) \
            );"
        self.cursor.execute(Donations)

        # Creates a table for Donations
        FoodAmounts = "CREATE TABLE IF NOT EXISTS FoodAmount( \
            AmountsID SERIAL PRIMARY KEY, \
            FoodID INTEGER NOT NULL, \
            DonationCenterID INTEGER NOT NULL, \
            Amount INTEGER NOT NULL, \
            FOREIGN KEY(FoodID) REFERENCES Foods(FoodID), \
            FOREIGN KEY(DonationCenterID) REFERENCES Donation_Center(DonationCenterID) \
            );"
        self.cursor.execute(Donations)

        # Execute and commit the sql
        self.cxn.commit()

    def commitChanges(self):
        """
            Summay: This will be used only to commit changes so that its done at specific points rather than after every function call.
            Input: none
            Output: none
        """
        self.cxn.commit()

    def Destroy(self):
        """ Destroys the database. For testing pusposes only """
        self.cursor.execute("DROP TABLE IF EXISTS Foods;")
        self.cursor.execute("DROP TABLE IF EXISTS Restaurant;")
        self.cursor.execute("DROP TABLE IF EXISTS Donations;")
        self.cursor.execute("DROP TABLE IF EXISTS Donation_Center;")
        self.cursor.execute("DROP TABLE IF EXISTS Trucks;")

        self.commitChanges()

        return

################### Database inserting ########################################

    def addFood(self, name, quant, expiration, food_type):
        """
            Summary: Here is where the admin will be able to add new rooms the the database. Will probably never be used except during setup.
            Input:  Requires a name for the room
            Output: New entry in the rooms table
        """
        if expiration == '0':
            self.cursor.execute("INSERT INTO Food (food_name, food_quantity, food_type) VALUES ('{}', '{}', '{}');".format(name, quant, food_type))
        else:
            self.cursor.execute("INSERT INTO Food (food_name, food_quantity, food_expiration, food_type) VALUES ('{}', '{}', '{}' ,'{}');".format(name, quant, expiration, food_type))
    
#################### Database retrieval #######################################
    def getFoodItem(self, foodName):
        """
            Summary: This will be used by the GUI to show the admin the current Users Table. Using the request amount method, this function will only
                return a limited amount
            Input: Amount request (optional) <int>
            Output: All entries in the user table <list of tuples>
        """
        # Here the SELECT has no ORDER BY since the users table is not dependent on times
        self.cursor.execute("SELECT * FROM Food WHERE food_name = {};".format(foodName))

        # retrieves all entries should the user not give a request amount
        food_entries = self.cursor.fetchall()

        return food_entries

    def getAllFood(self):
        self.cursor.execute("SELECT * FROM Food;")
        food = self.cursor.fetchall()

        return food
    
#################### Database removal #########################################
    def removeFood(self, _foodName):
        """
            Summary: This is what will be called whenever a user is needed to be removed from the database
        """
        self.cursor.execute("DELETE FROM Food WHERE UserName = {};".format(_foodName,))
        self.commitChanges()
        
############################################################## Main Testing

def main():
    """ This is used only for testing purposes"""

    # construct the argument parser and parse the arguments
    ap = argparse.ArgumentParser()
    ap.add_argument("-b", "--build", required=True, type=int, default=0,
        help="path to serialized db of facial encodings")
    args = vars(ap.parse_args())

    print ("Hello, World!")

    # Database add check
    interface = Database()
    interface.connectToDatabase()

    # checks to see if flag was tripped
    if args["build"] == 1:
        interface.setupTables()
    
    else:
        # Database removal check
        interface.Destroy()
        interface.setupTables()
  
    #/////////////////////////////////////////////////////

    interface.commitChanges()

if __name__ == "__main__":
    main()
