#!/bin/bash

echo " " > fake_donations.py;

for i in {1..1000}
do
    foodID=$(jot -r 1 1 500)
    donationCenterID=$(jot -r 1 1 4)
    quantity=$(jot -r 1 1 30)
    month=$(jot -r 1 1 12)
    day=$(jot -r 1 1 28)
    year=$(jot -r 1 2019 2020)

    echo "if interface.inFoodAmountTable($foodID) == False:" >> fake_donations.py
    echo "  interface.addDonation($foodID, $donationCenterID, $quantity, '$month/$day/$year')" >> fake_donations.py
    echo "  interface.addFoodAmount($foodID, $donationCenterID, $quantity)" >> fake_donations.py
    echo "else:" >> fake_donations.py
    echo "  interface.updateAmount($foodID, $quantity)" >> fake_donations.py
    echo " " >> fake_donations.py
done