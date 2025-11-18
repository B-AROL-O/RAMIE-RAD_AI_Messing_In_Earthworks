/**
 * shape_wall_sloped_with_bolt_nut
 *
 * Creates a sloped wall profile with two drilled holes and optional nut slots.
 * The wall is extruded along the Z‑axis with the specified thickness. Two
 * vertical sections (one before the slope and one after) are connected by a
 * linear segment that forms an angled slope. Holes can be placed on either side
 * of the slope at user‑defined distances from the start of the slope.
 *
 * Parameters:
 * - i_l_wall: Length of the wall in the X direction (mm).
 * - i_h_wall: Height of the entire wall in the Y direction (mm).
 * - i_t_wall: Thickness of the wall, extrusion depth along Z (mm).
 *
 * - i_h_wall_vertical: Height of the vertical section before the slope
 *   (mm). The remaining height will be used for the sloped portion.
 * - i_a_slope: Angle of the slope in degrees. 0° = vertical,
 *   90° = horizontal.
 *
 * - i_li_hole_upper: Distance from the start of the slope to the upper
 *   hole along the slope direction (mm).
 * - i_li_hole_lower: Distance from the start of the slope to the lower
 *   hole along the slope direction (mm).
 *
 * - i_d_bolt_drill: Diameter of the drill for both holes (mm).
 * - i_h_bolt: Length of the bolt that passes through the wall (mm).
 * - i_w_nut: Width of the square nut slot (mm).
 * - i_h_nut: Height/thickness of the square nut slot (mm).
 *
 * The module automatically calculates the slope length and positions for
 * the holes. Two cube prisms are added at each hole to represent the
 * mounting nuts, oriented to match the direction of the bolt.
 *
 * Example usage:
 * shape_sloped_wall_with_holes(
 *     i_l_wall=80,
 *     i_h_wall=70,
 *     i_t_wall=5,
 *     i_h_wall_vertical=50,
 *     i_a_slope=60,
 *     i_li_hole_upper=10,
 *     i_li_hole_lower=30,
 *     i_d_bolt_drill=3,
 *     i_h_bolt=20,
 *     i_w_nut=6,
 *     i_h_nut=3
 * );
 */
module shape_wall_sloped_with_bolt_nut
(
	//	OUTER PROFILE
	//Depth of the wall
	i_l_wall = 50,
	//Height of the wall
	i_h_wall = 70,
	//Thickness of the wall
	i_t_wall = 5,
	

	//	SLOPE
	//Vertical section leading to the slope
	i_h_wall_vertical = 50,
	//Angle of the slope 0=VERTICAL 90=Horizontal
	i_a_slope = 60,

	// SLOPE HOLE POSITION
	//Distance of the upper hole from the start of the slope
	i_li_hole_upper = 10,
	//Distance of the lower hole from the start of the slope
	i_li_hole_lower = 30,

	//	HOLE
	//Drill diameter
	i_d_bolt_drill = 3,
	//Length of the bolt
	i_h_bolt = 20,
	//Nut slot width and height
	i_w_nut = 6,
	i_h_nut = 3
	
)
{

	h_slope = i_h_wall -i_h_wall_vertical;
	//echo("h_slope", h_slope);

	l_slope = h_slope * tan(i_a_slope);
	//echo("l_slope", l_slope);

	//Flat section at the top
	l_wall_horizontal = i_l_wall - l_slope;
	//echo("l_wall_horizontal", l_wall_horizontal);


	//echo("Tan:", tan(i_a_slope));

	//Halfway point slope
	an_slope_halfway =
	[
		l_slope/2,
		i_h_wall -h_slope/2
	];

	//Upper Hole
	an_upper_hole = 
	[
		l_slope - i_li_hole_upper * sin(i_a_slope),
		i_h_wall -i_li_hole_upper * cos(i_a_slope)

	];


	//Lower Hole
	an_lower_hole = 
	[
		l_slope - i_li_hole_lower * sin(i_a_slope),
		i_h_wall -i_li_hole_lower * cos(i_a_slope)

	];

	



	aan_points = 
	[
		//X = Length Y = Height
		//Bottom Left
		[0,0],
		//Bottom Right
		[i_l_wall,0],
		//Top Right
		[i_l_wall,i_h_wall],
		//Horizontal section leading to the slope
		[l_slope,i_h_wall],
		
		//UPPER HOLE
		an_upper_hole,

		//an_slope_halfway,

		an_lower_hole,

		//Vertical section after the slope
		[0,i_h_wall-h_slope]
		


	];

	//Center on 0,0
	translate([i_l_wall/2,0,0])
	//Put upright
	rotate([90,0,180])
	difference()
	{
		union()
		{
			linear_extrude( i_t_wall,center=true )
			polygon(aan_points);
		} //End sum
		union()
		{
			//	UPPER HOLE
			//Translate in position
			translate(an_upper_hole)
			//Rotate toward the slope
			rotate([0,0,-i_a_slope+90])
			//Point it toward the structure down
			rotate([90,0,0])
			//HACK because of rounding
			translate([0,0,-0.001])
			//This points up
			cylinder(h=i_h_bolt, d=i_d_bolt_drill, $fn = 100);

			//	LOWER HOLE
			//Translate in position
			translate(an_lower_hole)
			//Rotate toward the slope
			rotate([0,0,-i_a_slope+90])
			//Point it toward the structure down
			rotate([90,0,0])
			//HACK because of rounding
			translate([0,0,-0.001])
			//This points up
			cylinder(h=i_h_bolt, d=i_d_bolt_drill, $fn = 100);

			//UPPER NUT
			//Translate in position
			translate(an_upper_hole)
			//Rotate toward the slope
			rotate([0,0,-i_a_slope+90])
			//Translate to the tip of the bolt
			translate([0,-i_h_bolt,0])
			//This points up
			cube([i_w_nut, i_h_nut, i_t_wall*1.1], center=true);

			//LOWER NUT
			//Translate in position
			translate(an_lower_hole)
			//Rotate toward the slope
			rotate([0,0,-i_a_slope+90])
			//Translate to the tip of the bolt
			translate([0,-i_h_bolt,0])
			//This points up
			cube([i_w_nut, i_h_nut, i_t_wall*1.1], center=true);
			
			
			
		} //End sub

	} //End difference

}

if (false)
shape_wall_sloped_with_bolt_nut();