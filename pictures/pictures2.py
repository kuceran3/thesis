from netCDF4 import Dataset
import numpy as np

import matplotlib.pyplot as plt
from mpl_toolkits.basemap import Basemap

my_example_nc_file = 'C:/Users/horak/Downloads/HIRS-Ch12-TB_v03r00_MonthlyGrid-InterSatCal_d2011_c20151006.nc'
fh = Dataset(my_example_nc_file, mode='r')

print(fh.dimensions)
print(fh.variables.keys())

lons = fh.variables['lon'][:]
lats = fh.variables['lat'][:]
tmax = fh.variables['time'][:]
ch = fh.variables['ch12'][:]
timeb = fh.variables['time_bounds'][:]
#print(len(fh.variables['tcdr_MSU_AMSUA_channel_TLS'][:]))

#print(fh.variables)

tmax_units = fh.variables['ch12'].units
fh.close()

# Get some parameters for the Stereographic Projection
lon_0 = lons.mean()
lat_0 = lats.mean()

print(lon_0, lat_0)
#print(fh.variables.keys())
#print(lats)
print(lons.shape, lats.shape, tmax.shape, ch.shape, timeb.shape)

m = Basemap(width=5000000,height=3500000, resolution='l',projection='stere', lat_ts=40,lat_0=lat_0,lon_0=lon_0)

# Because our lon and lat variables are 1D, 
# use meshgrid to create 2D arrays 
# Not necessary if coordinates are already in 2D arrays.
lon, lat = np.meshgrid(lons, lats)
xi, yi = m(lon, lat)

# Plot Data
cs = m.pcolor(xi,yi,np.squeeze(ch[0]))

# Add Grid Lines
m.drawparallels(np.arange(-80., 81., 10.), labels=[1,0,0,0], fontsize=10)
m.drawmeridians(np.arange(-180., 181., 10.), labels=[0,0,0,1], fontsize=10)

# Add Coastlines, States, and Country Boundaries
m.drawcoastlines(linewidth=0.25)
m.drawstates()
m.drawcountries()
#m.fillcontinents(color="coral", lake_color="aqua")

# Add Colorbar
cbar = m.colorbar(cs, location='bottom', pad="10%")
cbar.set_label(tmax_units)

# Add Title
plt.title('time')

plt.show()


cs = m.pcolor(xi,yi,np.squeeze(ch[1]))
cbar = m.colorbar(cs, location='bottom', pad="10%")
m.drawparallels(np.arange(-80., 81., 10.), labels=[1,0,0,0], fontsize=10)
m.drawmeridians(np.arange(-180., 181., 10.), labels=[0,0,0,1], fontsize=10)
m.drawcoastlines(linewidth=0.25)
m.drawstates()
m.drawcountries()
plt.title('time')
plt.show()


cs = m.pcolor(xi,yi,np.squeeze(ch[2]))
cbar = m.colorbar(cs, location='bottom', pad="10%")
m.drawparallels(np.arange(-80., 81., 10.), labels=[1,0,0,0], fontsize=10)
m.drawmeridians(np.arange(-180., 181., 10.), labels=[0,0,0,1], fontsize=10)
m.drawcoastlines(linewidth=0.25)
m.drawstates()
m.drawcountries()
plt.title('time')
plt.show()

