//Number of points to achieve a target error
include <function_circle_error.scad>
include <shape_rounded_rectangle.scad>

module shape_rounded_rectangle_tub
(
	//Dimensions of the rectangle
	i_l,
	i_w,
	i_h,
	i_t_base,
	i_t_wall,
	//Rounding of the corners in the XY direction
	i_r_rounding,
	//Error by the approximation
	i_e_precision
)
{
	difference()
	{
		union()
		{
			shape_rounded_rectangle
			(
				//Dimensions of the rectangle
				i_l,
				i_w,
				i_h,
				//Rounding of the corners in the XY direction
				i_r_rounding,
				//Error by the approximation
				i_e_precision
			);
		}
		union()
		{
			translate([0,0,i_t_base])
			shape_rounded_rectangle
			(
				//Dimensions of the rectangle
				i_l - i_t_wall,
				i_w - i_t_wall,
				i_h -i_t_base,
				//Rounding of the corners in the XY direction
				i_r_rounding,
				//Error by the approximation
				i_e_precision
			);
		}
	}
}

if (false)
shape_rounded_rectangle_tub
(
	//Dimensions of the rectangle
	i_l = 50,
	i_w = 30,
	i_h = 20,
	i_t_base = 2,
	i_t_wall = 1,
	//Rounding of the corners in the XY direction
	i_r_rounding = 5,
	//Error by the approximation
	i_e_precision = 0.01
);