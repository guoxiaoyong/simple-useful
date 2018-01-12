#myfile.py
from bokeh.plotting import curdoc, figure
doc = curdoc()
p1 = figure(width=1500, height=230, active_scroll="wheel_zoom")
doc.add_root(p1)
doc.clear()
p2 = figure(width=1500, height=500, active_scroll="wheel_zoom")
doc.add_root(p2)
