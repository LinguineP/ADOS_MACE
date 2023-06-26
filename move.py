import os
import shutil

def move_txt_files(source_directory, destination_directory):
    for root, dirs, files in os.walk(source_directory):
        for file in files:
            if file.endswith(".txt"):
                source_path = os.path.join(root, file)
                destination_path = os.path.join(destination_directory, file)
                shutil.move(source_path, destination_path)
                print(f"Moved {source_path} to {destination_path}")
def move_jpg_files(source_directory, destination_directory):
    for root, dirs, files in os.walk(source_directory):
        for file in files:
            if file.endswith(".jpg"):
                source_path = os.path.join(root, file)
                destination_path = os.path.join(destination_directory, file)
                shutil.move(source_path, destination_path)
                print(f"Moved {source_path} to {destination_path}")

# Usage example
source_directory = r"C:\Users\vpavl\Desktop\testMoverScript\toMove"  # Replace with the actual source directory path
destination_directory_txt = r"C:\Users\vpavl\Desktop\testMoverScript\move"  # Replace with the actual destination directory path
destination_directory_jpg = r"C:\Users\vpavl\Desktop\testMoverScript\move"


move_txt_files(source_directory, destination_directory_txt)
move_jpg_files(source_directory,destination_directory_jpg)

