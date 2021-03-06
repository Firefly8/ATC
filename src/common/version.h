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
*	file - version.h
*	description - define version of DP & daemon application
*/

#ifndef SRC_COMMON_VERSION_H_
#define SRC_COMMON_VERSION_H_

/*
 * P.GGMC.ZZZZ	- Version format
 * P		- Major 16 bits, fixed product number, allocated value is 22
 * GG		- Minor bits 15:8, 2 digits, GA release number 1, 2, 3..
 * M		- Minor bits 7:4, 1 digit, release number 0, 1, 2, 3...9
 * C		- Minor bits 3:0, 1 digit, branch maintenance number (customer), reset on each GA release
 * ZZZZ		- Currently zero
 *
 */


#ifdef NDEBUG
#	define DEBUG ""
#else
#	define DEBUG "-debug"
#endif

#ifdef CONFIG_ALVS
#       define APP_VERSION "$Revision: 22.0300.0000"DEBUG" $"
#endif
#ifdef CONFIG_TC
#       define APP_VERSION "$Revision: 24.0100.0000"DEBUG" $"
#endif

/**************************************************************************//**
 * \brief       Get version of the application
 *
 * \return      char *- pointer to version string
 */
const char *get_version(void);

#endif /* SRC_COMMON_VERSION_H_ */
