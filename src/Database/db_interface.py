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
            self.cxn = psql.connect(host="172.22.0.2",database="HackCU", user="Liam", password="liam")
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
            FoodName VARCHAR(100) NOT NULL, \
            FoodType VARCHAR (100) NOT NULL, \
            ShelfLife INTEGER, \
            Present BOOL,\
			Parishable BOOL \
            );"
        self.cursor.execute(Food)

				# Creates a table for Restaurants
        Restaurant = "CREATE TABLE IF NOT EXISTS Restaurant( \
            RestaurantID SERIAL PRIMARY KEY, \
            RestaurantName VARCHAR (100) NOT NULL, \
            Address VARCHAR(100) NOT NULL \
            );"
        self.cursor.execute(Restaurant)

        Donation_Center = "CREATE TABLE IF NOT EXISTS Donation_Center( \
            DonationCenterID SERIAL PRIMARY KEY, \
            Address VARCHAR(100) NOT NULL, \
            Capacity INTEGER NOT NULL, \
            Requested INTEGER NOT NULL, \
            DonationCenterName VARCHAR(100) \
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
            DateDonated DATE NOT NULL, \
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
        self.cursor.execute(FoodAmounts)

        # Execute and commit the sql
        self.commitChanges()

    def commitChanges(self):
        """
            Summay: This will be used only to commit changes so that its done at specific points rather than after every function call.
            Input: none
            Output: none
        """
        self.cxn.commit()

    def Destroy(self):
        """ Destroys the database. For testing pusposes only """
        self.cursor.execute("DROP TABLE IF EXISTS Trucks;")
        self.cursor.execute("DROP TABLE IF EXISTS Donations;")
        self.cursor.execute("DROP TABLE IF EXISTS Restaurant;")
        self.cursor.execute("DROP TABLE IF EXISTS FoodAmount;")
        self.cursor.execute("DROP TABLE IF EXISTS Donation_Center;")
        self.cursor.execute("DROP TABLE IF EXISTS Foods;")


        self.commitChanges()
        print("Tables Destroyed")

        return

################### Database inserting ########################################

    def addFood(self, _name, _type, _shelfLife, _parishable, _present):
        self.cursor.execute("INSERT INTO Foods (foodname, foodtype, shelflife, parishable, present) \
            VALUES (%s, %s, %s, %s, %s)", (_name, _type, _shelfLife, _parishable, _present))
      
        self.commitChanges()
    
    def addRestaurant(self, _name, _address):
        self.cursor.execute("INSERT INTO Restaurant (restaurantname, address) \
            VALUES (%s, %s)", (_name, _address))
      
        self.commitChanges()
    
    def addTruck(self, _city, _capacity):
        self.cursor.execute("INSERT INTO Trucks (City, Capacity) \
            VALUES (%s, %s)", (_city, _capacity))
      
        self.commitChanges()

    def addDonation(self, _foodName, _restaurantName, _donationCenterName, _amount, _dateAdded):
        self.cursor.execute("SELECT FoodID FROM Foods WHERE FoodName LIKE %s", (_foodName,))
        foodID = self.cursor.fetchone()

        self.cursor.execute("SELECT RestaurantID FROM Restaurant WHERE RestaurantName LIKE %s", (_restaurantName,))
        restaurantID = self.cursor.fetchone()

        self.cursor.execute("SELECT DonationCenterID FROM Donation_Center WHERE DonationCenterName LIKE %s", (_donationCenterName,))
        donationCenterID = self.cursor.fetchone()

        self.cursor.execute("INSERT INTO Donations (FoodID, RestaurantID, DonationCenterID, QuantityDonated, DateDonated) \
            VALUES (%s, %s, %s, %s, %s)", (foodID, restaurantID, donationCenterID, _amount, _dateAdded))
      
        self.commitChanges()

    def addDonationCenter(self, _name, _address, _capacity, _requested):
        self.cursor.execute("INSERT INTO Donation_Center (Address, Capacity, Requested, DonationCenterName) \
            VALUES (%s, %s, %s, %s)", (_address, _capacity, _requested, _name))
      
        self.commitChanges()

    def addFoodAmount(self, _foodName, _donationCenterName, _amount):
        self.cursor.execute("SELECT DonationCenterID FROM Donation_Center WHERE DonationCenterName LIKE %s", (_donationCenterName,))
        donationCenterID = self.cursor.fetchall()

        self.cursor.execute("SELECT FoodID FROM Foods WHERE FoodName LIKE %s", (_foodName,))
        foodID = self.cursor.fetchall()

        self.cursor.execute("INSERT INTO FoodAmounts (FoodID, DonationCenterID, Amount) \
            VALUES (%s, %s, %s,)", (foodID, donationCenterID, _amount))
      
        self.commitChanges() 
#################### Database retrieval #######################################
    def getFoodItem(self):
        # Here the SELECT has no ORDER BY since the users table is not dependent on times
        self.cursor.execute("SELECT * FROM Foods;")

        # retrieves all entries should the user not give a request amount
        food_entries = self.cursor.fetchall()

        return food_entries

    def getAddresses(self):
        self.cursor.execute("SELECT Address FROM Restaurant;")
        return self.cursor.fetchall()
    
#################### Database removal #########################################
    def removeFood(self, _foodName):
        self.cursor.execute("DELETE FROM Foods WHERE FoodName = %s;", (_foodName,))
        self.commitChanges()
    
    def removeRestaurant(self, _restaruantName):
        self.cursor.execute("DELETE FROM Restaurant WHERE RestaurantName = %s;", (_restaurantName,))
        self.commitChanges()

    def removeDonationCenter(self, _dontationCenterName):
        self.cursor.execute("DELETE FROM Donation_Center WHERE DonationCenterName = %s;", (_donationCenterName,))
        self.commitChanges()

    def removeTruck(self, _truckID):
        self.cursor.execute("DELETE FROM Trucks WHERE TruckID = %s;", (_truckID,))
        self.commitChanges()

    def removeFoodAmount(self, _foodAmount):
        self.cursor.execute("DELETE FROM FoodAmount WHERE AmountsID = %s;", (_foodAmount,))
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

    interface.addFood("milk", "Dairy", 10, True, True)
    interface.addRestaurant("Liam's Bistro", "1600 Pennsylvania Ave NW, Washington, DC 20500")
    interface.addDonationCenter("Liam's Food Bank", "Westminster, London SW1A 1AA, United Kingdom", 100, 23)
    interface.addTruck("Vatican City", 30)
    interface.addDonation("milk", "Liam's Bistro", "Liam's Food Bank", 2, "12/12/12")
  
    print(interface.getFoodItem())
    print(interface.getAddresses())
    
    #/////////////////////////////////////////////////////

    interface.commitChanges()

if __name__ == "__main__":
    main()
