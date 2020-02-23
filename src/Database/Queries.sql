CREATE TABLE IF NOT EXISTS Foods(
    FoodID SERIAL PRIMARY KEY,
    FoodName VARCHAR(100) NOT NULL,
    FoodType VARCHAR (100) NOT NULL,
    ShelfLife DATE,
    Present BOOL,
    Parishable BOOL
);