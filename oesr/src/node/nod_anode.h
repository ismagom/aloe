/* 
 * Copyright (c) 2012, Ismael Gomez-Miguelez <ismael.gomez@tsc.upc.edu>.
 * This file is part of ALOE++ (http://flexnets.upc.edu/)
 * 
 * ALOE++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ALOE++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with ALOE++.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NOD_ANODE_H
#define NOD_ANODE_H

#include "rtdal.h"
#include "nod_waveform.h"
#include "objects_max.h"

/**
 * This is the class that processes the commands coming from the ALOE-Manager.
 * 
 * The rtdal runs a background process and creates an object of this class. The rtdal is responsible to periodically check for incoming messages from the control interface and call the process_cmd() function
 */
typedef struct {
	nod_waveform_t *loaded_waveforms;
	int max_waveforms;
	packet_t packet;
	r_itf_t sync_itf;
	r_itf_t ctr_itf;
	r_itf_t probe_itf;
	r_itf_t slaves_itf[MAX(node_itfphysic)];
} nod_anode_t;

struct log_cfg {
	int enabled;
	int log_to_stdout;
	int modules_en;
	int modules_all;
	int modules_time_en;
	int modules_time_all;
	int modules_join;
	int log_modules_level;
	int queues_en;
	int queues_all;
	int queues_join;
	int log_queues_level;
	int join_logs_sync;
};


int nod_anode_initialize(rtdal_machine_t *machine, int max_waveforms);
int nod_anode_cmd_recv();

#endif
