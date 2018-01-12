from bokeh.models.widgets import Panel, Tabs
from bokeh.layouts import column
from bokeh.plotting import curdoc

col1 = column()
panel1 = Panel(title='Panel 1', child=col1)
col2 = column()
panel2 = Panel(title='Panel 2', child=col2)
root = Tabs(tabs=[panel1, panel2])
curdoc().add_root(root)
