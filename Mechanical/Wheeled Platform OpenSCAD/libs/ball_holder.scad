//Given a ball of diameter D
//Create an holder with a number of semi sphjeresto let the ball rotate without much attrition
//It should have spring tabs to hold the ball

//I create three arms, each with two semisphere that converge above the ball
//Other three arms go anchor to the base where it's screwed

include <shape_cylinder.scad>

include <primitive_arm_arc.scad>

//Diameter of the ball that needs to be held
gd_ball = 40.0;


//It's an arm arc, but adds cushion spheres to minimize contact area with the ball itself
module arm_arc_with_balls
(
	i_r_inner = 20,   // Inner radius of the arc
    i_r_outer = 40,   // Outer radius of the arc
    i_a_arc_start = 90, // Start angle in degrees
    i_a_arc_end = 10,   // End angle in degrees
    i_t = 10,          // Thickness (height) of the extruded arc
    i_n_points = 10,    // Number of points for arc approximation
	i_d_sphere = 5,		//Diameter of the cushion sphere
	i_n_sphere = 2		//Number of cushion spheres
)
{
	arm_arc
	(
		i_r_inner = i_r_inner,
		i_r_outer = i_r_outer,
		i_a_arc_start = i_a_arc_start, // Start angle in degrees
		i_a_arc_end = i_a_arc_end,   // End angle in degrees
		i_t = i_t,          // Thickness (height) of the extruded arc
		i_n_points = i_n_points    // Number of points for arc approximation
	);
	if(i_n_sphere>1)
	for (n_temp = [1:i_n_sphere-1])
		let(a_sphere = i_a_arc_start +(i_a_arc_end-i_a_arc_start)*n_temp/(i_n_sphere+0))
		translate([
			i_r_inner * cos(a_sphere),
			0,
			i_r_inner * sin(a_sphere)

		])
		sphere(d=i_d_sphere,$fn=100);



	translate([
		i_r_inner * cos(i_a_arc_start +(i_a_arc_end-i_a_arc_start)*0.95),
		0,
        i_r_inner * sin(i_a_arc_start +(i_a_arc_end-i_a_arc_start)*0.95)

	])
	sphere(d=i_d_sphere,$fn=80);
}

//arm_arc_with_balls();


module donut
(
	i_d_internal = 40,
	i_d_external = 60,
	i_h = 5,
	i_e_precision = 0.05
)
{
	difference()
	{
		union()
		{
			//Base
			shape_cylinder
			(
				i_d = i_d_external,
				i_h = i_h,
				i_e = i_e_precision
			);
		}
		union()
		{
			//Drill
			shape_cylinder
			(
				i_d = i_d_internal,
				i_h = i_h,
				i_e = i_e_precision
			);

		}

	}

}

module ball_holder_base
(
	i_d_hole = 41,
	i_d_external = 65,

	i_h_base = 5,
	
	//Number of sectors cut into the base for the holders
	i_n_sector = 3,
	//Margin separation between sector and base, game of the arm
	i_m_sector = 1,
	//Size of the sector
	i_d_sector = 30,
	//Interference between sector and base, it's an arc
	i_di_sector = 10,

	i_e_precision = 0.05
)
{

	difference()
	{
		union()
		{
			donut
			(
				i_d_hole,
				i_d_external,
				i_h_base,
				i_e_precision
			);
		}
		union()
		{
			//Sector
			for (n_cnt = [0:(i_n_sector-1)])
			{
				translate
				([
					(i_d_hole/2 -i_d_sector/2 +i_di_sector/2)*cos(n_cnt/(i_n_sector)*360),
					(i_d_hole/2 -i_d_sector/2 +i_di_sector/2)*sin(n_cnt/(i_n_sector)*360),
					0
				])
				donut
				(
					i_d_sector,
					i_d_sector+i_m_sector,
					5,
					i_e_precision
				);
			}
			
		}

	}


}

//ball_holder_base();

module ball_holder
(
	//Ball
	i_d_ball = 40.0,
	i_d_base = 60.0,
	//Structural Strength
	i_t_structure = 4.0,
	i_t_hold = 3.0,
	//Height margin of the base
	i_ho_base = 10,
	//Thickness of the base
	i_t_base = 2,

	i_x_show_ball = true,
	//Error
	i_e_precision = 0.01
)
{
	//How much bigger are the structural arm in diameter
	dm_struct_hold = 3.0;

	//Diameter of the cushion spheres
	d_cushion = 6;
	//Interference between the ball and the cushion
	i_ball_cushion = 0.0;

	//The structural arm have a bigger radious
	d_arm_struct = i_d_ball + dm_struct_hold;
	w_arm_struct = 22;
	t_arm_struct = i_t_structure;

	d_arm_hold = i_d_ball;
	w_arm_hold = 12;
	t_arm_hold = i_t_hold;

	t_base = 2;

	n_resolution = 100;
	
	ball_holder_base
	(
		i_d_hole = i_d_ball+1,
		i_d_external = i_d_base,

		i_h_base = i_t_base,
		//Number of sectors cut into the base for the holders
		i_n_sector = 3,
		//Margin separation between sector and base, game of the arm
		i_m_sector = 2,
		//Size of the sector
		i_d_sector = 20,
		//Interference between sector and base, it's an arc
		i_di_sector = 15,
		i_e_precision = i_e_precision
	);

	//Add a cap to smooth out the merging of the arms
	translate([0,0,d_arm_struct/2+i_ho_base])
	shape_cylinder
	(
		i_d = 0.7*i_d_ball,
		i_h = t_arm_struct*1.2,
		i_e = i_e_precision
	);

	//Structural Arms
	translate([0,0,i_ho_base])
	for (a_ray = [0+60,120+60,240+60])
	{
		rotate([0,0,a_ray])
		arm_arc
		(
			i_r_inner = d_arm_struct/2,   // Inner radius of the arc
			i_r_outer = d_arm_struct/2+t_arm_struct,   // Outer radius of the arc
			i_a_arc_start = 90, // Start angle in degrees
			i_a_arc_end = -19,   // End angle in degrees
			i_t = w_arm_struct,          // Thickness (height) of the extruded arc
			i_n_points = n_resolution    // Number of points for arc approximation
		);
	}


	translate([0,0,i_ho_base])
	for (a_ray = [0,120,240])
	{
		rotate([0,0,a_ray])
		arm_arc_with_balls
		(
			i_r_inner = d_arm_hold/2+d_cushion/2-i_ball_cushion,   // Inner radius of the arc
			i_r_outer = d_arm_hold/2+d_cushion/2-i_ball_cushion+t_arm_hold,   // Outer radius of the arc
			i_a_arc_start = 90, // Start angle in degrees
			i_a_arc_end = -21,   // End angle in degrees
			i_t = w_arm_hold,          // Thickness (height) of the extruded arc
			i_n_points = n_resolution,    // Number of points for arc approximation
			i_d_sphere = d_cushion,		//Diameter of the cushion sphere
			i_n_sphere = 3		//Number of cushion spheres
		);
	}

	if (i_x_show_ball == true)
	{
		color("#ffffff")
		translate([0,0,i_ho_base])
		sphere(d=gd_ball,$fn=100);
	}

	echo("Margin floor to bottom of base: ", gd_ball/2-i_ho_base );
}


if (false)
ball_holder();
