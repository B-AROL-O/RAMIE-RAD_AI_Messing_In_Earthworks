//Number of points to achieve a target error
include <function_circle_error.scad>

module shape_rounded_rectangle
(
	//Dimensions of the rectangle
	i_l,
	i_w,
	i_h,
	//Rounding of the corners in the XY direction
	i_r_rounding,
	//Error by the approximation
	i_n_error
)
{
	//Compute points for the rounding
	n_point = fn_num_side_to_achieve_error_round(i_r_rounding, i_n_error, 6);
	echo("Points for rounding: ", n_point );

	//I have two ways of doing it.
	//1) do a profile and a polygon
	//2) use two rectangles and four circles

	l_w_rectangle = i_l - 2 * i_r_rounding;
	w_l_rectangle = i_w - 2 * i_r_rounding;
	
	if ((l_w_rectangle <= 0) || (w_l_rectangle<=0))
		echo("ERR shape_rounded_rectangle too round L: ", l_w_rectangle, " | W ", w_l_rectangle );
	else
	{

		linear_extrude(i_h)
		union()
		{
			square([i_l,w_l_rectangle], center=true);
			square([l_w_rectangle,i_w], center=true);
			
			for (l_offset = [-l_w_rectangle/2,l_w_rectangle/2])
			for (w_offset = [-w_l_rectangle/2,w_l_rectangle/2])
			
				translate([l_offset,w_offset,0])
				circle(r=i_r_rounding,$fn=n_point);

		}


	}

}

if (false)
shape_rounded_rectangle
(
	2,
	5,
	1,
	0.6,
	0.01
);