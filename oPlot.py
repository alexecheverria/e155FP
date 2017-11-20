#! /usr/bin/env python

""" oPlot.py -- Displays Osciolloscope Output

Run this demo by typing 'python oPlot.py'.

"""

from numpy import *
import Gnuplot, Gnuplot.funcutils


def setupScopeScreen(g):

    g.title('MicrOscope')
    g.xlabel('Current Time Scale')
    g.ylabel('Current Volt Scale')

def display():
    """Demonstrate the Gnuplot package."""

    #Create a plot of current Osciolloscope Output
    g = Gnuplot.Gnuplot(debug=1)
    #Setup title, axis labels etc.
    setupScopeScreen(g)

    g.plot([[0,0],[1,1],[2,2],[3,3]])





# when executed, just run demo():
if __name__ == '__main__':
    display()

