#include <vector>
#include "statistics_observable.hpp"
#include "errorhandling.hpp"
#include "utils.hpp"

int create_id_list_from_types_and_ids(IntList *output_ids, IntList *input_types, IntList *input_ids, int all_particles);
int create_python_observable(std::string observable_name, IntList *input_types, IntList *input_ids, int all_particles);
std::vector<double> get_observable_values(int id);
std::vector<int> get_observable_ids(int id);

#define PARTICLE_OBSERVABLE(observable_to_register,obs_dimension) \
		class observable_##observable_to_register : public observable { \
		public: \
		observable_##observable_to_register (); \
		observable_##observable_to_register (IntList *input_types, IntList *input_ids, int all_particles) { \
			this->n=obs_dimension*5; \
			IntList *ids = (IntList*) malloc(sizeof(IntList)); \
			init_intlist(ids); \
			if ( create_id_list_from_types_and_ids (ids, input_types, input_ids, all_particles) ) { \
				std::ostringstream msg; \
				msg <<"Error parsing ID list for a n dimensional particle property\n"; \
				runtimeError(msg); \
			} \
			this->update=0; \
			this->container=ids; \
			this->n=obs_dimension*ids->n; \
			this->last_value=(double*)malloc(this->n*sizeof(double)); \
			this->calculate=&observable_calc_##observable_to_register; \
			this->obs_name = (char*)malloc((1+strlen(#observable_to_register))*sizeof(char)); \
			strcpy(this->obs_name, #observable_to_register); \
		} \
};

PARTICLE_OBSERVABLE(particle_velocities,3);
PARTICLE_OBSERVABLE(particle_body_velocities, 3);
PARTICLE_OBSERVABLE(particle_angular_momentum, 3);
PARTICLE_OBSERVABLE(particle_body_angular_momentum, 3);
