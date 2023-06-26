import os
import random
import shutil

def copy_random_photos(source_directory, destination_directory, num_photos):
    photo_files = [
        file for file in os.listdir(source_directory)
        if os.path.isfile(os.path.join(source_directory, file))
    ]
    random_photos = random.sample(photo_files, min(num_photos, len(photo_files)))
    
    for photo in random_photos:
        source_path = os.path.join(source_directory, photo)
        destination_path = os.path.join(destination_directory, photo)
        shutil.copy2(source_path, destination_path)
        print(f"Copied {source_path} to {destination_path}")

# Usage example
source_directory = r"C:\Users\vpavl\Desktop\mace\MACE_ADOS_PZ1_Labeling_and_Augmentation\datasets\validate\images"  # Replace with the actual source directory path
destination_directory = r"C:\Users\vpavl\Desktop\mace\MACE_ADOS_PZ1_Labeling_and_Augmentation\datasets\test"  # Replace with the actual destination directory path
copy_random_photos(source_directory, destination_directory, num_photos=100)


