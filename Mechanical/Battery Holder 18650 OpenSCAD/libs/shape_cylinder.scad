//Function that given a error, calculates the number of sides
include <function_circle_error.scad>

/**
 * shape_cylinder
 *
 * This module creates a cylindrical shape using linear extrusion from either
 * a diameter (i_d) or radius (i_r), depending on which is provided.
 *
 * Parameters:
 * - i_d: Diameter of the cylinder. If provided, this will override i_r.
 * - i_r: Radius of the cylinder. Used if i_d is not provided.
 * - i_h: Height of the cylinder (default = 10).
 * - i_e: Tolerance for circular approximation (default = 0.1).
 */
module shape_cylinder
(
	i_d = 0,
	i_r = 0,
	i_h = 10,
	i_e = 0.1
)
{
	// Check if diameter is provided and use it to calculate the number of sides
	if (i_d > 0)
	{
		let (n_side = fn_num_side_to_achieve_error_round( i_d / 2, i_e, 6 ) )
		{
			echo("Number of sides (d): ", n_side);
			linear_extrude(i_h)
				circle(d = i_d, $fn = n_side );
		}
	}
	// If radius is provided instead
	else if (i_r > 0)
	{
		let (n_side = fn_num_side_to_achieve_error_round( i_r, i_e, 6 ) )
		{
			echo("Number of sides (r): ", n_side);
			linear_extrude(i_h)
				circle(r = i_r, $fn = n_side );
		}
	}
	// If neither diameter nor radius is provided
	else
	{
		echo("WARNING: shape_cylinder called with 0 r and 0 d");
	}
}

//shape_cylinder(i_d = 20);

//shape_cylinder(i_r = 20);

if (false)
shape_cylinder
(
	i_d = 20,
	i_h = 10,
	i_e = 0.1
);