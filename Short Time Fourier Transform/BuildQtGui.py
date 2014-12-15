__author__ = 'knelson'

def make_gui(filename):
    """Makes a .py file from a .ui file.

    Assumes that the pyside-uic script is installed in the local Anaconda directory
    """

    from subprocess import check_call
    import os

    cmd = os.path.normpath('C:\Anaconda\Scripts\pyside-uic')
    infile = filename + '.ui'
    outfile = filename + '.py'

    call_args = [cmd, infile, '-o', outfile]
    print call_args

    check_call(call_args)