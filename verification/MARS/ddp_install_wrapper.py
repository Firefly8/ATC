#!/usr/bin/env python

# Built-in modules
import sys
import os

# Local modules
from reg2_wrapper.test_wrapper.standalone_wrapper import StandaloneWrapper

class ddp_install_wrapper(StandaloneWrapper):

    def get_prog_path(self):
        prog = "python2.7 ddp_install.py"
        return prog

    def configure_parser(self):
        super(ddp_install_wrapper, self).configure_parser()
        self.add_cmd_argument('-f',  help='Installation file name ')
        self.add_cmd_argument('-c',  help='number of CPUs')
        self.add_test_attribute_argument('--topo_file', 'topo_file', separator=' ') 

if __name__ == "__main__":
    ddp_wrapper = ddp_install_wrapper("ddp_install")
    ddp_wrapper.execute(sys.argv[1:])
