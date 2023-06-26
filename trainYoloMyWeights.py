from ultralytics import YOLO

# Load a model

model = YOLO('last.pt')  # load a pretrained model (recommended for training)

# Train the model
model.train(task='detect', data='data.yaml', epochs=35, imgsz=640,cache=True)
