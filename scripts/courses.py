import csv
import random

# Generate random course names
course_names = [
    "Introduction to Programming",
    "Data Structures",
    "Database Management",
    "Computer Networks",
    "Artificial Intelligence",
    "Web Development",
    "Software Engineering",
    "Machine Learning",
    "Operating Systems",
    "Computer Graphics",
]

# Generate random departments
departments = [
    "Computer Science",
    "Electrical Engineering",
    "Mechanical Engineering",
    "Mathematics",
    "Physics",
    "Biology",
    "Chemistry",
    "Economics",
    "Psychology",
    "Business",
]

# Generate random course descriptions
course_descriptions = [
    "An introductory course to programming concepts.",
    "Explore the fundamentals of data structures.",
    "Learn about designing and managing databases.",
    "Study the principles of computer networks.",
    "Introduction to AI techniques and algorithms.",
    "Build dynamic websites and web applications.",
    "Learn software development methodologies.",
    "Discover machine learning algorithms.",
    "Study the design and implementation of operating systems.",
    "Explore computer graphics and visualizations.",
]

# Generate random credit values
credits = [3, 4]

# Generate CSV data
csv_rows = []
for i in range(1, 101):
    course_name = random.choice(course_names)
    department = random.choice(departments)
    credit = random.choice(credits)
    course_description = random.choice(course_descriptions)

    csv_rows.append([i, course_name, department, credit, course_description])

# Write CSV data to a file
csv_filename = "courses_data.csv"
with open(csv_filename, mode="w", newline="") as csv_file:
    csv_writer = csv.writer(csv_file)
    csv_writer.writerow(
        ["CourseID", "CourseName", "Department", "Credits", "CourseDescription"]
    )
    csv_writer.writerows(csv_rows)

print(f"Generated {len(csv_rows)} rows of data in '{csv_filename}'.")
