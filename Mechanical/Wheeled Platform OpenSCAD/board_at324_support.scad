/*	Two support structure meant to hold the AT324 board on wheeled platrofms
**	I make a sloped 
**
**
*/

//Board outer dimensions
gc_l_board = 80.0;
gc_w_board = 45.0;
gc_h_board = 10.0;
//The LCD board is above and aligned
gc_w_board_lcd = 36.0;
gc_h_board_lcd = 35.0;

//Board Hole interaxis
gc_li_board_hole = 75.0;
gc_wi_board_hole = 35.0;



module shape_sloped_beam
(
	//	OUTER PROFILE
	//Depth of the wall
	i_l_wall = 50,
	//Thickness of the wall
	i_t_wall = 5,
	//Height of the wall
	i_h_wall = 70,

	//	SLOPE
	//Vertical section leading to the slope
	i_h_wall_vertical = 50,
	//Angle of the slope
	i_a_slope = 60,

	//	HOLE
	i_d_drill = 3,
	//Distance of the upper hole from the start of the slope
	i_li_hole_upper = 10,
	//Distance of the lower hole from the start of the slope
	i_li_hole_lower = 30,


	dummy
)
{

	h_slope = i_h_wall -i_h_wall_vertical;
	echo("h_slope", h_slope);

	l_slope = h_slope * tan(i_a_slope);
	echo("l_slope", l_slope);

	//Flat section at the top
	l_wall_horizontal = i_l_wall - l_slope;
	echo("l_wall_horizontal", l_wall_horizontal);


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

	difference()
	{
		union()
		{
			linear_extrude( i_t_wall,center=true )
			polygon(aan_points);

			
		}
		union()
		{
			//	UPPER HOLE
			//Translate in position
			translate(an_upper_hole)
			//Rotate toward the slope
			rotate([0,0,-i_a_slope+90])
			//Point it toward the structure down
			rotate([90,0,0])
			//This points up
			cylinder(h=100, d=i_d_drill, $fn = 100);

			//	LOWER HOLE
			//Translate in position
			translate(an_lower_hole)
			//Rotate toward the slope
			rotate([0,0,-i_a_slope+90])
			//Point it toward the structure down
			rotate([90,0,0])
			//This points up
			cylinder(h=100, d=i_d_drill, $fn = 100);
			
			
		}

	}


	

	//Drilling is done at 
}


shape_sloped_beam();