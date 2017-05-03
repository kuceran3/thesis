labels = ['BA','FFS','SF','HS','HS1','HR']
labels3 = ['FFS','SF','HS','HS1']
f1k0 = [62.6976,39.7566+0.467872,32.0781+3.241e-06+0.499143,51.3738+2.54579+0.176441,27.6832+2.94072+0.184241,26.3921+1.18727+0.199457]
f1k4 = [65.9625,21.7993+0.429102,27.5619+5.3e-08+1.64251,36.6696+3.68778+0.18025,18.8836+3.52902+0.180869,18.2954+2.63494+0.180422]
f1k16 = [143.887,20.1827+0.875461,19.1157+5.9e-08+2.0685,26.2532+5.83729+0.180028,12.4882+5.81776+0.178946,12.0187+148.088+0.249398]
f1k64 = [16.6402+1.35281,13.8718+6.3e-08+1.8608,21.2315+13.1568+0.202564,9.23871+13.26+0.180162]

f0k0 = [3968.64,981.951+19.0291,946.23+5.4e-08+19.0881,42.2523+186.248+16.944,948.873+180.708+22.6761,907.762+2.82148+18.1267]
f0k4 = [3859.71,600.365+18.6437,582.568+3.8e-08+18.936,1369.14+371.261+17.3985,554.188+380.057+23.5118,579.389+1.98123+18.5253]
f0k16 = [3795.89,387.686+18.6383,380.368+6.3e-08+18.9282,801.125+408.044+17.2598,318.724+401.434+23.3794,309.473+287.497+22.1856]
f0k64 = [280.368+18.6707,280.13+3.5e-08+18.8789,525.537+513.824+17.5152,231.805+521.62+25.9596]

f2k0 = [1229.42,557.904+0.486665,538.396+1.765e-05+0.78433,966.622+239.473+0.161787,506.077+238.446+0.188497,495.444+437.206+0.818015]
f2k4 = [100000,369.259+0.473206,375.008+1.899e-06+0.494005,629.002+287.213+0.0995565,306.091+290.57+0.347694,297.713+654.65+0.349238]
f2k16 = [100000,270.818+0.474934,269.392+2.143e-06+0.506257,437.461+348.844+0.134431,209.776+351.15+0.90204,201.959+10000]
f2k64 = [220.939+240.197,221.652+43.6142+0.632604,344.163+491.762+0.139229,152.812+485.8+0.337501]

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
ax.tick_params(axis='x',which='minor',bottom='off')
ax.yaxis.grid(True, which='major', color='gray', linestyle='dashed')
ax.yaxis.grid(True, which='minor', color='gray', linestyle='dashed')
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
ax.tick_params(axis='x',which='minor',bottom='off')
ax.yaxis.grid(True, which='major', color='gray', linestyle='dashed')
ax.yaxis.grid(True, which='minor', color='gray', linestyle='dashed')
ax.set_xticks(ind + width)
ax.set_xticklabels(labels3)
ax.set_ylim((0, 160)) 
plt.show()

