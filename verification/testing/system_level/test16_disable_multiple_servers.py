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
from alvs_tester_class import ALVS_Tester


# pythons modules 
# local
currentdir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
parentdir = os.path.dirname(currentdir)
sys.path.insert(0,parentdir) 
from alvs_infra import *
from alvs_players_factory import *

#===============================================================================
# Test Globals
#===============================================================================
request_count = 200 
server_count = 4
client_count = 10
service_count = 2



#===============================================================================
# User Area function needed by infrastructure
#===============================================================================
class Test16(ALVS_Tester):
	
	def user_init(self, setup_num):
		print "FUNCTION " + sys._getframe().f_code.co_name + " called"
		
		self.test_resources = ALVS_Players_Factory.generic_init(setup_num, service_count, server_count, client_count)
		
		i = 0
		for s in self.test_resources['server_list']:
			s.vip = self.test_resources['vip_list'][i%service_count]
			i += 1
			
	
	def client_execution(self, client, vip):
		client.exec_params += " -i %s -r %d -e True" %(vip, request_count)
		client.execute()
	
	def run_user_test(self):
		print "FUNCTION " + sys._getframe().f_code.co_name + " called"
		process_list = []
		port = '80'
		ezbox = self.test_resources['ezbox']
		server_list = self.test_resources['server_list']
		client_list = self.test_resources['client_list']
		vip_list = self.test_resources['vip_list']
	
		#service scheduling algorithm is SH with port
		for i in range(service_count):
			ezbox.add_service(vip_list[i], port)
		for server in server_list:
			ezbox.add_server(server.vip, port, server.ip, port)
			server.set_large_index_html()
			
		
		for index, client in enumerate(client_list):
			process_list.append(Process(target=self.client_execution, args=(client,vip_list[index%service_count],)))
		for p in process_list:
			p.start()
	
		for i in range(20):		
			time.sleep(2) 
			# Disable server - director will remove server from IPVS
			print 'remove test.html'
			server_list[0].delete_test_html()
			server_list[1].delete_test_html()
			time.sleep(10) 
			print 're-add test.html'
			server_list[0].set_test_html()
			server_list[1].set_test_html()
		
		for p in process_list:
			p.join()
		
			
		print 'End user test'
	
	def run_user_checker(self, log_dir):
		print "FUNCTION " + sys._getframe().f_code.co_name + " called"
		expected_servers = {}
		for index, client in enumerate(self.test_resources['client_list']):
			client_expected_servers=[s.ip for s in self.test_resources['server_list'] if s.vip == self.test_resources['vip_list'][index%service_count]]
			client_expected_servers.append('Connection closed ERROR')
			expected_servers[client.ip] = client_expected_servers
		expected_dict = {'client_response_count':request_count,
							'client_count': client_count,
							'no_connection_closed': False,
							'expected_servers_per_client':expected_servers}
		
		return client_checker(log_dir, expected_dict)
	
current_test = Test16()
current_test.main()