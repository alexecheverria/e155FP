#! /usr/bin/env python

""" oPlot.py -- Displays Oscilloscope Output

Run this program by typing python oPlot2.py

"""

from numpy import *
import Gnuplot, Gnuplot.funcutils
import commands


def setupScopeScreen(g):

	g.title('MicrOscope')
	g.xlabel('Current Time Scale')
	g.ylabel('Current Volt Scale')
	return g

def display():
	"""Demonstrate the Gnuplot package."""

	yAxisBound = commands.getstatusoutput('sudo ./readVoltsScale')

	#Create a plot of current Oscilloscope Output
	g = Gnuplot.Gnuplot(debug=1)
	#Setup title, axis labels etc.
	g.title('MicrOscope')
	g('set style data linespoints')
	g('set grid')
	databuff = Gnuplot.File("values.data",using='1:2')
	g.plot(databuff)

	#Setting the volts/div
	if(yAxisBound[1] == '13'):
		g('set yrange ["0":"5"]')
	elif(yAxisBound[1] == '19'):
		g('set yrange ["0":"2.5"]')
	elif(yAxisBound[1] =='26'):
		g('set yrange ["2.5":"5"]')
	else:
		g('set yrange ["0":"5"]')

	g.hardcopy(filename="micrOscopeOutput.png",terminal="png")
#	raw_input('Please press return to continue....\n')

#when executed, just run demo():
if __name__ == '__main__':
	display()
