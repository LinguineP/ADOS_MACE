from ultralytics import YOLO


# Load a model
model = YOLO('yolov8s.pt')  # load an official detection model

model.predict(source='datasets/test',save_txt=True,save=True,project="predictionResultDumpStock",classes=15)
# Load a model
model = YOLO('best.pt')  # load a pretrained model 


model.predict(source='datasets/test',save_txt=True,save=True,project="predictionResultDumpBest" )  # predict on an image

model = YOLO('last.pt') 

model.predict(source='datasets/test',save_txt=True,save=True,project="predictionResultDumpLast" )  






