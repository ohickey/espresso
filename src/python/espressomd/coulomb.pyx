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
# include "myconfig.pxi"
# 
# IF ELECTROSTATICS ==1:
#   import numpy as np
# 
#   cdef class DHparaHandle:
#     
#     def __init__(self):
#       print 'DH implementation not finished'
#       #set coulomb method, force both params to be set?
#   
#     property kappa:
#       def __set__(self, double _kappa):
#         if _kappa<0:
#           raise ValueError("kappa must be > 0")
#         dh_set_params(_kappa, dh_params.r_cut)
#       def __get__(self):
#         return dh_params.kappa
#     property r_cut:
#       def __set__(self, double _r_cut):
#         if _r_cut<0:
#           raise ValueError("r_cut must be > 0")
#         dh_set_params(dh_params.kappa, _r_cut)
#       def __get__(self):
#         return dh_params.r_cut