# Set the slit pore geometry the width is the non-periodic part of the geometry
# the padding is used to ensure that there is no field outside the slit

proc friction_proc {gamma_actual dens_at_r lb_visc lb_rho} {
  set F_s [expr 10*$dens_at_r/pow(1-$dens_at_r,3.0)]
  set a_s [expr $gamma_actual/(6*3.1416*$lb_visc*$lb_rho)]
  set l_B [expr 2*$a_s*$a_s/(9*$dens_at_r*$F_s)]
  set fric [expr $lb_visc*$lb_rho/($l_B*$l_B)]
  return $fric
}

set box_l 96


setmd box_l $box_l $box_l $box_l

# Set the electrokinetic parameters

set agrid 1.0
set dt 0.01
set kT 1.0
set bjerrum_length 2.0

set lb_visc 1.0
set lb_rho 1.0
set lb_gamma 1.0
set gamma_actual [expr 1.0/$lb_gamma+1.0/(25*$lb_visc*$lb_rho*$agrid)]
set D [expr $kT/$gamma_actual]

set E_field 0.1

# Set the simulation parameters

setmd time_step $dt
setmd skin 0.1
thermostat off

#TODO, ADJUST
set integration_length 200000

# Set up the (LB) electrokinetics fluid

electrokinetics agrid $agrid lb_density $lb_rho viscosity $lb_visc friction $lb_gamma T $kT bjerrum_length $bjerrum_length

# Set up the charged and neutral species

set charge_gel 1222
set valency_counterions -1.0
set density_counterions [expr 1.0*$charge_gel/pow($box_l,3.0)]
electrokinetics 1 density $density_counterions D $D valency $valency_counterions ext_force $valency_counterions*$E_field 0 0


#setup salt ions
set density_salt 0.01
electrokinetics 2 density $density_salt D $D valency $valency_counterions ext_force $valency_counterions*$E_field 0 0
electrokinetics 3 density $density_salt D $D valency 1 ext_force $E_field 0 0

for {set xi 0} {$xi<$box_l} {incr xi} {
  for {set yi 0} {$yi<$box_l} {incr yi} {
    for {set zi 0} {$zi<$box_l} {incr zi} {
      set r [expr pow($xi-$box_l/2.0,2.0) + pow($yi-$box_l/2.0,2.0) + pow($zi-$box_l/2.0,2.0)]
      set rbin [expr int($r/$r_bin_size+0.5)]
      set dens_at_r [lindex $dens $rbin]
      part lb_nodex_x lb_node_y lb_node_z type 0 q $dens_at_r mu_E [friction_proc $gamma_actual $dens_at_r $lb_visc $lb_rho ] 0 0 fix 1 1 1
    }
  }
}
# Integrate the system

integrate $integration_length

# Output

set fp [open "eof_electrokinetics.dat" "w"]
puts $fp "#position measured_density measured_velocity measured_pressure_xz"

for {set i 0} {$i < [expr $box_l/$agrid]} {incr i} {
  if {[expr $i*$agrid] >= 0 && [expr $i*$agrid] < $box_l } {
    set xvalue [expr $i*$agrid]
    set position [expr $i*$agrid - $box_l/2.0 + $agrid/2.0]

    # density
    set measured_density [electrokinetics 1 node [expr int($box_l/(2*$agrid))] [expr int($box_l/(2*$agrid))] $i print density]

    # velocity
    set measured_velocity [lindex [electrokinetics node [expr int($box_l/(2*$agrid))] [expr int($box_l/(2*$agrid))] $i print velocity] 0]

    # xz component pressure tensor
    set measured_pressure_xz [lindex [lbnode [expr int($box_l/(2*$agrid))] [expr int($box_l/(2*$agrid))] $i print pi_neq] 3]

    puts $fp "$position $measured_density $measured_velocity $measured_pressure_xz"
  }
}

close $fp
