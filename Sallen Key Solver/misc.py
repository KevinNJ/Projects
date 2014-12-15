from itertools import product

def common_part_values():
    """ Returns a list of common capacitor and resistor values.  
    
    Resistor values are in ohms (1 ohm to 9.1 MOhm)
    Capacitor values are in farads (1e-11 F to 8.2e-7 F) = (10 pF to 820 nF)
    """
    rexponent = [1, 10, 100, 1000, 10000, 100000]
    rmantissa = [1.0, 1.1, 1.2, 1.3, 1.5, 1.6, 1.8, 2.0, 2.2, 2.4, 2.7, 3.0, 3.3, 3.6, 3.9, 4.3, 4.7, 5.1, 5.6, 6.2, 6.8, 7.5, 8.2, 9.1]
    rvalues = [x[0]*x[1] for x in product(rexponent, rmantissa)]

    cexponent = [x * 1e-12 for x in [1, 10, 100, 1000, 1000, 10000]]
    cmantissa = [10, 12, 15, 18, 22, 27, 33, 39, 47, 56, 68, 82]
    cvalues = [x[0]*x[1] for x in product(cexponent, cmantissa)]
    
    return rvalues,cvalues
    
def frexp_10(decimal):
    "Splits a float into it's base 10 mantissa and exponent"
    parts = ("%e" % decimal).split('e')
    return float(parts[0]), int(parts[1])

def metric_prefix(decimal, unit):
    """ Transforms a decimal value into a metric prefix
    
    E.G (1e-8 F) => (10 nF)
    """
    mantissa, power = frexp_10(decimal)
    prefixes = {-12:'p', -9:'n', -6:'u', -3:'m', 0:'', 3:'k', 6:'M'}
    for p in prefixes.iteritems():
        if p[0] <= power < p[0]+3:
            if p[0] == 0:
                # no prefix, between 0.1 and 10
                return '%s %s' % (mantissa * 10**(power-p[0]), unit)
            else:
                # has a prefix
                return '%s %s%s' % (mantissa * 10**(power-p[0]), p[1], unit)