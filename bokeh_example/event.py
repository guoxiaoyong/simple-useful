from bokeh.models.callbacks import CustomJS
from bokeh.plotting import figure, curdoc

callback = CustomJS(code="""
// the event that triggered the callback is cb_obj:
// The event type determines the relevant attributes
console.log('Tap event occured at x-position: ' + cb_obj.x + ',' + cb_obj.y)
console.log(cb_obj)
""")

p = figure()
p.line(x=[1,2,3],y=[3,4,5])
p.js_on_event('tap', callback)
p.pprint()
curdoc().add_root(p)
