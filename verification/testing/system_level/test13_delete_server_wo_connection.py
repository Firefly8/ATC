#!/usr/bin/env python


#===============================================================================
# imports
#===============================================================================

# system  
import cmd
from collections import namedtuple
import logging
import os
import sys
import inspect
from multiprocessing import Process



# pythons modules 
# local
currentdir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
parentdir = os.path.dirname(currentdir)
sys.path.insert(0,parentdir) 
from e2e_infra import *


#===============================================================================
# Test Globals
#===============================================================================
request_count = 1000
server_count = 2
client_count = 1
service_count = 1



#===============================================================================
# User Area function needed by infrastructure
#===============================================================================

def user_init(setup_num):
    print "FUNCTION " + sys._getframe().f_code.co_name + " called"
    
    dict = generic_init(setup_num, service_count, server_count, client_count)
    
    w = 1
    for s in dict['server_list']:
        s.vip = dict['vip_list'][0]
        s.weight = w
        w *= 4
        
    return convert_generic_init_to_user_format(dict)
    
def client_execution(client, vip):
    client.exec_params += " -i %s -r %d " %(vip, request_count)
    client.execute()
    
def run_user_test(server_list, ezbox, client_list, vip_list):
    print "FUNCTION " + sys._getframe().f_code.co_name + " called"
    process_list = []
    port = '80'
    vip = vip_list[0]
    
    # Step 1 - delete + add server
    ezbox.add_service(vip, port)
    for server in server_list:
        ezbox.add_server(server.vip, port, server.ip, port, server.weight)
     
    
    for client in client_list:
        process_list.append(Process(target=client_execution, args=(client,vip,)))    
    for p in process_list:
        p.start()
    
    time.sleep(1)
    
    print 'remove server[0]'
    ezbox.delete_server(server_list[0].vip, port, server_list[0].ip, port)
    
    time.sleep(1)
    
    print 'add server[0]'
    ezbox.add_server(server_list[0].vip, port, server_list[0].ip, port, server_list[0].weight)
    
    for p in process_list:
        p.join()
    
    # Step 2 - regular run without manipulations after changes in Step 1
    process_list = []
    for client in client_list:
        new_log_name = client.logfile_name+'_1'
        client.add_log(new_log_name) 
        process_list.append(Process(target=client_execution, args=(client,vip,)))
    for p in process_list:
        p.start()
    for p in process_list:
        p.join()    

        
    print 'End user test'
    

    
def run_user_checker(server_list, ezbox, client_list, log_dir, vip_list):
    print "FUNCTION " + sys._getframe().f_code.co_name + " called"
    
    sd = 0.01
    expected_dict = {}
    expected_dict[0] = {'client_response_count':request_count,
                        'client_count': len(client_list),
                        'no_connection_closed': True,
                        'no_404': True,
                        'expected_servers':server_list}
    
    expected_dict[1] = {'client_response_count':request_count,
                        'client_count': len(client_list),
                        'no_connection_closed': True,
                        'no_404': True,
                        'expected_servers':server_list,
                        'check_distribution':(server_list,vip_list,sd)}
    
    return client_checker(log_dir, expected_dict, 2)
    
    
#===============================================================================
# main function
#===============================================================================    
def main():
    print "FUNCTION " + sys._getframe().f_code.co_name + " called"
    
    config = generic_main()
    
    server_list, ezbox, client_list, vip_list = user_init(config['setup_num'])
    
    init_players(server_list, ezbox, client_list, vip_list, config)
    
    run_user_test(server_list, ezbox, client_list, vip_list)
    
    log_dir = collect_logs(server_list, ezbox, client_list)

    gen_rc = general_checker(server_list, ezbox, client_list)
    
    clean_players(server_list, ezbox, client_list, True, config['stop_ezbox'])
    
    user_rc = run_user_checker(server_list, ezbox, client_list, log_dir,vip_list)
    
    if user_rc and gen_rc:
        print 'Test passed !!!'
        exit(0)
    else:
        print 'Test failed !!!'
        exit(1)

main()    
    