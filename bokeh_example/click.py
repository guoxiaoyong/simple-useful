from bokeh.models.widgets import RadioButtonGroup
from bokeh.plotting import figure, curdoc

def my_radio_handler(*arg):
  print(arg)
  print ('Radio button option selected.')

radio_button_group = RadioButtonGroup(
        labels=["Option 1", "Option 2", "Option 3"], active=0)

radio_button_group.on_click(my_radio_handler)

curdoc().add_root(radio_button_group)
