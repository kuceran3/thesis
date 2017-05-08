#labels2 = ['brute', 'exactSkip', 'appBrute', 'appSplit', 'stricterFilter', 'hash', 'hashOnce', 'hashRolling']
labels2 = ['BE', 'SE', 'BA', 'FFS', 'SF', 'NSH', 'SH', 'LSB']
#labels = ['appBrute','appSplit','stricterFilter','hash','hashOnce','hashRolling']
labels = ['BA','FFS','SF','NSH','SH','LSB']
labels3 = ['FFS','SF','NSH','SH']
f0_1 = [5.69835,5.19847,62.6976,34.7566,33.0781,51.3738,27.6832,26.3921]
p0_1 = [0,0,0,0,3.241e-06,2.54579,2.94072,1.18727]
d0_1 = [0,0,0,0.467872,0.499143,0.176441,0.184241,0.199457]
fp0_1 = [5.69835,5.19847,62.6976,34.7566,33.0781+3.241e-06,51.3738+2.54579,27.6832+2.94072,26.3921+1.18727]

f1k0 = [62.6976,39.7566+0.467872,32.0781+3.241e-06+0.499143,51.3738+2.54579+0.176441,27.6832+2.94072+0.184241,26.3921+1.18727+0.199457]
f1k4 = [65.9625,21.7993+0.429102,27.5619+5.3e-08+1.64251,36.6696+3.68778+0.18025,18.8836+3.52902+0.180869,18.2954+2.63494+0.180422]
f1k16 = [143.887,20.1827+0.875461,19.1157+5.9e-08+2.0685,26.2532+5.83729+0.180028,12.4882+5.81776+0.178946,12.0187+148.088+0.249398]
f1k64 = [16.6402+1.35281,13.8718+6.3e-08+1.8608,21.2315+13.1568+0.202564,9.23871+13.26+0.180162]

f0k0 = [40.2679,16.0737+0.40398,15.6877+1.837e-06+0.427304,39.9948+2.98447+0.139729,14.9432+3.75483+0.139003,13.7939+1.04815+0.139439]
f0k4 = [38.9102,11.8033+0.4137,11.6067+1.892e-06+0.426844,24.9465+3.91189+0.138309,9.21039+3.7264+0.138995,8.803+3.08806+0.139551]
f0k16 = [87.1144,9.00765+0.404225,8.96233+1.94e-06+0.426933,16.2284+6.17302+0.140201,6.01822+6.0387+0.138817,6.15472+210.177+0.139062]
f0k64 = [7.40684+1.27076,7.45584+1.843e-06+1.62994,11.7707+14.1884+0.138801,4.67126+14.5677+0.139643]

f2k0 = [346.302,254.811+0.413671,253.171+3.01e-06+0.47098,299.749+0.369654+0.139129,238.079+0.364398+0.138534,240.688+0.27148+0.138656]
f2k4 = [365.566,168.661+0.412665,167.073+2.809e-06+0.469326,216.249+1.18321+0.138682,147.121+1.16727+0.138899,148.903+1.12235+0.138661]
f2k16 = [886.281,125.147+0.418123,119.653+3.159e-06+0.474357, 166.346+2.28038+0.138073,97.3086+2.23416+0.138273, 95.4027+3.79212+0.1384]
f2k64 = [97.1707+1.74888, 97.929+3.129e-06+2.60808, 142.921+5.7384+0.139334, 72.3039+5.37634+0.138329]


import numpy as np
import matplotlib.pyplot as plt
plt.rcParams.update({'font.size': 40})



width = 0.8
ind = np.arange(len(f0_1))

#all
fig, ax = plt.subplots()
rects1 = ax.bar(ind, f0_1, width, color = 'royalblue', label='find')
rects2 = ax.bar(ind, p0_1, width, color = 'darkorange', bottom = f0_1, label='preverification')
rects3 = ax.bar(ind, d0_1, width, color = 'firebrick', bottom = fp0_1, label='DP check')
ax.set_ylabel('sec', rotation='horizontal')
ax.yaxis.set_label_coords(-0.05, 1.05)
ax.set_xticks(ind)
ax.set_ylim((0, 80)) 
#ax.set_xticklabels(labels2, rotation='vertical')
ax.set_xticklabels(labels2)
#plt.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3, ncol=2, mode="expand", borderaxespad=0.)
plt.legend(loc=9, ncol=3)

plt.show()


#k0
width = 0.3
ind = np.arange(len(f0k0))
fig, ax = plt.subplots()
#rects1 = ax.bar(ind, f0k0, width, color = 'seagreen', label='100%')
rects1 = ax.bar(ind, f0k0, width, color = 'seagreen')
#rects2 = ax.bar(ind + width, f1k0, width, color = 'darkorange', label='50%')
rects2 = ax.bar(ind + width, f1k0, width, color = 'darkorange')
#rects2 = ax.bar(ind + 2*width, f2k0, width, color = 'royalblue', label='2%')
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
ax.yaxis.set_label_coords(-0.05, 1)
ax.yaxis.grid(True, which='major', color='gray', linestyle='dashed')
ax.yaxis.grid(True, which='minor', color='gray', linestyle='dashed')
ax.set_ylabel('sec', rotation='horizontal')

ax.set_xticks(ind + width)
ax.set_xticklabels(labels)
ax.set_ylim((0, 230)) 
plt.show()

#k16
fig, ax = plt.subplots()
rects1 = ax.bar(ind, f0k16, width, color = 'seagreen')
rects2 = ax.bar(ind + width, f1k16, width, color = 'darkorange')
rects2 = ax.bar(ind + 2*width, f2k16, width, color = 'royalblue')
ax.set_axisbelow(True)
plt.minorticks_on()
ax.tick_params(axis='x',which='minor',bottom='off')
ax.yaxis.grid(True, which='major', color='gray', linestyle='dashed')
ax.yaxis.grid(True, which='minor', color='gray', linestyle='dashed')
ax.set_ylabel('sec', rotation='horizontal')
ax.yaxis.set_label_coords(-0.05, 1)
ax.set_xticks(ind + width)
ax.set_xticklabels(labels)
ax.set_ylim((0, 240)) 
plt.show()

#k64
ind = np.arange(len(f0k64))
fig, ax = plt.subplots()
rects1 = ax.bar(ind, f0k64, width, color = 'seagreen')
rects2 = ax.bar(ind + width, f1k64, width, color = 'darkorange')
rects2 = ax.bar(ind + 2*width, f2k64, width, color = 'royalblue')
ax.set_axisbelow(True)
plt.minorticks_on()
ax.yaxis.set_label_coords(-0.05, 1)
ax.yaxis.grid(True, which='major', color='gray', linestyle='dashed')
ax.yaxis.grid(True, which='minor', color='gray', linestyle='dashed')
ax.set_ylabel('sec', rotation='horizontal')
ax.set_xticks(ind + width)
ax.set_xticklabels(labels3)
ax.set_ylim((0, 160)) 
plt.show()
