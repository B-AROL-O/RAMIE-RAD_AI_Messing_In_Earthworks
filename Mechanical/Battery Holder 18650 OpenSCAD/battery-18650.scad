include <libs/shape_cylinder.scad>

//		2024-06-23
//	Printed but too short for the batteries
//		2025-08-26
//	Model for just the battery with margin
//		2025-09-18
//	Leave dimension as input, since batteries have different lengths
//	upgraded logic to use ternary condition and clean up code
//	used cylinder function that computes sides to achive a given error

// Include necessary modules or functions from OpenSCAD libraries
include <libs/shape_cylinder.scad>

/* 
Battery Constants:
    gc_l_18650 (float): Standard length of an 18650 battery
    gc_d_18650 (float): Standard diameter of an 18650 battery
    gc_l_18650_key (float): Standard length of the positive tab
    gc_d_18650_key (float): Standard diameter of the positive tab
*/
gc_l_18650 = 66.0;
gc_d_18650 = 18.4;
gc_l_18650_key = 3.0;
gc_d_18650_key = 8.0;

/* 
Module: battery_18650
Description:
    This module generates a 3D model of an 18650 lithium-ion battery with customizable dimensions and orientation.
    
Parameters:
    i_l_18650 (float): Length of the battery barrel (default = gc_l_18650)
    i_d_18650 (float): Diameter of the battery barrel (default = gc_d_18650)
    i_l_18650_key (float): Length of the positive tab (default = gc_l_18650_key)
    i_d_18650_key (float): Diameter of the positive tab (default = gc_d_18650_key)
    ix_sideway (bool): If true, models battery sideways along the X-axis (default = false)
    in_invert_poles (bool): If true, inverts the position of the positive and negative poles (default = false)
    i_e_precision (float): Precision for cylinder end flatness (default = 0.1)

Returns:
    A 3D model of an 18650 battery with specified parameters.
*/

module battery_18650
(
    // Barrel Dimensions
    i_l_18650 = gc_l_18650,
    i_d_18650 = gc_d_18650,
    // Positive Tab Dimensions
    i_l_18650_key = gc_l_18650_key,
    i_d_18650_key = gc_d_18650_key,
    // Orientation
    ix_sideway = false,
    in_invert_poles = false,
    i_e_precision = 0.1
)
{
    // Calculate the barrel length without the key tab
    i_l_18650_nokey = i_l_18650 - i_l_18650_key;

    // Translate and rotate to model battery sideways if required
    translate([0, 0, ix_sideway ? i_d_18650 / 2 : 0])
	rotate([0, ix_sideway ? 90 : 0, 0])
	union()
	{
		// Barrel (Main body of the battery)
		translate([
			0,
			0,
			in_invert_poles ? i_l_18650_key : 0
		])
		color("blue")
		shape_cylinder
		(
			i_d = i_d_18650,
			i_h = i_l_18650_nokey,
			i_e = i_e_precision
		);

		// Positive Tab (Tip of the battery)
		translate
		([
			0,
			0,
			in_invert_poles ? 0 : i_l_18650_nokey
		])
		color("gold")
		shape_cylinder
		(
			i_d = i_d_18650_key,
			i_h = i_l_18650_key,
			i_e = i_e_precision
		);
	}
}

// Example usage (uncomment to render)
//battery_18650(in_invert_poles=false, ix_sideway=false);
//battery_18650(in_invert_poles=true, ix_sideway=false);
//battery_18650(in_invert_poles=false, ix_sideway=true);
//battery_18650(in_invert_poles=true, ix_sideway=true);

