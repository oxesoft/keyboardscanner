import numpy as np
import matplotlib.pyplot as plt

x = np.linspace(0, 1, 128)

x = np.linspace(0, 1, 128)

velocity_floor = 12

split = 0.42        # previously: 0.35 → mid range starts later
gamma_low = 0.55    # keeps pianissimo audible
gamma_mid = 1.8     # holds mp/mf more firmly

y = np.zeros_like(x)

# pianissimo / piano region
low = x <= split
y[low] = np.power(x[low] / split, gamma_low) * split

# mid / forte region
high = x > split
y[high] = split + np.power(
    (x[high] - split) / (1 - split),
    gamma_mid
) * (1 - split)

curve = np.round(
    velocity_floor + y * (127 - velocity_floor)
).astype(int)

lines = ["const byte VELOCITY_CURVE[] = {", "    " + ", ".join(map(str, curve)), "};"]
with open("../velocity.h", "w") as file:
    for line in lines:
        file.write(line + "\n")

plt.plot(curve)
plt.xlabel("Input")
plt.ylabel("Output")
plt.grid(True)
plt.show()
