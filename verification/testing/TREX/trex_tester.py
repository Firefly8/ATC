#!/usr/bin/env python

#===============================================================================
# imports
#===============================================================================
# system  
import os
import inspect
import sys
import abc
from netaddr import *
from multiprocessing import Process, Queue 
# pythons modules 
# local
currentdir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
parentdir = os.path.dirname(currentdir)
sys.path.insert(0,parentdir) 

from trex_infra_utils import *
from tester import Tester

#===============================================================================
# Test Globals
#===============================================================================
class TrexTester(Tester):
    
    __metaclass__  = abc.ABCMeta
    
    def __init__(self):        
        self.config = {'setup_num'       : None,
                       'modify_run_cpus' : True,
                       'use_4k_cpus'     : False,
                       'install_package' : True,
                       'install_file'    : None,
                       'copy_binaries'   : True,
                       'use_director'    : False,
                       'start_ezbox'     : False,
                       'stop_ezbox'      : False}
        
        self.test_resources = TrexPlayers()
        self.test_result = TrexTestResult()
        
    @abc.abstractmethod
    def user_init(self):
        """User initialization - must be implemented"""
        pass
    
    @abc.abstractmethod
    def user_create_yaml(self):
        """yaml creation - must be implemented"""
        pass
    
    def clean_all_players(self):
        self.test_resources.clean_players(True, self.config['stop_ezbox'])
    
    def run_user_test(self):
        process_list = []
        trex_test_result_list = []
        #Use a synchronized queue class to share a variable between threads
        trex_test_result_queue = Queue()
        
        for trex in self.test_resources.trex_hosts:
            process_list.append(Process(target=trex.run_trex_test, args=(trex_test_result_queue,)))
            
        for p in process_list:
            p.start()    
                            
        for p in process_list:
            trex_test_result_list.append(trex_test_result_queue.get())  
                      
        for p in process_list:
            p.join()   
                    
        self.test_result.set_result(trex_test_result_list)    

    def general_trex_checker(self):
        self.test_result.general_checker()
        
    def get_test_rc(self):
        return 0
    
    def start_test(self):
        print "FUNCTION " + sys._getframe().f_code.co_name + " called"
        self.user_init()            
        self.user_create_yaml()            
        self.run_user_test()       
        self.general_trex_checker()
