from PIL import Image

def rgb2num(color):
  if len(color) == 4:
    m = (1, 256, 256*256, 256*256*256)
    return m[0]*color[3]+m[1]*color[2]+m[2]*color[1]+m[3]*color[0]
  else:
    m = (1, 256, 256*256)
    return m[0]*color[2]+m[1]*color[1]+m[2]*color[0]


im = Image.open('fish.jpg', 'r')
width, height = im.size
for color in im.getdata():
  print(rgb2num(color))
