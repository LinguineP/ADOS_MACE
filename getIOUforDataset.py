import os
import pprint
import csv


def search_and_read_label(directory, file_name):
    file_path = os.path.join(directory, file_name)
    
    if os.path.isfile(file_path) and file_name.endswith('.txt'):
         f = open(directory+'/'+file_name, "r")
         lines = f.readlines()
         f.close()
            
         for line in lines:
                values = line.strip().split(' ')
                class_id = int(values[0])
                x_center, y_center, width, height = map(float, values[1:])
                label=(file_name,class_id,(x_center, y_center, width, height))
         return label
    return None


    

def makeIoUcsv(outfilename,realPath, pathList, pictureList):
    
    bestPath=pathList[0]
    lastPath=pathList[1]
    stockPath=pathList[2]
    
    filename = outfilename+'.csv'
    csvfile= open(filename, 'w')
    
    csvwriter=csv.writer(csvfile)
    
    fields = ['PictureName', 'IOU_stock_yoloV8s', 'IOU_best', 'IOU_last']
    
    csvwriter.writerow(fields)
    
  
    
    for pic in pictureList:
        realLabel = search_and_read_label(realPath,pic)
        bestLabel = search_and_read_label(bestPath,pic)
        lastLabel = search_and_read_label(lastPath,pic)
        stockLabel = search_and_read_label(stockPath,pic)
        
        bestIoU=-1
        lastIoU=-1
        stockIoU=-1
        
        if bestLabel is not None:
            bestIoU=calculate_iou(realLabel[2],bestLabel[2])
        if lastLabel is not None:
            lastIoU=calculate_iou(realLabel[2],lastLabel[2])
        if stockLabel is not None:
            stockIoU=calculate_iou(realLabel[2],stockLabel[2])        
        
        
        row = [pic.strip('.txt'), stockIoU, bestIoU, lastIoU]
    
        csvwriter.writerow(row)
        
        print(pic.strip('.txt')+'................ is done')
        
    
def get_list_of_pics(pathList):
    
    picture_list=[]
    
    for path in pathList:
        for file_name in os.listdir(path):
            if picture_list.count(file_name) == 0:
                picture_list.append(file_name)
    
    
    return picture_list 
          
    

def calculate_iou(box1, box2):
    """
    Calculate the Intersection over Union (IoU) for two bounding boxes.
    Each bounding box is represented as a tuple (x, y, width, height).
    """
    x1, y1, w1, h1 = box1
    x2, y2, w2, h2 = box2

    # Calculate the coordinates of the intersection rectangle
    x_left = max(x1, x2)
    y_top = max(y1, y2)
    x_right = min(x1 + w1, x2 + w2)
    y_bottom = min(y1 + h1, y2 + h2)

    # If the intersection is negative (no overlap), return 0
    if x_right <= x_left or y_bottom <= y_top:
        return 0.0

    # Calculate the areas of the bounding boxes
    area_box1 = w1 * h1
    area_box2 = w2 * h2

    # Calculate the area of the intersection rectangle
    intersection_area = (x_right - x_left) * (y_bottom - y_top)

    # Calculate the Union area 
    union_area = area_box1 + area_box2 - intersection_area

    # Calculate the IoU
    iou = intersection_area / union_area
    
    return iou


################################################################################################################################################
print('\n--------------------------------------------------')
print('---------------------IOU calculation--------------')
print('--------------------------------------------------\n\n')


outFile=input("Specify Output File Name: " )


print('\n----------------------starting calculation--------------\n\n')




realLabelPath=r"C:\Users\vpavl\Desktop\mace\MACE_ADOS_PZ1_Labeling_and_Augmentation\datasets\validate\labels"

bestPredictionLabelPath=r"C:\Users\vpavl\Desktop\mace\MACE_ADOS_PZ1_Labeling_and_Augmentation\predictionResultDumpBest\predict\labels"

lastPredictionLabelPath=r"C:\Users\vpavl\Desktop\mace\MACE_ADOS_PZ1_Labeling_and_Augmentation\predictionResultDumpLast\predict\labels"

stockPredictionLabelPath=r"C:\Users\vpavl\Desktop\mace\MACE_ADOS_PZ1_Labeling_and_Augmentation\predictionResultDumpStock\predict\labels"

pathList=[]

pathList.append(bestPredictionLabelPath)
pathList.append(lastPredictionLabelPath)
pathList.append(stockPredictionLabelPath)


pictureList=get_list_of_pics(pathList)



makeIoUcsv(outFile,realLabelPath,pathList,pictureList)

print('\n--------------------------------------------------\n\n')



