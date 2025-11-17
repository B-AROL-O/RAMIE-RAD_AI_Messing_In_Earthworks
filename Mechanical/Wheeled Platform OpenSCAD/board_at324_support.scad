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
	//Depth of the wall
	i_l_wall = 50,
	//Thickness of the wall
	i_t_wall = 5,
	//Height of the wall
	i_h_wall = 70,
	//Flat section on top of the pillar
	i_l_wall_top = 20,

	//Angle of the slope
	i_a_slope = 45,


	dummy
)
{

	//Length of the slope
	l_slope = i_l_wall -i_l_wall_top;
	//I need to compute the height of the slope
	//It's some trigonometry
	h_slope = l_slope * tan(i_a_slope);

	aan_points = 
	[
		//X = Length Y = Height
		[0,0],
		[i_l_wall,0],
		
		[i_l_wall,i_h_wall],
		//Horizontal section leading to the slope
		[i_l_wall_top,i_h_wall],
		//Vertical section after the slope
		[0,i_h_wall-h_slope]


	];


	linear_extrude( i_t_wall )
	polygon(aan_points);
}


shape_sloped_beam();