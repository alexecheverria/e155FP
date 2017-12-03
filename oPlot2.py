#! /usr/bin/env python

""" oPlot.py -- Displays Oscilloscope Output

Run this program by typing python oPlot2.py

"""

from numpy import *
import Gnuplot, Gnuplot.funcutils


def setupScopeScreen(g):

	g.title('MicrOscope')
	g.xlabel('Current Time Scale')
	g.ylabel('Current Volt Scale')
	return g

def display():
	"""Demonstrate the Gnuplot package."""

	#Create a plot of current Oscilloscope Output
	g = Gnuplot.Gnuplot(debug=1)
	#Setup title, axis labels etc.
	#g = setupScopeScreen(g)
	g.title('MicrOscope')
	g('set data style linespoints')
#	g.plot([[0,0],[1,1],[2,1],[3,1],[1.5,1],[2.5,1]])
	databuff = Gnuplot.File("values.data",using='1:2')
	g.plot(databuff)
	g.hardcopy(filename="micrOscopeOutput.png",terminal="png")
	raw_input('Please press return to continue....\n')



#when executed, just run demo():
if __name__ == '__main__':
	display()
