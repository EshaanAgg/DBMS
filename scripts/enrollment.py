import csv
import random

# Generate random student IDs and course IDs
student_ids = list(range(1, 101))
course_ids = list(range(1, 101))

# Generate random enrollments
enrollments = []
for student_id in student_ids:
    num_courses = random.randint(1, 5)
    selected_courses = random.sample(course_ids, num_courses)
    for course_id in selected_courses:
        enrollments.append((student_id, course_id))

# Write enrollment data to CSV file
csv_file_path = "enrolled_courses_data.csv"
with open(csv_file_path, mode="w", newline="") as csv_file:
    csv_writer = csv.writer(csv_file)
    csv_writer.writerow(["StudentID", "CourseID"])
    csv_writer.writerows(enrollments)

print(f"Generated {len(enrollments)} enrollment entries in '{csv_file_path}'.")
