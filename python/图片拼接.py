# join the below 16 images of each folder into one picture
import PIL.Image as Image
import os

# 1. set image's attribute.
IMAGE_WIDTH = 1920
IMAGE_HEIGHT = 1080

# 2. Final image's rows and columns.
IMAGE_ROW = 4
IMAGE_COLUMN = 4

# 3. create final image.
dst_image = Image.new(
    'RGB', (IMAGE_COLUMN*IMAGE_WIDTH, IMAGE_ROW*IMAGE_HEIGHT))

for row in range(IMAGE_ROW):
    for col in range(IMAGE_COLUMN):
        # 4. find the image to be used.
        num = col + IMAGE_COLUMN * row
        IMAGE_NAME = './undistort/color/%04d.png' % num

        # 5. processing this image to right size
        src_image = Image.open(IMAGE_NAME).resize(
            (IMAGE_WIDTH, IMAGE_HEIGHT), Image.ANTIALIAS)

        # 6. paste this image to right position.
        src_dst_image.paste(src_image, (col*IMAGE_WIDTH, row*IMAGE_HEIGHT))

dst_image.save('final.png')
