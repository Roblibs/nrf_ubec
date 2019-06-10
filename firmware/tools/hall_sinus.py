#%%
import matplotlib.pyplot as plt
import numpy as np

#%%
two_third_pi = 2*np.pi/3
two_pi = 2 * np.pi
a = np.arange(0.0, two_pi, 0.005)
h1 = 45 * np.sin(a)
h2 = 45 * np.sin(a + two_third_pi)
h3 = 45 * np.sin(a - two_third_pi)

plt.style.use('classic')
fig = plt.figure(figsize=[10,5])
pi_x = [0,np.pi/2, np.pi, 3*np.pi/2,two_pi]
pi_labels = ["0", "pi/2", "pi", "3pi/2","2pi"]

ax = fig.add_axes([0, 0, 1, 1])

ax.plot(a, h1)
ax.plot(a, h2)
ax.plot(a, h3)
ax.set(xlabel='angle(rad)', ylabel='voltage (mV)',
       title='hall sensors Voltage over angle')#,xticks=[pi_x,pi_labels]
ax.set_xticks(pi_x)
ax.set_xticklabels(pi_labels)
ax.set_yticks([-45,0,45])
ax.grid(which='major')
plt.savefig("hall_pi.png",bbox_inches='tight')
plt.show()

#%%

#%%
plt.style.available