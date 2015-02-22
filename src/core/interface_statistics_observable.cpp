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

	if (all_particles) {
		realloc_intlist(output_ids, output_ids->n=n_part);
		for (int i = 0; i<n_part; i++ ) {
			output_ids->e[i] = i;
		}
		return ES_OK;
	}

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

	for (int i = 0; i<n_part; i++ ) {
		flag=0;
		for (int j = 0; j<input_types->n ; j++ ) {
			if(partCfg[i].p.type == input_types->e[j])  flag=1;
		}
		if(flag==1){
			realloc_intlist(output_ids, output_ids->n=n_ids+1);
			output_ids->e[n_ids] = i;
			n_ids++;
		}
	}
	return ES_OK;
}
