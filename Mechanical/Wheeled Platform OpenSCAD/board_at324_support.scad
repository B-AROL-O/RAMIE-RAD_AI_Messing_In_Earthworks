/*	Two support structure meant to hold the AT324 board on wheeled platrofms
**	I make a sloped 
**
**
*/

include <libs/shape_wall_sloped_with_bolt_nut.scad>

include <board_at324.scad>


module at324_module_with_support_sloped
(
	//Interaxis between holes
	i_wi_hole = gc_li_at324_hole,


	//Show extra models
	i_x_show_at324_module = true,
	dummy
)
{
	for (wo_temp = [-i_wi_hole/2,i_wi_hole/2])
	{
		translate([0,wo_temp,0])
		shape_wall_sloped_with_bolt_nut
		(
			//	OUTER PROFILE
			//Depth of the wall
			i_l_wall = gc_w_at324,
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
			
		);
	}


}

//if (false)
at324_module_with_support_sloped();