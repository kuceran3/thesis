labels = ['BA','FFS','SF','HS','HS1','HR']
labels3 = ['FFS','SF','HS','HS1']
f1k0 = [62.6976,39.7566+0.467872,32.0781+3.241e-06+0.499143,51.3738+2.54579+0.176441,27.6832+2.94072+0.184241,26.3921+1.18727+0.199457]
f1k4 = [65.9625,21.7993+0.429102,27.5619+5.3e-08+1.64251,36.6696+3.68778+0.18025,18.8836+3.52902+0.180869,18.2954+2.63494+0.180422]
f1k16 = [143.887,20.1827+0.875461,19.1157+5.9e-08+2.0685,26.2532+5.83729+0.180028,12.4882+5.81776+0.178946,12.0187+148.088+0.249398]
f1k64 = [16.6402+1.35281,13.8718+6.3e-08+1.8608,21.2315+13.1568+0.202564,9.23871+13.26+0.180162]

f0k0 = [58.2711,30.3232+0.103754,29.7964+2.559e-06+0.102836,50.882+2.5244+0.0347465,27.6168+2.94301+0.0346743,27.5243+1.06963+0.0350625]
f0k4 = [59.8218,21.0389+0.104599,21.6378+2.28e-06+0.10331,35.0455+3.24838+0.0348398,18.2123+3.17592+0.034923,17.3452+2.55619+0.0348983]
f0k16 = [137.863,15.5111+0.104635,15.4087+0.0182273+0.10301,25.0334+5.23119+0.0349379,11.419+5.08747+0.0350661,12.1047+138.384+0.0393798]
f0k64 = [12.7756+1.22052,13.5795+1.58158+0.134901,19.4888+12.4855+0.0366742,8.87639+12.4652+0.0392737]

f2k0 = [63.0026,32.0714+2.57754,31.8241+2.606e-06+3.06998,54.8633+2.70401+1.49625,28.505+3.14271+1.50762,27.9548+1.22174+1.52158]
f2k4 = [62.3756,21.8738+2.5402,22.0519+2.496e-06+2.95028,35.8438+3.58794+1.48597,17.8081+3.46849+1.46702,17.5872+2.69849+1.54222]
f2k16 = [145.018,15.9111+2.49403,16.446+4.454e-06+3.19437,25.0349+5.53956+1.53205,11.684+5.6765+1.59549,11.9655+136.516+1.51747]
f2k64 = [13.5717+2.72442,13.5373+1.62682+2.7347,28.505+3.14271+1.50762,9.02785+3.2655+1.48228]

import numpy as np
import matplotlib.pyplot as plt

plt.rcParams.update({'font.size': 40})


#k0
width = 0.3
ind = np.arange(len(f0k0))
fig, ax = plt.subplots()
rects1 = ax.bar(ind, f0k0, width, color = 'seagreen')
rects2 = ax.bar(ind + width, f1k0, width, color = 'darkorange')
rects2 = ax.bar(ind + 2*width, f2k0, width, color = 'royalblue')
ax.set_axisbelow(True)
plt.minorticks_on()
ax.set_ylabel('s', rotation='horizontal')
ax.yaxis.set_label_coords(-0.02, 1)
ax.tick_params(axis='x',which='minor',bottom='off')
ax.yaxis.grid(True, which='major', color='gray', linestyle='dashed')
ax.yaxis.grid(True, which='minor', color='gray', linestyle='dashed')
ax.set_xticks(ind + width)
ax.set_xticklabels(labels)
plt.show()

#k4
fig, ax = plt.subplots()
rects1 = ax.bar(ind, f0k4, width, color = 'seagreen')
rects2 = ax.bar(ind + width, f1k4, width, color = 'darkorange')
rects2 = ax.bar(ind + 2*width, f2k4, width, color = 'royalblue')
ax.set_axisbelow(True)
plt.minorticks_on()
ax.set_ylabel('s', rotation='horizontal')
ax.yaxis.set_label_coords(-0.02, 1)
ax.tick_params(axis='x',which='minor',bottom='off')
ax.yaxis.grid(True, which='major', color='gray', linestyle='dashed')
ax.yaxis.grid(True, which='minor', color='gray', linestyle='dashed')
ax.set_xticks(ind + width)
ax.set_xticklabels(labels)
ax.set_ylim((0, 70)) 
plt.show()

#k16
fig, ax = plt.subplots()
rects1 = ax.bar(ind, f0k16, width, color = 'seagreen')
rects2 = ax.bar(ind + width, f1k16, width, color = 'darkorange')
rects2 = ax.bar(ind + 2*width, f2k16, width, color = 'royalblue')
ax.set_axisbelow(True)
plt.minorticks_on()
ax.set_ylabel('s', rotation='horizontal')
ax.yaxis.set_label_coords(-0.02, 1)
ax.tick_params(axis='x',which='minor',bottom='off')
ax.yaxis.grid(True, which='major', color='gray', linestyle='dashed')
ax.yaxis.grid(True, which='minor', color='gray', linestyle='dashed')
ax.set_xticks(ind + width)
ax.set_xticklabels(labels)
ax.set_ylim((0, 170)) 
plt.show()

#k64
ind = np.arange(len(f0k64))
fig, ax = plt.subplots()
rects1 = ax.bar(ind, f0k64, width, color = 'seagreen')
rects2 = ax.bar(ind + width, f1k64, width, color = 'darkorange')
rects2 = ax.bar(ind + 2*width, f2k64, width, color = 'royalblue')
ax.set_axisbelow(True)
plt.minorticks_on()
ax.set_ylabel('s', rotation='horizontal')
ax.yaxis.set_label_coords(-0.02, 1.02)
ax.tick_params(axis='x',which='minor',bottom='off')
ax.yaxis.grid(True, which='major', color='gray', linestyle='dashed')
ax.yaxis.grid(True, which='minor', color='gray', linestyle='dashed')
ax.set_xticks(ind + width)
ax.set_xticklabels(labels3)
ax.set_ylim((0, 40)) 
plt.show()
