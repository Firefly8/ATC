/* Copyright (c) 2016 Mellanox Technologies, Ltd. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
* 3. Neither the names of the copyright holders nor the names of its
*    contributors may be used to endorse or promote products derived from
*    this software without specific prior written permission.
*
* Alternatively, this software may be distributed under the terms of the
* GNU General Public License ("GPL") version 2 as published by the Free
* Software Foundation.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*
*  Project:             NPS400 ALVS application
*  File:                infrastructure_utils.h
*  Desc:                Infrastructure utils API.
*
*/

#ifndef _INFRASTRUCTURE_UTILS_H_
#define _INFRASTRUCTURE_UTILS_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <net/ethernet.h>
#include "ezdp_memory_defs.h"


/*! Required parameters for hash creation data structure  */
struct infra_hash_params {
	uint32_t key_size;
	uint32_t result_size;
	uint32_t max_num_of_entries;
	uint32_t hash_size;
	bool single_cycle;
	bool updated_from_dp;
	uint32_t sig_pool_id;
	uint32_t result_pool_id;
	bool is_external;
	uint32_t main_table_search_mem_heap;
	uint32_t sig_table_search_mem_heap;
	uint32_t res_table_search_mem_heap;

};

/*! Required parameters for table creation data structure  */
struct infra_table_params {
	uint32_t key_size;
	uint32_t result_size;
	uint32_t max_num_of_entries;
	bool updated_from_dp;
	bool is_external;
	uint32_t search_mem_heap;
};

/*! Required parameters for tcam creation data structure  */
struct infra_tcam_params {
	uint32_t side;
	uint32_t profile;
	uint32_t lookup_table_count;
	uint32_t table;
	uint32_t key_size;
	uint32_t result_size;
	uint32_t max_num_of_entries;
};


/* RTC channel ID */
#define REAL_TIME_CLOCK_CHANNEL_ID	0

/*! Required parameters for get RTC  */
struct read_real_time_clock_result {
	uint32_t         nano_seconds;
	/* The nanoseconds value */

	uint32_t         seconds;
	/* The seconds value */
};


/**************************************************************************//**
 * \brief       Get my MAC
 *
 * \param[out]  my_mac - reference to ethernet address type
 *
 * \return      true - success
 *              false - can't find tap interface file
 */
bool infra_get_my_mac(struct ether_addr *my_mac);

/**************************************************************************//**
 * \brief       Create TCAM data structure
 *
 * \param[in]   params          - parameters of the tcam
 *
 * \return      bool - success or failure
 */
bool infra_create_tcam(struct infra_tcam_params *params);

/**************************************************************************//**
 * \brief       Create hash data structure
 *
 * \param[in]   struct_id       - structure id of the hash
 * \param[in]   params          - parameters of the hash (size of key & result,
 *                                max number of entries and update mode)
 *
 * \return      bool - success or failure
 */
bool infra_create_hash(uint32_t struct_id,
		       struct infra_hash_params *params);

/**************************************************************************//**
 * \brief       Create table data structure
 *
 * \param[in]   struct_id       - structure id of the table
 * \param[in]   params          - parameters of the table (size of key & result
 *                                and max number of entries)
 *
 * \return      bool - success or failure
 */
bool infra_create_table(uint32_t struct_id,
			struct infra_table_params *params);

/**************************************************************************//**
 * \brief       Add an entry to a data structure
 *
 * \param[in]   struct_id       - structure id of the search structure
 * \param[in]   key             - reference to key
 * \param[in]   key_size        - size of the key in bytes
 * \param[in]   result          - reference to result
 * \param[in]   result_size     - size of the result in bytes
 *
 * \return      bool - success or failure
 */
bool infra_add_entry(uint32_t struct_id, void *key, uint32_t key_size,
		     void *result, uint32_t result_size);

/**************************************************************************//**
 * \brief       Modify an entry in a data structure
 *
 * \param[in]   struct_id       - structure id of the search structure
 * \param[in]   key             - reference to key
 * \param[in]   key_size        - size of the key in bytes
 * \param[in]   result          - reference to result
 * \param[in]   result_size     - size of the result in bytes
 *
 * \return      bool - success or failure
 */
bool infra_modify_entry(uint32_t struct_id, void *key, uint32_t key_size,
			void *result, uint32_t result_size);

/**************************************************************************//**
 * \brief       Delete an entry from a data structure
 *
 * \param[in]   struct_id       - structure id of the search structure
 * \param[in]   key             - reference to key
 * \param[in]   key_size        - size of the key in bytes
 *
 * \return      bool - success or failure
 */
bool infra_delete_entry(uint32_t struct_id, void *key, uint32_t key_size);

/**************************************************************************//**
 * \brief       Get an entry from data structure
 *
 * \param[in]   struct_id       - structure id of the search structure
 * \param[in]   key             - reference to key
 * \param[in]   key_size        - size of the key in bytes
 * \param[in]   result          - reference to result
 * \param[in]   result_size     - size of the result in bytes
 *
 * \return      bool - success or failure
 */
bool infra_get_entry(uint32_t struct_id, void *key, uint32_t key_size, void *result, uint32_t result_size);

/**************************************************************************//**
 * \brief       Add an entry to a TCAM data structure
 *
 * \param[in]   side            - side of TCAM table (0/1)
 * \param[in]   table           - table number
 * \param[in]   key             - reference to key
 * \param[in]   key_size        - size of the key in bytes
 * \param[in]   mask            - reference to mask
 * \param[in]   index           - index in table
 * \param[in]   result          - reference to result
 * \param[in]   result_size     - size of the result in bytes
 *
 * \return      bool - success or failure
 */
bool infra_add_tcam_entry(uint32_t side, uint32_t table, void *key, uint32_t key_size,
			  void *mask, uint32_t index, void *result, uint32_t result_size);

/**************************************************************************//**
 * \brief       Delete an entry from a TCAM data structure
 *
 * \param[in]   side            - side of TCAM table (0/1)
 * \param[in]   table           - table number
 * \param[in]   key             - reference to key
 * \param[in]   key_size        - size of the key in bytes
 * \param[in]   index           - index in table
 *
 * \return      bool - success or failure
 */
bool infra_delete_tcam_entry(uint32_t side, uint32_t table, void *key, uint32_t key_size, uint32_t index);

/**************************************************************************//**
 * \brief       Delete all entries from a data structure
 *
 * \param[in]   struct_id       - structure id of the search structure
 *
 * \return      bool - success or failure
 */
bool infra_delete_all_entries(uint32_t struct_id);

/**************************************************************************//**
 * \brief       Read Double Counters Values
 * \param[in]   counter_index   - index of starting counter
 *		num_of_counters - number of counters from the starting counter
 *		[out] counters_value - pointer to the array of results (array of uint64 size must be num_of_couinters)
 * \return      bool
 *
 */
bool infra_get_double_counters(uint32_t counter_index,
			       uint32_t num_of_counters,
			       uint64_t *frames_value,
			       uint64_t *bytes_value);

/**************************************************************************//**
 * \brief       write Long Counters Values
 * \param[in]   counter_index   - index of starting counter
 *		num_of_counters - number of counters from the starting counter
 *		value_lsb - lsb value of the long counters
 *		value_msb - msb value of the long counters
 * \return      bool
 *
 */
bool infra_set_long_counters(uint32_t counter_index,
			     uint32_t num_of_counters,
			     uint32_t value_lsb,
			     uint32_t value_msb);

/**************************************************************************//**
 * \brief       Read Long Counters Values, read several counters (num_of_counters)
 * \param[in]   counter_index   - index of starting counter
 *		num_of_counters - number of counters from the starting counter
 *		[out] counters_value - pointer to the array of results (array of uint64 size must be num_of_couinters)
 * \return      bool
 *
 */
bool infra_get_long_counters(uint32_t counter_index,
			     uint32_t num_of_counters,
			     uint64_t *counters_value);

/**************************************************************************//**
 * \brief       Get posted counters value, read several counters (num_of_counters)
 *
 * \param[in]   counter_index   - index of starting counter
 *		num_of_counters - number of counters from the starting counter
 *		[out] counters_value - pointer to the array of results (array of uint64 size must be num_of_couinters)
 * \return      bool
 */
bool infra_get_posted_counters(uint32_t counter_index,
			       uint32_t num_of_counters,
			       uint64_t *counters_value);

/**************************************************************************//**
 * \brief       Set posted counters values - set to a several counters (num_of_counters)
 *
 * \param[in]   counter_index   - index of starting counter
 *		num_of_counters - number of counters from the starting counter
 * \return      bool
 */
bool infra_clear_posted_counters(uint32_t counter_index,
				 uint32_t num_of_counters);

/**************************************************************************//**
 * \brief       Set long counters values - set to a several counters (num_of_counters)
 *
 * \param[in]   counter_index   - index of starting counter
 *		num_of_counters - number of counters from the starting counter
 * \return      bool
 */
bool infra_clear_long_counters(uint32_t counter_index,
			       uint32_t num_of_counters);

/**************************************************************************//**
 * \brief       Clear double counters values - set to a several counters (num_of_counters)
 *
 * \param[in]   counter_index   - index of starting counter
 *		num_of_counters - number of counters from the starting counter
 * \return      bool
 */
bool infra_clear_double_counters(uint32_t counter_index,
				 uint32_t num_of_counters);

/**************************************************************************//**
 * \brief       Copy data to memory (IMEM/EMEM)
 *
 * \param[in]   addr         - extended address of the memory to copy data to
 *		data         - the data to copy to memory
 *		data_size    - size of the data (in bytes)
 *
 * \return      bool
 */
bool infra_set_memory(struct ezdp_ext_addr *addr,
		      void *data,
		      uint32_t data_size);

/**************************************************************************//**
 * \brief       Read real time clock from NPS
 *
 * \param[out]   rtc_result  - RTC read result
 *
 * \return      bool - success or failure
 */
bool infra_read_real_time_clock(struct read_real_time_clock_result  *rtc_result);

/**************************************************************************//**
 * \brief       Read real time clock from NPS (seconds only)
 *
 * \param[out]  uint32_t seconds - NPS real time clock (seconds)
 *
 * \return      bool - success or failure
 */
bool infra_read_real_time_clock_seconds(uint32_t *seconds);

#endif /* _INFRASTRUCTURE_UTILS_H_ */
