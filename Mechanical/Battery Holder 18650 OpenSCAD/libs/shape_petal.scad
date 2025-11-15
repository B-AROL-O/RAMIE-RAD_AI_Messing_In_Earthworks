//Function that given a error, calculates the number of sides
include <function_circle_error.scad>

include <shape_cylinder.scad>

// This file defines a shape_petal module, which creates a petal-shaped object ideal for holding round objects with snap-fit action.

/**
 * Module: shape_petal
 *
 * Creates a petal-like shape formed by an arc of a ring, tipped by a smooth cylinder.
 * Ideal for applications involving spring or snap-fit mechanisms.
 *
 * Parameters:
 * - i_r_inner (default = 10): Inner radius of the ring (in mm).
 * - i_r_thickness (default = 1): Thickness of the ring (in mm).
 * - i_l_length (default = 20): Length of the petal's body (in mm).
 * - i_a_angle (default = 90): Angle of arc for the petal (in degrees).
 * - i_e (default = 0.01): Error tolerance for precision in calculations.
 *
 * Uses:
 * - fn_num_side_to_achieve_error_round: Calculates number of sides based on error and radius.
 * - shape_cylinder: Draws a cylinder to close the tip of the petal.
 */
 
module shape_petal
(
	i_r_inner = 10,
	i_r_thickness = 1,
	i_l_length =20,
	i_a_angle = 90,
	i_e = 0.01
)
{
	n_arc_side = fn_num_side_to_achieve_error_round
	(
		i_r_inner,
		i_e,
		6
	);
	echo("Petal Sides: ",  n_arc_side );

    //Rest on donut belly
    translate
	([
		0,
		0,
		i_r_inner+i_r_thickness
	])
    rotate
	([
		90,
		90,
		90
	])
    //Donut construction
    rotate_extrude
	(
		angle=i_a_angle,
		//sides for the full 360° rotation
		$fn = n_arc_side
	)
	//Create the secction of the donut
    translate
	([
		i_r_inner,
		0,
		0
	]) 
    square
	([
		i_r_thickness,
		i_l_length
	]);

    //Add a cylinder on top of the donut to close in STYLE the shape
    //Move the cylinder on the tip of the donut
    translate
	([
		0,
		(i_r_thickness/2+i_r_inner)*sin(i_a_angle),
		i_r_thickness/2 +(i_r_thickness/2+i_r_inner)*(1-cos(i_a_angle))
	])
    //Rotate the cylinder in the right direction
    rotate
	([
		0,
		90,
		0
	])
    //Construct the cylinder
	shape_cylinder
	(
		i_d = i_r_thickness,
		i_h = i_l_length,
		i_e = i_e
	);
}

if (false)
shape_petal
(
	i_r_inner = 10,
	i_r_thickness = 1,
	i_l_length =20,
	i_a_angle = 90,
	i_e = 0.01
);