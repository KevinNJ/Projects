# -*- coding: utf-8 -*-

from __future__ import division

import math
import random

import matplotlib.pyplot as plt
import scipy.signal as sig

from itertools import product
from misc import common_part_values, metric_prefix
from anneal import Annealer

# Setup optimization targets
target_q = 0.707    # 1/sqrt(2) - Which is the maximally flat response
target_freq = 500   # Hz
target_atten = -40  # dB

rvalues, cvalues = common_part_values()


def f0(system):
    """Return the natural frequency of the system."""
    c1,c2,r1,r2 = system
    fn = 1 / (2 * math.pi * math.sqrt(r1 * c1 * r2 * c2))
    return fn


def q(system):
    """Return the Q Value of the system."""
    c1,c2,r1,r2 = system
    q  = math.sqrt(c1 * c2 * r1 * r2)/ (c2 * (r1 + r2))
    return q

    
def frf(system):
    """Return the Frequency Response Function of the system.
    
    Returns a function which takes a frequency as an argument.
    This function when evaluated at any frequency returns the complex
    frequency response at that frequency.
    
    Example: frf(system)(10) returns the complex FRF at 10 Hz
    """
    def internal(f):
        c1,c2,r1,r2 = system
        w = 2 * math.pi * f
        num = 1 / (c1*c2*r1*r2)
        den = 1 / (c1*c2*r1*r2) + (r1+r2)/(c1*r1*r2) * 1j*w - w**2
        return num/den
    
    return internal

    
def dB(x):
    """Returns the argument in decibels"""
    return 20 * math.log10(abs(x))

    
def stepResponse(system):
    """Computes the step response for a given system"""
    c1,c2,r1,r2 = system
    num = 1 / (c1*c2*r1*r2)
    den = (1, (r1+r2)/(c1*r1*r2), 1/(c1*c2*r1*r2))
    return sig.step((num,den))

    
def energy(system):
    """Computes the energy of a given system.
    
    The energy is defined as decreasing towards zero as the system
    approaches an ideal system.
    """
    
    frf_ = frf(system)
    f0_ = f0(system)
    q_ = q(system)
    c1,c2,r1,r2 = system
    
    e = 0
    e += abs(target_atten - dB(frf_(target_freq))) / abs(target_atten) # percent error off frequency @ attenuation
    e += abs(target_q - q_) / abs(target_q)                            # percent error off ideal Q value
    e += abs(c1-c2) / abs((c1+c2)/2) * 0.1                             # percent difference in capacitor values
    e += abs(r1-r2) / abs((r1+r2)/2) * 0.1                             # percent difference in resistor values

    return e


def move(system):
    """ Changes the system randomly
    
    This function makes a random change to one of the component values
    in the system.
    """
    component = random.randrange(0, 4)
    
    if component == 0:
        index = random.randrange(0, len(cvalues))
        system[0] = cvalues[index]
    elif component == 1:
        index = random.randrange(0, len(cvalues))
        system[1] = cvalues[index]
    elif component == 2:
        index = random.randrange(0, len(rvalues))
        system[2] = rvalues[index]
    elif component == 3:
        index = random.randrange(0, len(rvalues))
        system[3] = rvalues[index]


if __name__ == '__main__':

    # set up simulated annealing algorithm
    units=('F', 'F', u'Ω', u'Ω')   # units of the values in the system
    initial_system = [cvalues[0], cvalues[0], rvalues[0], rvalues[0]]
    annealer = Annealer(energy, move)
    schedule = annealer.auto(initial_system, minutes=0.1)
    
    # run simulated annealing algorithm and compute properties of the final system
    final_system, error = annealer.anneal(initial_system, schedule['tmax'], schedule['tmin'], schedule['steps'], updates=100)
    final_frf = frf(final_system)
    final_f0 = f0(final_system)
    final_q  = q(final_system)
    final_vals = [metric_prefix(*s) for s in zip(final_system, units)]
    
    print 'Soln: (%s), Remaining Energy: %s' % (', '.join(final_vals), error)

    # calculate data for graphs
    freqs = range(1000000)    # response from 0 Hz to 1 MHz
    response = [dB(final_frf(f)) for f in freqs]
    natural = final_f0, dB(final_frf(final_f0))
    target = target_freq, dB(final_frf(target_freq))
    step_freqs, step_response = stepResponse(final_system)

    
    plt.figure()

    # bode response plot
    ax = plt.subplot(2,1,1)
    plt.semilogx(freqs,response)
    plt.semilogx(natural[0], natural[1], 'r+', ms=10)
    plt.annotate('Natural Freq: (%.2f Hz, %.2f dB) ' % natural, xy=natural, xytext=(10,10), textcoords='offset points')
    plt.semilogx(target[0], target[1], 'r+', ms=10)
    plt.annotate('target attenuation: (%.2f Hz, %.2f dB)'%target, xy=target, xytext=(10,10), textcoords='offset points')
    plt.title('Bode Plot (F0: %.2f Hz, Q-Factor: %.2f)\n' % (final_f0, final_q) + 'Soln: (%s)' % ', '.join(final_vals))
    plt.xlabel('Frequency [Hz]')
    plt.ylabel('Gain [dB]')
    
    lims=list(ax.get_ylim())
    lims[1]=20
    plt.ylim(lims)

    # step response plot
    plt.subplot(2,1,2)
    plt.plot(step_freqs, step_response)
    plt.title('Step Response')
    plt.xlabel('Time (s)')
    plt.ylabel('Response (v)')
    
    plt.show()


"""
References:
[1] http://en.wikipedia.org/wiki/Sallen%E2%80%93Key_topology
[2] http://en.wikipedia.org/wiki/Q_factor
[3] http://sim.okawa-denshi.jp/en/OPstool.php
[4] http://www.falstad.com/circuit/
"""

