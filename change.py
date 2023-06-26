import os
os.getcwd()


#this program changes the names and extensions  of all files in a specified directory
collection = "./labels_a5/" #directory name goes here
for i, filename in enumerate(os.listdir(collection)):
    os.rename("./labels_a5/" + filename, "./labels_a5/" +"a5_"+filename)  
#first param write direstory name with a / in the end (do not change +filename pls dont), second param write directiry name with a slash in the name + name + extention
