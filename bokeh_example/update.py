from bokeh.plotting import curdoc
from bokeh.models.widgets import Panel, Tabs, Div, Button
from bokeh.layouts import column

n = 0
def update():
  global n
  tabs.tabs.append(Panel(title="header", child=Div(text='<h1>%s</h>' % n)))
  n += 1

button = Button(label="click me")
button.on_click(update)

tabs = Tabs()
col = column(tabs, button)
curdoc().add_root(col)
