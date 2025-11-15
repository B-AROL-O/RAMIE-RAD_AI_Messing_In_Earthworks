include <libs/shape_cylinder.scad>

include <libs/shape_rounded_rectangle.scad>


module shape_pcb
(
	//Size
	i_l = 50,
	i_w = 30,
	i_h = 15,
	//Hole
	i_d = 3.0 + 0.5,
	i_li = 40,
	i_wi = 20,
	
	i_e_precision = 0.01
)
{
	//Geometry
	difference()
	{
		//Add geometry
		union()
		{
			color("#00ff00")
			shape_rounded_rectangle
			(
				//Dimensions of the rectangle
				i_l = i_l,
				i_w = i_w,
				i_h = i_h,
				//Rounding of the corners in the XY direction
				i_r_rounding = (i_l + i_w) /20,
				//Error by the approximation
				i_n_error = i_e_precision
			);
		}	//Add geometry

		//Sub geometry
		union()
		{
			for (lo_temp = [-i_li/2,i_li/2])
			for (wo_temp = [-i_wi/2,i_wi/2])
			{
				translate
				([
					lo_temp,
					wo_temp,
					0
				])
				shape_cylinder
				(
					i_d = i_d,
					i_h = i_h,
					i_e = i_e_precision
				);
			}
		}	//Sub geometry
	} //Geometry
}

//shape_pcb();