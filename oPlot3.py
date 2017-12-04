#! /usr/bin/env python

""" oPlot3.py -- Displays Oscilloscope Output
Run this program by typing: python oPlot3.py
This program will cause gnuplot to display in real time,
which might be more helpful for a MicrOscope user than
looking at the web page, which is limited by how fast it
can refresh the page.
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
	databuff = Gnuplot.File("values.data",using='1:2')
	g.plot(databuff)
	# gnuplot commands to cause plot to scroll, continually adding data
	# try compiling, the syntax might be a tad off
	g('pause 1')
	g('reread')
	raw_input('Please press return to continue....\n')



#when executed, just run demo():
if __name__ == '__main__':
	display()