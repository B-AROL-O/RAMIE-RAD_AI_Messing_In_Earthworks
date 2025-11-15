//Primitives
include <libs/shape_rounded_rectangle.scad>

include <libs/shape_rounded_rectangle_tub.scad>

include <libs/shape_donut_square.scad>

include <libs/shape_cylinder.scad>

include <libs/shape_hexagon.scad>

//Model of the Rasperry Pi 3 and 5
//include <libs/raspberry_pi_3.scad>
include <libs/sbc_support.scad>

include <pcb_regulator.scad>

//Model of the servo
//include <libs/hs422-servo.scad>
include <libs/servo_holder.scad>

//Holder for a pivot tennis ball
include <libs/ball_holder.scad>

include <battery-18650-holder.scad>

include <sbc_lattepanda_mu_lite_board.scad>


module ellipse
(
	x_r=5,
	y_r=10,
	i_n_points = 100
)
{
    ap_ellipse =
	[
        for (n_index = [0 : i_n_points - 1])
            let (a_tempe = 360 * n_index / i_n_points)
			[x_r * cos(a_tempe), y_r * sin(a_tempe)]
    ];
    polygon(ap_ellipse);
}

module industrious_resonance
(
	i_x_show_sbc = false,
	i_x_show_servo = false,
	i_x_show_battery = false,
	i_x_show_pivot = false,
	//Thickness of the base
	i_t_base = 2.0,
	//Precision
	i_e_precision = 0.01
)
{
	//------------------------------------------------------------------
	//	BATTERY
	//------------------------------------------------------------------

	//Battery Offset
	lo_battery = -13;
	wo_battery = 0;
	//Battery dimensions
	d_battery = 18.4+0.5;
	l_battery = 71.0;
	//Battery Holder structural dimensions
	t_battery_cap_wall = 2.0;
	t_battery_cap = 4.0;
	l_spring_loaded = 3.0;

	//------------------------------------------------------------------
	//	BASE
	//------------------------------------------------------------------

	//Base Dimension
	c_l_base = 220.0;
	c_w_base = 2 * l_battery + 2 * t_battery_cap + l_spring_loaded;
	c_h_base = 15.0;
	c_r_base = 60.0;

	echo("Dimension L: ", c_l_base, " | W: ", c_w_base );

	//BASE parameters
	t_base = i_t_base;

	//------------------------------------------------------------------
	//	PIVOT
	//------------------------------------------------------------------

	//Offset of the pivot wheel
	lo_pivot = -80;
	//This is a number to control anchor between pivot mechanism and its base
	//I can't be bothered to work out the angles with the arcsin to make it work without this parameter
	ho_pivot = 10;
	//Diameter of the hole where I'll stot in the pivot wheel mechanism
	d_pivot_cutout = 60;
	//Diameter of the pivot sphere (a tennis ball I had laying around)
	d_pivot_sphere = 40.0;
	//Clearance under the base
	h_pivot_clearance = d_pivot_sphere / 2 - ho_pivot;

	echo("Pivot ball clearance: ",h_pivot_clearance);

	//------------------------------------------------------------------
	//	WHEELS
	//------------------------------------------------------------------

	//Position of the motors on the base
	l_wheel = 49.0;
	w_wheel = 55.0;

	//Specs of the wheels
	d_wheel = 68.0;
	t_wheel = 8.0;
	//Margin to apply to the wheel hole
	wm_wheel = -1.0;
	tm_wheel = 2.5;
	dm_wheel = -5.0;
	//Parameters to adjust the relative position of wheel and servo
	wo_wheel = gh_hs422_flange / 2;

	//Height of the servo from floor of pillar
	//Wheel radious minus half servo thickness minus base thickness MINUS pivot height from floor clearance
	//
	h_floor_servo = d_wheel / 2 - 10 - t_base - h_pivot_clearance;

	//------------------------------------------------------------------
	//	SBC
	//------------------------------------------------------------------

	//SBC Offset Position
	lo_sbc = 23.0;
	wo_sbc = 0.0;

	//Height of the SBC from the top of the base
	t_sbc = 35;

	//HOLE
	li_sbc_hole = g_li_lpmu_hole;
	wi_sbc_hole = g_wi_lpmu_hole;



	d_hole = 3.0 + 0.5;

	d_sbc_nut = 6.0 + 0.5;

	d_sbc_support_top = 6.0;
	d_sbc_support_base = 8.0;
	
	//------------------------------------------------------------------
	//	PCB
	//------------------------------------------------------------------

	///	Regulator
	//Size
	l_regulator = 55.0;
	w_regulator = 30.0;
	h_regulator = 20;
	//Holes
	li_regulator_hole = 50.0;
	wi_regulator_hole = 25.0;
	//Position Offset on the robot
	lo_regulator = 50.0;
	wo_regulator = 0.0;
	ho_regulator = 10.0;

	//------------------------------------------------------------------
	//	SWITCH
	//------------------------------------------------------------------

	d_switch_hole = 6.0 + 1.0;

	//------------------------------------------------------------------
	//	GEOMETRY
	//------------------------------------------------------------------

	difference()
	{
		union()
		{
			//---------------------------------------------------------------------
			// BASE
			//---------------------------------------------------------------------

			if (false)
			color("#888888")
			shape_rounded_rectangle
			(
				//Dimensions of the rectangle
				i_l = c_l_base,
				i_w = c_w_base,
				i_h = i_t_base,
				//Rounding of the corners in the XY direction
				i_r_rounding = c_r_base,
				//Error by the approximation
				i_n_error = i_e_precision
			);

			color("#888888")
			shape_rounded_rectangle_tub
			(
				//Dimensions of the rectangle
				i_l = c_l_base,
				i_w = c_w_base,
				i_h = c_h_base,
				i_t_base = i_t_base,
				i_t_wall = i_t_base,
				//Rounding of the corners in the XY direction
				i_r_rounding = c_r_base,
				//Error by the approximation
				i_e_precision = i_e_precision
			);

			//---------------------------------------------------------------------
			// SBC
			//---------------------------------------------------------------------

			translate
			([
				lo_sbc,
				wo_sbc,
				t_base
			])
			union()
			{
				//SBC BOARD
				if (i_x_show_sbc == true)
				{
					translate
					([
						0,
						0,
						t_sbc
					])
					rotate([0,0,180])
					sbc_lattepanda_mu_lite_board();
				}

				for (lo_temp = [-li_sbc_hole/2,li_sbc_hole/2])
				for (wo_temp = [-wi_sbc_hole/2,wi_sbc_hole/2])
				{
					translate
					([
						lo_temp,
						wo_temp,
						0
					])
					solid_pillar
					(
						//Top diameter
						i_d_top = d_sbc_support_top,
						//Bottom diameter
						i_d_base = d_sbc_support_base,
						//Height of the pillar
						i_h_pillar = t_sbc,
						//Straight section at top and bottom
						i_h_vertical = 5,
						//Precision
						i_e_precision = i_e_precision
					);
				}
			}	//SBC SUPPORT

			translate
			([
				lo_regulator,
				wo_regulator,
				t_base
			])
			union()
			{
				if (i_x_show_sbc == true)
				{
					//REGULATOR
					translate([0,0,ho_regulator])
					shape_pcb
					(
						//Size
						i_l = l_regulator,
						i_w = w_regulator,
						i_h = h_regulator,
						//Hole
						i_d = 3.0 + 0.5,
						i_li = li_regulator_hole,
						i_wi = wi_regulator_hole,
						
						i_e_precision = i_e_precision
					);
				}

				for (lo_temp = [-li_regulator_hole/2,li_regulator_hole/2])
				for (wo_temp = [-wi_regulator_hole/2,wi_regulator_hole/2])
				{
					translate
					([
						lo_temp,
						wo_temp,
						0
					])
					solid_pillar
					(
						//Top diameter
						i_d_top = d_sbc_support_top,
						//Bottom diameter
						i_d_base = d_sbc_support_base,
						//Height of the pillar
						i_h_pillar = ho_regulator,
						//Straight section at top and bottom
						i_h_vertical = h_regulator/8,
						//Precision
						i_e_precision = i_e_precision
					);
				}
			}

			//---------------------------------------------------------------------
			//	SERVO WHEEL
			//---------------------------------------------------------------------

			//Right Wheel
			translate
			([
				l_wheel,
				-w_wheel,
				i_t_base
			])
			rotate([0,0,90])
			servo_holder
			(
				//Height of the servo from floor of pillar
				i_ho_servo = h_floor_servo,
				//Wheel
				i_d_wheel = d_wheel,
				i_t_wheel = t_wheel,
				//Visualize components
				i_x_right = true,
				i_x_show_servo = i_x_show_servo,
				i_x_show_wheel = i_x_show_servo
			);

			//Left Wheel
			translate
			([
				l_wheel,
				+w_wheel,
				i_t_base
			])
			rotate([0,0,-90])
			servo_holder
			(
				//Height of the servo from floor of pillar
				i_ho_servo = h_floor_servo,
				//Wheel
				i_d_wheel = d_wheel,
				i_t_wheel = t_wheel,
				//Visualize components
				i_x_right = false,
				i_x_show_servo = i_x_show_servo,
				i_x_show_wheel = i_x_show_servo
			);

		}
		//Extrude
		union()
		{
			//Right Wheel Hole
			translate
			([
				l_wheel,
				-w_wheel - wm_wheel - (t_wheel + tm_wheel) * 0.5,
				0
			])
			shape_rounded_rectangle
			(
				//Dimensions of the rectangle
				i_l = d_wheel + dm_wheel,
				i_w = t_wheel + tm_wheel,
				i_h = i_t_base,
				//Rounding of the corners in the XY direction
				i_r_rounding = 2,
				//Error by the approximation
				i_n_error = i_e_precision
			);

			//Left Wheel Hole
			translate
			([
				l_wheel,
				+w_wheel + wm_wheel + (t_wheel + tm_wheel) * 0.5,
				0
			])
			shape_rounded_rectangle
			(
				//Dimensions of the rectangle
				i_l = d_wheel+dm_wheel,
				i_w = t_wheel+tm_wheel,
				i_h = i_t_base,
				//Rounding of the corners in the XY direction
				i_r_rounding = 2,
				//Error by the approximation
				i_n_error = i_e_precision
			);

			//An hole where I'll slot in the pivot wheel
			translate([lo_pivot,0,0])
			cylinder(h=i_t_base,d=d_pivot_cutout, $fn=80);
	
			//Hole to access the batteries from below
			translate([lo_battery,0,0])
			linear_extrude(t_base)
			square([d_battery * 3.0,c_w_base-t_battery_cap_wall*2],center=true);

			//---------------------------------------------------------------------
			// SBC HOLE
			//---------------------------------------------------------------------

			translate
			([
				lo_sbc,
				wo_sbc,
				0
			])
			union()
			{
				for (lo_temp = [-li_sbc_hole/2,li_sbc_hole/2])
				for (wo_temp = [-wi_sbc_hole/2,wi_sbc_hole/2])
				{
					translate
					([
						lo_temp,
						wo_temp,
						0
					])
					shape_cylinder
					(
						i_d = d_hole,
						i_h = t_base+t_sbc,
						i_e = i_e_precision
					);

					translate
					([
						lo_temp,
						wo_temp,
						0
					])
					shape_hexagon
					(
						i_d = d_sbc_nut,
						i_h = 2.5
					);
				}
			}	//SBC SUPPORT

			//REGULATOR HOLE AND NUT
			translate
			([
				lo_regulator,
				wo_regulator,
				0
			])
			union()
			{
				for (lo_temp = [-li_regulator_hole/2,li_regulator_hole/2])
				for (wo_temp = [-wi_regulator_hole/2,wi_regulator_hole/2])
				{
					translate
					([
						lo_temp,
						wo_temp,
						0
					])
					shape_cylinder
					(
						i_d = d_hole,
						i_h = t_base+ho_regulator,
						i_e = i_e_precision
					);

					translate
					([
						lo_temp,
						wo_temp,
						0
					])
					shape_hexagon
					(
						i_d = d_sbc_nut,
						i_h = 2.5
					);
				}
			}	//REGULATOR HOLE AND NUT

			//---------------------------------------------------------------------
			// SWITCH HOLE
			//---------------------------------------------------------------------

			translate
			([
				-47,
				-c_w_base/2 +t_base,
				i_t_base + d_switch_hole/2 + 3
			])
			rotate([90,0,0])
			shape_cylinder
			(
				i_d = d_switch_hole,
				i_h = t_base,
				i_e = i_e_precision
			);


		}	//Geometry difference
	}	//Geometry

	//---------------------------------------------------------------------
	//	PIVOT
	//---------------------------------------------------------------------

	translate([lo_pivot,0,0])
	ball_holder
	(
		//Ball
		i_d_ball = 40.0,
		i_d_base = d_pivot_cutout,
		//Structural Strength
		i_t_structure = 4.0,
		i_t_hold = 3.0,
		//Height margin of the base
		i_ho_base = ho_pivot,
		//Thickness of the base
		i_t_base = i_t_base,
		//Show ball
		i_x_show_ball = i_x_show_pivot
	);

	//---------------------------------------------------------------------
	// BATTERY HOLDER
	//---------------------------------------------------------------------

	translate
	([
		lo_battery,
		wo_battery,
		0
	])
	rotate([0,0,90])
	holder_18650_2s2p_inverted
	(
		//Diameter of the 18650 battery plus tollerance
		id_18650 = d_battery,
		//Length of the 18650 battery from base to button
		il_18650 = l_battery,
		//Thickness of the Holder walls
		it_wall = t_base,
		//Thickness of the cap plus rails
		it_cap = t_battery_cap,
		//Show non printable elements
		ix_show_battery = i_x_show_battery,
		ix_show_tab = i_x_show_battery
	);


}

//industrious_resonance();

//if (false)
industrious_resonance
(
	i_x_show_sbc = true,
	i_x_show_battery = true,
	i_x_show_servo = true,
	i_x_show_pivot = true,
	i_e_precision = 0.01
);