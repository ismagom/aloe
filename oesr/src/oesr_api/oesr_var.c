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

#include <stddef.h>
#include "rtdal.h"
#include "oesr.h"
#include "oesr_context.h"
#include "nod_waveform.h"


/**
 * Creates a new public variable with a given name. After this call size bytes of the buffer ptr may be
 * accessible by other modules or ALOE (through the OESR Manager API) for either read or write.
 *
 * \param context OESR context pointer
 * \param name Name of the variable. This name will be used by other modules or the OESR Manager API to
 * view/modify the variable value
 * \param ptr Pointer to the user memory where the variable is located
 * \param size Size of the variable
 * \return On success, oesr_var_create() returns a non-null handler which is passed as a first parameter
 * to oesr_var_close() to close the public variable.
 *
 */
var_t oesr_var_create(void *context, char *name, void *ptr, int size) {
	oesr_context_t *ctx = context;
	nod_module_t *module = ctx->module;
	OESR_ASSERT_PARAM_P(name);
	sdebug("name=%s, size=%d, ptr=0x%x\n",name,size,ptr);
	OESR_ASSERT_PARAM_P(module);
	OESR_ASSERT_PARAM_P(size>0);
	OESR_ASSERT_PARAM_P(ptr);

	variable_t *variable;
	variable = nod_module_variable_get(module, name);
	if (!variable) {
		variable = nod_module_variable_create(module, name,0);
		if (!variable) {
			return NULL;
		}
	}
	variable->size = size;
	variable->cur_value = ptr;
	return 0;
}

var_t oesr_var_param_create_remote(void *context, int module_idx, char *name, int size) {
	oesr_context_t *ctx = context;
	OESR_ASSERT_PARAM_P(name);
	sdebug("remote_idx=%d name=%s, size=%d\n",module_idx,name,size);
	OESR_ASSERT_PARAM_P(module_idx);
	OESR_ASSERT_PARAM_P(size>=0);

	nod_module_t *my_module = ctx->module;
	nod_waveform_t *waveform = (nod_waveform_t*) my_module->parent.waveform;
	nod_module_t *his_module = (nod_module_t*) &waveform->modules[module_idx];


	return (var_t) nod_module_variable_create(his_module, name, size);
}

/** Sets up to size bytes of the buffer pointed by ptr to the value of the parameter
 * returned by oesr_var_param_get()
 *
 * \param context OESR context pointer
 * \param parameter Handler returned by the oesr_var_param_get() function.
 * \param value Pointer to the user memory where the parameter value will be stored
 * \param size Size of user memory buffer
 *
 * \return On success, returns a non-negative integer indicating the number of bytes written to value.
 * On error returns -1
 */
int oesr_var_param_get_value(void *context, var_t parameter, void* value, int size) {
	int cpy_sz;

	cast(ctx,context);

	OESR_ASSERT_PARAM(parameter);
	OESR_ASSERT_PARAM(value);
	OESR_ASSERT_PARAM(size>0);

	nod_module_t *module = (nod_module_t*) ctx->module;
	variable_t *variable = (variable_t*) parameter;

	sdebug("id=0x%x, size=%d, value=0x%x, cur_mode=%d\n",parameter,size,value,module->parent.mode.cur_mode);

	cpy_sz = (variable->size > size)?size:variable->size;
	if (module->parent.mode.next_tslot &&
			module->parent.mode.next_tslot <= rtdal_time_slot()) {
		module->parent.mode.cur_mode = module->parent.mode.next_mode;
		module->parent.mode.next_tslot = 0;
	}
	memcpy(value, variable->init_value[module->parent.mode.cur_mode], (size_t) cpy_sz);
	sdebug("id=0x%x, copied=%d\n", variable, cpy_sz);
	return cpy_sz;
}


/** Sets up to size bytes of the value of the parameter to the value of the
 * buffer pointed by ptr to the
 *
 * \param context OESR context pointer
 * \param parameter Handler returned by the oesr_var_param_get() function.
 * \param value Pointer to the user memory where the parameter value will be stored
 * \param size Size of user memory buffer
 *
 * \return On success, returns a non-negative integer indicating the number of bytes written to value.
 * On error returns -1
 */
int oesr_var_param_set_value(void *context, var_t parameter, void* value, int size) {
	int cpy_sz;

	cast(ctx,context);

	sdebug("set_value id=0x%x, size=%d\n",parameter,size);

	OESR_ASSERT_PARAM(parameter);
	OESR_ASSERT_PARAM(value);
	OESR_ASSERT_PARAM(size>0);

	nod_module_t *module = (nod_module_t*) ctx->module;
	variable_t *variable = (variable_t*) parameter;

	cpy_sz = (size > variable->size)?variable->size:size;

	memcpy(variable->init_value[module->parent.mode.cur_mode], value, (size_t) cpy_sz);
	sdebug("id=0x%x, copied=%d\n", parameter, cpy_sz);
	return cpy_sz;
}

/** Sets up to size bytes of the value of the parameter to the value of the
 * buffer pointed by ptr to the
 *
 * \param context OESR context pointer
 * \param idx Index of the parameter in the local database
 * \param value Pointer to the user memory where the parameter value will be stored
 * \param size Size of user memory buffer
 *
 * \return On success, returns a non-negative integer indicating the number of bytes written to value.
 * On error returns -1
 */
int oesr_var_param_set_value_idx(void *context, int idx, void* value, int size) {
	int cpy_sz;

	cast(ctx,context);

	sdebug("%s: set_value_idx idx=%d, value 0x%x size=%d\n",oesr_module_name(ctx),idx,value,size);
	OESR_ASSERT_PARAM(idx>=0);
	OESR_ASSERT_PARAM(value);
	OESR_ASSERT_PARAM(size>0);

	nod_module_t *module = (nod_module_t*) ctx->module;
	variable_t *variable = (variable_t*) &module->parent.variables[idx];

	/*
	if (!strcmp(oesr_module_name(ctx),"ctrl")) {
		printf("%s: %d: set variable %s value %d pos %d\n",oesr_module_name(ctx),oesr_tstamp(context),
					variable->name,*((int*) value),idx);
	}
	*/

	cpy_sz = (size > variable->size)?variable->size:size;

	memcpy(variable->init_value[module->parent.mode.cur_mode], value, (size_t) cpy_sz);
	sdebug("id=0x%x, copied=%d\n", variable, cpy_sz);
	return cpy_sz;
}



/**
 * Fills the buffer parameters with up to max_elems module parameters defined in the app
 *
 * \param context OESR context pointer
 * \param parameter Pointer to a parameters buffer
 * \param max_elems Size of the parameters buffer
 *
 * \return On success, returns a non-negative number indicating the number of parameters
 * successfully copied to the buffer, on error returns -1
 */
int oesr_var_param_list(void *context, var_t *parameters, int max_elems) {
	cast(ctx,context);
	int i;

	OESR_ASSERT_PARAM(parameters);
	OESR_ASSERT_PARAM(max_elems>=0);

	nod_module_t *module = (nod_module_t*) ctx->module;

	if (max_elems > module->parent.nof_variables) {
		max_elems = module->parent.nof_variables;
	}
	for (i=0;i<max_elems;i++) {
		parameters[i] = (var_t) &module->parent.variables[i];
		sdebug("list %d %s id=0x%x size %d\n",i,module->parent.variables[i].name,
				parameters[i],module->parent.variables[i].size);
	}
	return max_elems;
}




/**
 * Returns a handler for the parameter with name "name".
 * This handler is then used by the functions oesr_var_param_value() and oesr_var_param_type()
 *
 * \param context OESR context pointer
 * \param name Name of the parameter, as it appears in the waveform .app file
 * \return On success, returns a non-null handler. On error returns null.
 */
var_t oesr_var_param_get(void *context, char *name) {
	cast_p(ctx,context);
	OESR_ASSERT_PARAM_P(name);
	sdebug("name=%s\n",name);

	nod_module_t *module = (nod_module_t*) ctx->module;
	variable_t *variable = nod_module_variable_get(module,name);
	if (!variable) {
		OESR_SETERROR(OESR_ERROR_NOTFOUND);
		return NULL;
	}
	return (var_t) variable;
}

/**
 * Returns the parameter variable type (see enum oesr_var_type_t)
 * \param context OESR context pointer
 * \param parameter Handler returned by the oesr_var_param_get() function.
 */
oesr_var_type_t oesr_var_param_type(void *context, var_t parameter) {
	cast(ctx,context);

	sdebug("id=0x%x\n",parameter);

	OESR_ASSERT_PARAM(parameter);

	variable_t *variable = (variable_t*) parameter;
	return (oesr_var_type_t) variable->type;
}

/**
 * Closes a variable handler. After a call to this function, ALOE can not access to
 * this variable anymore. However, the module may still use the contents of the memory address
 * passed as a parameter in the oesr_var_create() function.
 *
 * \param context OESR context pointer
 * \param parameter Handler returned by the oesr_var_param_get() function.
 *
 * \returns 0 on success, -1 on error
 */
int oesr_var_close(void *context, var_t var) {
	cast(ctx,context);
	variable_t *variable = (variable_t*) var;
	OESR_ASSERT_PARAM(var);
	variable->size = 0;
	variable->id = 0;
	variable->cur_value = NULL;
	return 0;
}
