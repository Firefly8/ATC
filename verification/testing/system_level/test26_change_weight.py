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
import copy



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
server_count = 5
client_count = 1
service_count = 1



#===============================================================================
# User Area function needed by infrastructure
#===============================================================================

def user_init(setup_num):
	print "FUNCTION " + sys._getframe().f_code.co_name + " called"

	vip_list = [get_setup_vip(setup_num,i) for i in range(service_count)]

	index = 0
	setup_list = get_setup_list(setup_num)

	server_list=[]
	for i in range(server_count):
		server_list.append(HttpServer(ip = setup_list[index]['ip'],
						  hostname = setup_list[index]['hostname'], 
						  username = "root", 
						  password = "3tango", 
						  vip = vip_list[0],
						  eth='ens6',
						  weight=1))
		index+=1
	
	script_dirname = os.path.dirname(os.path.realpath(__file__))
	client_list=[]
	for i in range(client_count):
		client_list.append(HttpClient(ip = setup_list[index]['ip'],
						  hostname = setup_list[index]['hostname'], 
						  username = "root", 
						  password = "3tango",
						  exe_path= script_dirname,
						  exe_script  = "basic_client_requests.py",
						  exec_params = ""))
		index+=1
	

	# EZbox
	ezbox = ezbox_host(setup_num)
	
	return (server_list, ezbox, client_list, vip_list)

def client_execution(client, vip):
	client.exec_params += " -i %s -r %d" %(vip, request_count)
	client.execute()

def run_user_test(server_list, ezbox, client_list, vip_list):
	print "FUNCTION " + sys._getframe().f_code.co_name + " called"
	process_list = []
	port = '80'
	vip = vip_list[0]
	
	ezbox.add_service(vip, port)
	for server in server_list:
		ezbox.add_server(server.vip, port, server.ip, port)
	
	time.sleep(5)
	for client in client_list:
		process_list.append(Process(target=client_execution, args=(client,vip,)))
	for p in process_list:
		p.start()
	for p in process_list:
		p.join()
	
	print 'change weight of servers: %s & %s' %(server_list[0].ip , server_list[1].ip)
	
	new_weight = 2
	server_list[0].weight = new_weight
	ezbox.modify_server(vip, port, server_list[0].ip, port, weight=new_weight)
	server_list[1].weight = new_weight
	ezbox.modify_server(vip, port, server_list[1].ip, port, weight=new_weight)
	
	process_list = []
	time.sleep(5)
	for client in client_list:
		new_log_name = client.logfile_name+'_1'
		client.add_log(new_log_name) 
		process_list.append(Process(target=client_execution, args=(client,vip,)))
	for p in process_list:
		p.start()
	for p in process_list:
		p.join()
	
	print 'End user test'

def run_user_checker(server_list, ezbox, client_list, log_dir,vip_list):
	print "FUNCTION " + sys._getframe().f_code.co_name + " called"
	expected_dict = {}
	old_server_list = copy.deepcopy(server_list)
	old_server_list[0].weight = 1
	old_server_list[1].weight = 1
	sd = 0.02
	expected_dict[0] = {'client_response_count':request_count,
						'client_count': len(client_list),
						'no_connection_closed': True,
						'no_404': True,
						'expected_servers':old_server_list,
						'check_distribution':(old_server_list,vip_list,sd)}
	expected_dict[1] = {'client_response_count':request_count,
						'client_count': len(client_list),
						'no_connection_closed': True,
						'no_404': True,
						'expected_servers':server_list,
						'check_distribution':(server_list,vip_list,sd)}
	
	return client_checker(log_dir, expected_dict,2)

#===============================================================================
# main function
#===============================================================================
def main():
	print "FUNCTION " + sys._getframe().f_code.co_name + " called"
	if len(sys.argv) != 2:
		print "script expects exactly 1 input argument"
		print "Usage: client_requests.py <setup_num>"
		exit(1)
	
	setup_num  = int(sys.argv[1])
	
	server_list, ezbox, client_list, vip_list = user_init(setup_num)
	
	init_players(server_list, ezbox, client_list, vip_list, use_director=True, use_4k_cpus=False)
	
	run_user_test(server_list, ezbox, client_list, vip_list)
	
	log_dir = collect_logs(server_list, ezbox, client_list)

	gen_rc = general_checker(server_list, ezbox, client_list)
	
	clean_players(server_list, ezbox, client_list, use_director=True)
	
	user_rc = run_user_checker(server_list, ezbox, client_list, log_dir,vip_list)
	
	if user_rc and gen_rc:
		print 'Test passed !!!'
		exit(0)
	else:
		print 'Test failed !!!'
		exit(1)

main()
