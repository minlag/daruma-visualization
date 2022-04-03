# Daruma Visualization

Tools to create animated visualizations of the various LED driver waveforms from my [daruma](https://github.com/minlav/daruma) project. Includes example input .csv and output .gif files.

<img src="https://github.com/minlav/daruma-visualization/blob/main/Sin8.gif" width=40% height=40%>
<img src="https://github.com/minlav/daruma-visualization/blob/main/Sin8_Log.gif" width=40% height=40%>

## Motivations

These visualizations are a convenient way to compare the outputs of various LED driver algorithms. By changing the `LOG_SCALE` parameter to `True` a logarithmic scale that approximates the brightness perception of the human eye will be applied.

## Usage

1. Open `daruma_visualization.ino` with the Arduino IDE and upload it to your board of choice.
1. Copy the full output of the serial monitor and save it as a .csv file in the same directory as `graph_plotter.py`. 
1. Within `graph_plotter.py` change the `CSV_TITLE = ` parameter to equal the title of your created .csv.
1. Finally, change the other parameters to suit your purposes, run `graph_plotter.py`, and your beautiful animation will be created.
