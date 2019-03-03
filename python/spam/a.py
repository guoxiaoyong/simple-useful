import pyximport
pyximport.install(inplace=True)

import sumn
print(sumn.add(100000000))
