labels = ['BA','FFS','SF','HS','HS1','HR']
labels3 = ['FFS','SF','HS','HS1']
f1k0 = [62.6976,39.7566+0.467872,32.0781+3.241e-06+0.499143,51.3738+2.54579+0.176441,27.6832+2.94072+0.184241,26.3921+1.18727+0.199457]
f1k4 = [65.9625,21.7993+0.429102,27.5619+5.3e-08+1.64251,36.6696+3.68778+0.18025,18.8836+3.52902+0.180869,18.2954+2.63494+0.180422]
f1k16 = [143.887,20.1827+0.875461,19.1157+5.9e-08+2.0685,26.2532+5.83729+0.180028,12.4882+5.81776+0.178946,12.0187+148.088+0.249398]
f1k64 = [16.6402+1.35281,13.8718+6.3e-08+1.8608,21.2315+13.1568+0.202564,9.23871+13.26+0.180162]

f0k0 = [60.7028,19.537+0.5323,19.4191+0.547511+0.680663,51.434+2.72578+0.154143,62.7155+2.37147+0.154233,41.1559+0.970904+0.155844]
f0k4 = [60.8622,10.8384+0.531336,10.8367+0.54578+0.677166,20.3981+4.68379+0.155223,12.5192+4.129+0.154002,10.6888+370.455+0.154703]
f0k16 = [133.781,8.48205+9.57418,8.57563+3.49143+0.788893,12.2355+2.58502+0.155701,5.55525+3.49226+0.155979,6.9789+10000]
f0k64 = [8.49029+20.5798,8.58769+4.18862+0.791939,12.2531+3.26113+0.15435,5.55016+4.15911+0.154071]

f2k0 = [149.218,139.347+0.040417,137.782+0.00411719+0.0349779,165.327+1.65234+0.0348747,124.933+1.71145+0.0386926,122.571+0.162193+0.0387313]
f2k4 = [145.958,108.607+0.0407475,103.05+0.00403517+0.0347463,137.369+2.67559+0.0396275,89.9849+2.67541+0.0385319,89.7505+2.08305+0.0392187]
f2k16 = [370.296,78.5144+0.038711,77.1456+0.00405037+0.0351513,102.305+3.687+0.039468,61.4603+4.42872+0.0391674,59.7461+27.2521+0.0412062]
f2k64 = [63.7212+0.560091,62.2292+0.50036+0.0349359,89.1875+8.83776+0.0388849,47.5859+8.2909+0.0391701]

import numpy as np
import matplotlib.pyplot as plt

plt.rcParams.update({'font.size': 40})


#k0
width = 0.3
ind = np.arange(len(f0k0))
fig, ax = plt.subplots()
#rects1 = ax.bar(ind, f0k0, width, color = 'seagreen', label='2')
#rects2 = ax.bar(ind + width, f1k0, width, color = 'darkorange', label='3')
#rects2 = ax.bar(ind + 2*width, f2k0, width, color = 'royalblue', label='4')
rects1 = ax.bar(ind, f0k0, width, color = 'seagreen')
rects2 = ax.bar(ind + width, f1k0, width, color = 'darkorange')
rects2 = ax.bar(ind + 2*width, f2k0, width, color = 'royalblue')
ax.set_axisbelow(True)
plt.minorticks_on()
ax.set_ylabel('sec', rotation='horizontal')
ax.yaxis.set_label_coords(-0.05, 1)
ax.tick_params(axis='x',which='minor',bottom='off')
ax.yaxis.grid(True, which='major', color='gray', linestyle='dashed')
ax.yaxis.grid(True, which='minor', color='gray', linestyle='dashed')
ax.set_xticks(ind + width)
ax.set_xticklabels(labels)
#plt.legend(loc=9, ncol=3, bbox_to_anchor=(0.5, 1.19))
plt.show()

#k4
fig, ax = plt.subplots()
rects1 = ax.bar(ind, f0k4, width, color = 'seagreen')
rects2 = ax.bar(ind + width, f1k4, width, color = 'darkorange')
rects2 = ax.bar(ind + 2*width, f2k4, width, color = 'royalblue')
ax.set_axisbelow(True)
plt.minorticks_on()
ax.set_ylabel('sec', rotation='horizontal')
ax.yaxis.set_label_coords(-0.05, 1)
ax.tick_params(axis='x',which='minor',bottom='off')
ax.yaxis.grid(True, which='major', color='gray', linestyle='dashed')
ax.yaxis.grid(True, which='minor', color='gray', linestyle='dashed')
ax.set_xticks(ind + width)
ax.set_xticklabels(labels)
ax.set_ylim((0, 160)) 
plt.show()

#k16
fig, ax = plt.subplots()
rects1 = ax.bar(ind, f0k16, width, color = 'seagreen')
rects2 = ax.bar(ind + width, f1k16, width, color = 'darkorange')
rects2 = ax.bar(ind + 2*width, f2k16, width, color = 'royalblue')
ax.set_axisbelow(True)
plt.minorticks_on()
ax.set_ylabel('sec', rotation='horizontal')
ax.yaxis.set_label_coords(-0.05, 1)
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
ax.set_ylabel('sec', rotation='horizontal')
ax.yaxis.set_label_coords(-0.05, 1)
ax.tick_params(axis='x',which='minor',bottom='off')
ax.yaxis.grid(True, which='major', color='gray', linestyle='dashed')
ax.yaxis.grid(True, which='minor', color='gray', linestyle='dashed')
ax.set_xticks(ind + width)
ax.set_xticklabels(labels3)
ax.set_ylim((0, 110)) 
plt.show()
