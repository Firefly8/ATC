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
*  File:                alvs_aging.h
*  Desc:                handle aging functionality
*/

#ifndef ALVS_AGING_H_
#define ALVS_AGING_H_

#include "alvs_conn.h"
#include "alvs_defs.h"

/******************************************************************************
 * \brief         perform aging on connection entries
 * \return        void
 */
static __always_inline
void alvs_handle_aging_event(uint32_t event_id)
{
	uint32_t conn_index;
	uint32_t last_conn_index;
	uint32_t iteration_num = event_id >> LOG2(ALVS_AGING_TIMER_EVENTS_PER_ITERATION);

	event_id &= ALVS_AGING_TIMER_EVENT_ID_MASK;
	last_conn_index = (event_id + 1) << LOG2(ALVS_AGING_TIMER_SCAN_ENTRIES_PER_JOB);

	/*scan ALVS_AGING_TIMER_SCAN_ENTRIES_PER_JOB entries in connection DB and check if their aging bit is set*/
	for (conn_index = event_id << LOG2(ALVS_AGING_TIMER_SCAN_ENTRIES_PER_JOB);
		conn_index < last_conn_index;
		conn_index++) {
		if (alvs_conn_info_lookup(conn_index) == 0) {
			if (cmem_alvs.conn_info_result.delete_bit == 1) {
				alvs_conn_delete(conn_index);
				return;
			}

			if (cmem_alvs.conn_info_result.aging_bit == 1) {
				alvs_conn_age_out(conn_index, ezdp_mod(iteration_num, cmem_alvs.conn_info_result.conn_state, 0, 0));
				return;
			}

			if (cmem_alvs.conn_info_result.age_iteration == ezdp_mod(iteration_num, cmem_alvs.conn_info_result.conn_state, 0, 0) &&
				cmem_alvs.conn_info_result.aging_bit == 0) {
				alvs_conn_delete(conn_index);
				return;
			}
#if 0 /*old implementation*/

			if (cmem_alvs.conn_info_result.conn_state == ALVS_TCP_CONNECTION_ESTABLISHED &&
				((iteration_num & LOG2(ALVS_TCP_EST_MULTIPLIER)) == 0)) {
				if (cmem_alvs.conn_info_result.aging_bit == 0) {
					alvs_conn_delete(conn_index);
				}
				return;
			}

			if (cmem_alvs.conn_info_result.conn_state == ALVS_TCP_CONNECTION_CLOSE_WAIT &&
				((iteration_num & LOG2(ALVS_TCP_CLOSE_WAIT_MULTIPLIER)) == 0)) {
				if (cmem_alvs.conn_info_result.aging_bit == 0) {
					alvs_conn_delete(conn_index);
				}
				return;
			}
#endif
		}
	}
}


#endif	/*ALVS_AGING_H_*/