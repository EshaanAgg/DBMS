import mysql.connector
from faker import Faker
import random

# Create a connection to the MySQL database
db = mysql.connector.connect(
    host="localhost", user="root", password="SuperSecretPassword123$", database="ecomm"
)

# Create a cursor object to interact with the database
cursor = db.cursor()

# Initialize the Faker object
fake = Faker()

numberUsers = 100000
numberCategories = 30000
numberProducts = 200000
numberOrders = 300000


# Define a function to generate and insert fake users
def insertUsers():
    for i in range(numberUsers):
        print("User: ", i + 1)

        first_name = fake.first_name()
        last_name = fake.last_name()
        # Append the serial number to the email to ensure that the UNIQUE constraint in not violated
        email = str(i + 1) + fake.email()
        password = fake.password()

        # SQL query to insert a new user into the Users table
        insert_query = "INSERT INTO Users (FirstName, LastName, Email, Password) VALUES (%s, %s, %s, %s)"
        user_data = (first_name, last_name, email, password)

        cursor.execute(insert_query, user_data)
    db.commit()


def insertCategories():
    for i in range(numberCategories):
        print("Category: ", i + 1)

        category_name = fake.word()

        # SQL query to insert a new category into the Categories table
        insert_query = "INSERT INTO Categories (Name) VALUES (%s)"
        category_data = (category_name,)

        cursor.execute(insert_query, category_data)

    db.commit()


def insertProducts():
    for i in range(numberProducts):
        print("Product: ", i + 1)

        product_name = fake.word()
        description = fake.text()
        price = round(random.uniform(10, 500), 2)
        category_id = int(random.uniform(2, numberCategories - 2))

        # SQL query to insert a new product into the Products table
        insert_query = "INSERT INTO Products (Name, Description, Price, CategoryID) VALUES (%s, %s, %s, %s)"
        product_data = (product_name, description, price, category_id)

        cursor.execute(insert_query, product_data)

    db.commit()


def insertOrders():
    for i in range(numberOrders):
        print("Order: ", i + 1)

        user_id = int(random.uniform(2, numberUsers - 2))
        product_id = int(random.uniform(2, numberProducts - 2))
        order_date = fake.date_time_between(start_date="-1y", end_date="now")
        quantity = random.randint(1, 10)

        # SQL query to insert a new order into the Orders table
        insert_query = "INSERT INTO Orders (UserID, ProductID, OrderDate, Quantity) VALUES (%s, %s, %s, %s)"
        order_data = (user_id, product_id, order_date, quantity)

        cursor.execute(insert_query, order_data)

    db.commit()


# Insert all the data
insertUsers()
insertCategories()
insertProducts()
insertOrders()

# Close the database connection
db.close()
