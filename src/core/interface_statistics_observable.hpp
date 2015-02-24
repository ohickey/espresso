#include "statistics_observable.hpp"
#include "errorhandling.hpp"
#include "utils.hpp"

int create_id_list_from_types_and_ids(IntList *output_ids, IntList *input_types, IntList *input_ids, int all_particles);

#define PARTICLE_OBSERVABLE(obs_name,obs_dimension) \
	class observable_##obs_name : public observable { \
	public: \
		observable_##obs_name (); \
		observable_##obs_name (IntList *input_types, IntList *input_ids, int all_particles) { \
			IntList *ids = NULL; \
			if ( create_id_list_from_types_and_ids (ids, input_types, input_ids, all_particles) ) { \
				std::ostringstream msg; \
				msg <<"Error parsing ID list for a n dimensional particle property\n"; \
				runtimeError(msg); \
			} \
			this->update=0; \
			this->container=ids; \
			this->n=obs_dimension*ids->n; \
			this->last_value=(double*)malloc(this->n*sizeof(double)); \
			this->calculate=&observable_calc_##obs_name; \
		} \
	};

PARTICLE_OBSERVABLE(particle_velocities,3);
PARTICLE_OBSERVABLE(particle_body_velocities, 3);
PARTICLE_OBSERVABLE(particle_angular_momentum, 3);
PARTICLE_OBSERVABLE(particle_body_angular_momentum, 3);
