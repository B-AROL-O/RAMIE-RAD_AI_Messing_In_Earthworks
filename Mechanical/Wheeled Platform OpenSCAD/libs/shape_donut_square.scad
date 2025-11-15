//Compute the number of sides needed to achieve a given resolution
include <function_circle_error.scad>

/**
 * @date 2025-11-07
 * @brief Creates a 3D donut-shaped square using rotate_extrude.
 *
 * This module generates a hollow cylindrical segment (donut) by rotating a square
 * around an arc. The shape is defined by inner and outer radii, thickness,
 *
 * @param ir_inner Inner radius of the donut hole (distance from center to inner edge).
 * @param ir_outer Outer radius of the donut (distance from center to outer edge).
 * @param it Thickness of the donut's cross-section (height in Z-direction).
 * @param ie_precision Tolerance for circular approximation. Lower values = smoother curves.
 *
 */

module shape_donut_square
(
	ir_inner = 2,
	ir_outer = 10,
	it = 3,
	ie_precision = 0.01
)
{
	//rotate extrude already takes into account the arc
	n_sides = fn_num_side_to_achieve_error_round
	(
		ir_outer, 
		ie_precision,
		6
	);
	//echo("Error:", ie_precision , "Number of Sides: ",n_sides);
	rotate_extrude(angle=360, $fn=n_sides)
	translate([ir_inner,0])
	square([ir_outer-ir_inner,it]);
}

if (false)
shape_donut_square();