#
# Copyright (C) 2013,2014 The ESPResSo project
#  
# This file is part of ESPResSo.
#  
# ESPResSo is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#  
# ESPResSo is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#  
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>. 
#  
# For C-extern Analysis 

cimport numpy as np
from utils cimport *
from libcpp.string cimport string #import std::string as string
from libcpp.vector cimport vector #import std::vector as vector

cdef extern from "statistics.hpp":
  ctypedef struct Observable_stat:
    int init_status
    DoubleList data
    int n_coulomb
    int n_dipolar
    int n_non_bonded
    double *bonded
    double *non_bonded
    double *coulomb
    double *dipolar

cdef extern from "statistics.hpp":
  cdef double mindist(IntList *set1, IntList *set2)
  cdef void nbhood(double pos[3], double r_catch, IntList *il, int planedims[3])
  cdef double distto(double pos[3], int pid)
  cdef double *obsstat_bonded(Observable_stat *stat, int j)
  cdef double *obsstat_nonbonded(Observable_stat *stat, int i, int j)
  cdef void analyze_pressure_all(int v_comp, vector[string] *pressure_labels, vector[double] *pressures)
  cdef double analyse_pressure(string pressure_to_calc, int v_comp)
  cdef double analyse_pressure_pair(string pressure_to_calc, int bond_or_type, int v_comp)
  cdef double analyse_pressure_single(string pressure_to_calc, int bond_or_type, int v_comp)
  cdef void analyze_stress_tensor_all(int v_comp, vector[string] *stressTensorLabel, vector[double] *stressTensorValues)
  cdef double* analyze_stress_tensor(string pressure_to_calc, int v_comp)
  cdef double* analyse_stress_pair(string pressure_to_calc, int type1, int type2, int v_comp)
  cdef double* analyse_stress_single(string pressure_to_calc, int bond_or_type, int v_comp)
  cdef analyze_local_stress_tensor(int* input_args, DoubleList* local_stress_tensor)


cdef extern from "energy.hpp":
  cdef Observable_stat total_energy

cdef extern from "energy.hpp":
  cdef void master_energy_calc()
  cdef void init_energies(Observable_stat *stat)
