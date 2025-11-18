/*	Two support structure meant to hold the AT324 board on wheeled platrofms
**	I make a sloped 
**
**
*/

include <libs/shape_wall_sloped_with_bolt_nut.scad>

include <board_at324.scad>


module at324_module_with_support_sloped
(
	//	SIZE
	i_w_wall = 50,
	i_h_wall = 70,
	i_h_wall_vertical = 50,
	i_t_wall = 5,
	//	ANGLE AND SLOPE
	i_a_slope = 60,
	//Interaxis between holes
	i_wi_hole = gc_li_at324_hole,
	//Distance of the upper hole from the start of the slope
	i_li_hole_upper = 5,
	//Distance of the lower hole from the start of the slope
	i_li_hole_lower = 5+gc_wi_at324_hole,
	//	HOLE
	//Drill diameter
	i_d_bolt_drill = 2.5+0.3,
	//Length of the bolt
	i_h_bolt = 10,
	//Nut slot width and height
	i_w_nut = 5+0.5,
	i_h_nut = 3,

	//	SHOW
	//Show extra models
	i_x_show_at324_module = true
)
{
	/*
	//Size of the slope
	h_slope = i_h_wall -i_h_wall_vertical;
	l_slope = h_slope * tan(i_a_slope);
	//Flat section at the top
	l_wall_horizontal = i_l_wall - l_slope;
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
	*/

	union()
	{
		if (i_x_show_at324_module==true)
		{
			translate([0,0,i_h_wall])
			rotate([i_a_slope-90,0,-90])
			at324_board();
		}

		//Spawn sloped walls
		for (wo_temp = [-i_wi_hole/2,i_wi_hole/2])
		{
			translate([0,wo_temp,0])
			shape_wall_sloped_with_bolt_nut
			(
				//	OUTER PROFILE
				//Depth of the wall
				i_l_wall = i_w_wall,
				//Height of the wall
				i_h_wall = i_h_wall,
				//Thickness of the wall
				i_t_wall = i_t_wall,
				
				//	SLOPE
				//Vertical section leading to the slope
				i_h_wall_vertical = i_h_wall_vertical,
				//Angle of the slope 0=VERTICAL 90=Horizontal
				i_a_slope = i_a_slope,

				// SLOPE HOLE POSITION
				//Distance of the upper hole from the start of the slope
				i_li_hole_upper = i_li_hole_upper,
				//Distance of the lower hole from the start of the slope
				i_li_hole_lower = i_li_hole_lower,

				//	HOLE
				//Drill diameter
				i_d_bolt_drill = i_d_bolt_drill,
				//Length of the bolt
				i_h_bolt = i_h_bolt,
				//Nut slot width and height
				i_w_nut = i_w_nut,
				i_h_nut = i_h_nut
				
			);
		}
	}

}

if (false)
at324_module_with_support_sloped();