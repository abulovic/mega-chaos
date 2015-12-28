import numpy as np
from random import random

t = np.arange(0., 1000., 0.1)
sine = np.sin(t) + np.random.rand(10000,) / 5. + np.sin(0.09 * t)
with open('sine.txt', 'w') as fout:
	fout.write(','.join(map(lambda x: str(x), sine)))

import matplotlib.pyplot as plt
plt.plot(sine)
plt.show()