import os
import subprocess

path_in = "datasets/validate/images/"
path_out = "datasets/train/images/"



absolute_path = os.path.dirname(__file__)
full_path = os.path.join(absolute_path, path_in)

entries = os.listdir(path_in)

i=0

for in_img_name in entries:
   
	i+=1
	print(i)
	"""
	os.system('./ImageDSP/build/linux/build-ImageDSP-Desktop_Qt_5_9_5_GCC_64bit-Debug/ImageDSP --prog-name "Augmentation 1" --in-img "datasets/validate/images/' + in_img_name + '" --out-img "datasets/validate/a1/a1_' + in_img_name + '" --params "' + "0;0" + '"')

	os.system('./ImageDSP/build/linux/build-ImageDSP-Desktop_Qt_5_9_5_GCC_64bit-Debug/ImageDSP --prog-name "Augmentation 2" --in-img "datasets/validate/images/' + in_img_name + '" --out-img "datasets/validate/a2/a2_' + in_img_name + '" --params "' + "0;0" +'"')
    
	os.system('./ImageDSP/build/linux/build-ImageDSP-Desktop_Qt_5_9_5_GCC_64bit-Debug/ImageDSP --prog-name "Augmentation 3" --in-img "datasets/validate/images/' + in_img_name + '" --out-img "datasets/validate/a3/a3_' + in_img_name + '" --params "' + "0;0" +'"')
		"""
	#os.system('./ImageDSP/build/linux/build-ImageDSP-Desktop_Qt_5_9_5_GCC_64bit-Debug/ImageDSP --prog-name "Augmentation 4" --in-img "datasets/validate/images/' + in_img_name + '" --out-img "datasets/validate/images_a5/a5_' + in_img_name + '" --params "' +"0;0" + '"')
		

	os.system('./ImageDSP/build/linux/build-ImageDSP-Desktop_Qt_5_9_5_GCC_64bit-Debug/ImageDSP --prog-name "Augmentation 5" --in-img "datasets/validate/images/' + in_img_name + '" --out-img "datasets/validate/images_a5/a5_' + in_img_name + '" --params "' +"0;0" + '"')
		
print(entries)
