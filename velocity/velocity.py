import numpy as np
import matplotlib.pyplot as plt

x = np.linspace(0, 1, 128)
y = np.power(x, 2.0)  # below 1.0, concave; above 1.0, convex.
minimum_value = 4
curve = np.round(y * (127 - minimum_value) + minimum_value).astype(int)

lines = ["const byte VELOCITY_CURVE[] = {", "    " + ", ".join(map(str, curve)), "};"]
with open("../velocity.h", "w") as file:
    for line in lines:
        file.write(line + "\n")

plt.plot(curve)
plt.xlabel("Input")
plt.ylabel("Output")
plt.grid(True)
plt.show()
