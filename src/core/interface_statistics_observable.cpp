/*
  Copyright (C) 2010,2011,2012,2013 The ESPResSo project

  This file is part of ESPResSo.

  ESPResSo is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  ESPResSo is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "errorhandling.hpp"
#include "utils.hpp"
#include "interface_statistics_observable.hpp"
#include "particle_data.hpp"
#include "statistics_observable.hpp"


int create_id_list_from_types_and_ids(IntList* output_ids, IntList *input_types, IntList *input_ids, int all_particles) {
	int flag;
	int n_ids = 0;

	//Observables rely on the partCfg being sortable. If this is not true, an ambiguous error pops up later
	if (!sortPartCfg()) {
		std::ostringstream msg;
		msg <<"Error parsing particle specifications.\nProbably your particle ids are not contiguous.\n";
		runtimeError(msg);
		return ES_ERROR;
	}

	if (!output_ids) {
		std::ostringstream msg;
		msg <<"Must provide valid pointer as first argument to create_id_list_from_types_and_ids.\n";
		runtimeError(msg);
	}

	if (all_particles) {
		realloc_intlist(output_ids, output_ids->n=n_part);
		for (int i = 0; i<n_part; i++ ) {
			output_ids->e[i] = i;
		}
		return ES_OK;
	}

	if (input_ids) {
		realloc_intlist(output_ids, output_ids->n=input_ids->n);

		for (int i=0; i<input_ids->n; i++) {
			if (input_ids->e[i] >= n_part) {
				std::ostringstream msg;
				msg <<"Error parsing ID list. Given particle ID exceeds the number of existing particles\n";
				runtimeError(msg);
				return ES_ERROR;
			}
			output_ids->e[i] = input_ids->e[i];
		}
		return ES_OK;
	}

	if (input_types) {
		n_ids = 0;
		for (int i = 0; i<n_part; i++ ) {
			flag=0;
			for (int j = 0; j<input_types->n ; j++ ) {
				if(partCfg[i].p.type == input_types->e[j]) flag=1;
			}
			if(flag==1){
				realloc_intlist(output_ids, output_ids->n=n_ids+1);
				output_ids->e[n_ids] = i;
				n_ids++;
			}
		}
		return ES_OK;
	}
	std::ostringstream msg;
	msg <<"Error parsing ID list. Must pass all particles, particle ids, or particle types.\n";
	runtimeError(msg);
	return ES_ERROR;
}

#define CREATE_PARTICLE_OBSERVABLE(observable_to_register) \
		if (observable_name==#observable_to_register) { \
			observable_##observable_to_register *new_obs; \
			new_obs = new observable_##observable_to_register (input_types, input_ids, all_particles); \
			n_observables++; \
			observables[id] = new_obs; \
			return id; \
		}
#define PARTICLE_OBSERVABLE_MISSING(observable_to_register) \
		if (observable_name==#observable_to_register) { \
			fprintf(stderr,"Observable ", #observable_to_register," not compiled in!\n"); \
			errexit(); \
		}

int create_python_observable(std::string observable_name, IntList *input_types, IntList *input_ids, int all_particles){
	int id;

	// find the next free observable id
	for (id=0;id<n_observables;id++)
		if ( observables+id == 0 ) break;
	if (id==n_observables)
		observables=(observable**) realloc(observables, (n_observables+1)*sizeof(observable*));

	CREATE_PARTICLE_OBSERVABLE(particle_angular_momentum);
	CREATE_PARTICLE_OBSERVABLE(particle_body_angular_momentum);
#ifdef ELECTROSTATICS
	CREATE_PARTICLE_OBSERVABLE(particle_currents);
#else
	PARTICLE_OBSERVABLE_MISSING(particle_currents);
#endif
	CREATE_PARTICLE_OBSERVABLE(particle_forces);
	CREATE_PARTICLE_OBSERVABLE(particle_positions);
	CREATE_PARTICLE_OBSERVABLE(particle_velocities);
	CREATE_PARTICLE_OBSERVABLE(particle_body_velocities);


	fprintf(stderr,"Observable type not recognized\n");
	errexit();
	return ES_ERROR;
}

std::vector<double> get_observable_values(int id){
	observable_calculate(observables[id]);
	return observables[id]->return_observable_values();
}
std::vector<int> get_observable_ids(int id){
	return observables[id]->return_observable_ids();
}
