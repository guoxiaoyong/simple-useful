import ast
import numpy as np
import pandas as pd
import time
import multiprocessing as mp
from tornado.ioloop import IOLoop
from bokeh.application.handlers import FunctionHandler
from bokeh.application import Application
from bokeh.layouts import column
from bokeh.models import ColumnDataSource, Slider
from bokeh.models.widgets import Button
from bokeh.models import CheckboxGroup
from bokeh.plotting import figure
from bokeh.server.server import Server


class ViewerSpectrum:

    def __init__(self):
        x = np.array([1, 2, 3])
        y1 = np.array([11, 12, 13])
        self._io_loop = IOLoop.current()
        self._source = ColumnDataSource()
        self._curr_plot = 0

    def _build_source(self, dataset):
        if len(dataset) > 0:
            d = {k: dataset[k][:, 1] for k in dataset}
            for k in dataset:
                d['waves'] = dataset[k][:, 0]
                break
            self._source = ColumnDataSource(pd.DataFrame(d))


    def _modify_doc(self, doc):
        # The plot
        lines = []
        plot = figure(webgl=False)
        for k in self._source.data:
            if k != 'waves' and k != 'index':
                lines.append(plot.line('waves', k, source=self._source))

        # If I do visible = False in plot.line things dont work.
        # This two step process works.
        for i, line in enumerate(lines):
            if i % 10 == 0:
                line.visible = True
            else:
                line.visible = False

        # Create buttons
        next_btn = Button(label='Next')
        prev_btn = Button(label='Prev')
        done_btn = Button(label='Done')
        # Button callbacks
        def next_cb():
            self._curr_plot += 1
            print('next: {}'.format(self._curr_plot))
        def prev_cb():
            self._curr_plot -= 1
            print('prev: {}'.format(self._curr_plot))
        def done_cb():
            self._server.stop()
            self._server.unlisten()
            self._io_loop.stop()
        # Attach callbacks to buttons
        next_btn.on_click(next_cb)
        prev_btn.on_click(prev_cb)
        done_btn.on_click(done_cb)

        # Add elements to doc
        doc.add_root(column(plot, next_btn, prev_btn, done_btn))


    def _run(self):
        self._bokeh_app = Application(FunctionHandler(self._modify_doc))
        self._server = Server({'/': self._bokeh_app},
                              io_loop=self._io_loop)
        for k in self._server.__dict__:
            print("{}: {}".format(k, self._server.__dict__[k]))
        self._server.start()
        self._io_loop.add_callback(self._server.show, '/')
        self._io_loop.start()


    def run(self, dataset):
        self._build_source(dataset=dataset)
        self._run()


if __name__ == '__main__':
    x = np.linspace(1, 10, 500)
    ones = np.ones(x.shape)
    ys = [m*ones for m in np.linspace(-1, 1, 50)]
    # m = np.linspace(1, 10, 1000)
    # print(m)
    # ys = [np.log(x), np.sin(x), np.cos(x), -np.log(x), 2*x + 1, 10 - x]
    dataset = {}
    for i, y in enumerate(ys):
        key = 'meas{}'.format(i + 1)
        dataset[key] = np.column_stack((x, y))
    print(len(ys))
    print(len(dataset))

    viewer = ViewerSpectrum()
    viewer.run(dataset=dataset)

    viewer1 = ViewerSpectrum()
    viewer1.run(dataset=dataset)
