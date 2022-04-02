import csv
import numpy as np
from matplotlib import pyplot as plt
from matplotlib.animation import FuncAnimation
plt.style.use('dark_background')

# This is a program to graph periodic waves using csv files
# Based on the moving sine wave example here:
# https://www.kdnuggets.com/2019/05/animations-with-matplotlib.html

# set params
CSV_TITLE = "Sin8.csv"
PLOT_TITLE = "8-Bit Sine Without Gamma"
FILE_NAME = "Sin8.gif"
SCROLL_SPEED = 5
LOG_SCALE = False
plt.rcParams.update({'font.family':'Helvetica Neue LT Std'})

# axis data
csv_y = []
gen_x = []

# get y-axis data
with open(CSV_TITLE, 'r') as csvfile:
    plots= csv.reader(csvfile, delimiter=',')
    for row in plots:
        csv_y.append(int(row[0])) # create a list of y-axis values
csv_y.extend(csv_y) # repeat y-axis data to show two full waves

# get x-axis data
gen_x=np.arange(0, len(csv_y), 1) # create x-axis with same length as y-axis
gen_x = gen_x.tolist() # convert np.array to a python list

# set up plot and style
fig = plt.figure()
ax = plt.axes(xlim = (-2, 511), ylim = (-1, 270)) # set length of both axis
line, = ax.plot([], [], lw = 0, color = 'white', ms = 1.5, marker = '.')
plt.title(PLOT_TITLE, fontsize = 20, y = 1.03)
plt.yticks(ticks = [0, 128, 255]) # set y-axis ticks for linear scale
plt.tick_params(axis = 'both', left = False, which = 'major', labelsize = 16) # y-axis tick style
plt.xticks(ticks = []) # remove x-axis scale
ax.yaxis.grid(color = 'gray', linestyle = 'dashed') # set horizontal grid style
ax.spines['top'].set_visible(False) # remove box border
ax.spines['right'].set_visible(False)
ax.spines['bottom'].set_visible(False)
ax.spines['left'].set_visible(False)
if LOG_SCALE is True:
    plt.yscale('symlog', base = 2) # make y-axis log scale, symlog allows 0 values

def init():
    line.set_data([], [])
    return line,

def animate(i):
    for x in range(0, SCROLL_SPEED): # change the speed of the animation
        csv_y.append(csv_y.pop(0)) # rotate the list to make it scroll

    x = gen_x # assign the rotated lists
    y = csv_y

    line.set_data(x, y)
    return line,

#interval must be >= 20 when saving .gifs or the output will play slowly
anim = FuncAnimation(fig, animate, init_func = init,
                              frames = len(csv_y) // 2 , interval = 20, blit = True)

# plt.show()

anim.save(FILE_NAME, writer='imagemagick',  dpi= 100)
