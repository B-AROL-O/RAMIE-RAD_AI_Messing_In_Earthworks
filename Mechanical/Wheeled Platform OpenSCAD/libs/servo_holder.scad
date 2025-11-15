//Model of the servo
include <hs422-servo.scad>

//translate([0,0,10+20])
//HS422();


//This is a wide pillar that houses one side of the servo
module servo_pillar_full
(
	i_l_base = 40,
	i_h_base = 10,
	i_l_top = 20,
	i_h_top = 60,
	i_t = 15
)
{

	//Outline of the pillar
	ap_pillar = 
	[
		//X out, Y up
		//Origin
		[0,0],
		//Out to base
		[i_l_base,0],
		//Up to straight 
		[i_l_base,i_h_base],
		//slope
		[i_l_top, i_h_top],
		//in and close
		[0, i_h_top]
	];
	translate([0,i_t,0])
	rotate([90,0,0])
	linear_extrude(h=i_t)
	polygon(ap_pillar);


}

//servo_pillar_full();


//I extrude from the pillar a cutout for the flange, and the holes for the screw
module servo_pillar
(
	//Pillar settings
	i_l_base = 30,
	i_h_base = 10,
	i_l_top = 20,
	i_t = 15,
	//FlangeCutout
	i_l_cutout = 8,
	i_h_cutout = gw_hs422,
	i_t_cutout = gh_hs422_flange,
	//Offset height of the servo
	iho_cutout = 20,
	//Margin from front of the pillar to bearing
	//Margin to wheels and other structs
	i_lm_pillar_wheel = 2,
	//How far is the cutout from the front of the pillar
	//This is limited by the hex and the flange
	i_lm_cutout = 8,
	//Margin to add to the cutout across all dimensions
	i_m_cutout = 1,
	//Holes
	i_d_hole = 3.4,
	//The M3 hex nut has 6mm of outer diameter
	i_d_nut = 6.5,
	//Distance of hole from boundary
	i_wm_hole = 3.0,
	//Vertical interaxis between holes
	i_hi_hole_interaxis = gwi_hs422_hole

)
{

	lm_cutout = i_lm_cutout - i_lm_pillar_wheel;

	translate([i_lm_pillar_wheel,0,0])
	difference()
	{

		union()
		{
			servo_pillar_full
			(
				i_l_base = i_l_base,
				i_h_base = i_h_base,
				i_l_top = i_l_top,
				i_h_top = iho_cutout+i_h_cutout+i_m_cutout,
				i_t = i_t
			);
		}
		union()
		{
			l_hole = 20;
			//Cutout for the flange, it slots in
			translate([lm_cutout,0,iho_cutout])
			cube([i_t_cutout+i_m_cutout,i_l_cutout+i_m_cutout,i_h_cutout+i_m_cutout]);

			//Bottom Hole
			translate([0,i_wm_hole,iho_cutout+i_h_cutout/2-i_hi_hole_interaxis/2])
			rotate([0,90,0])
			cylinder(d=i_d_hole,h=l_hole,$fn=20);
			//Bottom Nut
			translate([0,i_wm_hole,iho_cutout+i_h_cutout/2-i_hi_hole_interaxis/2])
			rotate([0,90,0])
			cylinder(d=i_d_nut,h=3,$fn=6);
			//Bottom Head
			translate([i_l_top,i_wm_hole,iho_cutout+i_h_cutout/2-i_hi_hole_interaxis/2])
			rotate([0,90,0])
			cylinder(d=i_d_nut,h=l_hole,$fn=20);

			//Top Hole
			translate([0,i_wm_hole,iho_cutout+i_h_cutout/2+i_hi_hole_interaxis/2])
			rotate([0,90,0])
			cylinder(d=i_d_hole,h=l_hole,$fn=20);
			//Top Nut
			translate([0,i_wm_hole,iho_cutout+i_h_cutout/2+i_hi_hole_interaxis/2])
			rotate([0,90,0])
			cylinder(d=i_d_nut,h=3,$fn=6);
			//Top Head
			translate([i_l_top,i_wm_hole,iho_cutout+i_h_cutout/2+i_hi_hole_interaxis/2])
			rotate([0,90,0])
			cylinder(d=i_d_nut,h=l_hole,$fn=20);

		}
	}
}

module servo_holder
(
	//Holder
	i_li_hole = 49.5,
	//interaxis between pillars
	i_l_servo = gl_hs422_base,
	//Margin for the servo size
	i_lm_servo = 0.5,

	i_ho_servo = 20,
	//Wheel
	i_d_wheel = 60,
	i_t_wheel = 7,
	//The hub is asymmetric, it can be near the left or right pillar
	i_x_right = false,
	//Visualize components
	i_x_show_servo = false,
	i_x_show_wheel = false,
	i_e_precision = 0.01
)
{
	//Align pillars with the hole, two configurations
	lo_pillar = i_x_right?-9.6:9.6;
	echo("Offset: ",lo_pillar);

	l_servo = i_l_servo + i_lm_servo;


	if (i_x_show_servo==true)
	{
		translate([0,-9.5*0,gw_hs422/2+i_ho_servo])
		rotate([0,0,180])
		HS422_wheel
		(
			//Parameters of the wheel
			i_d_wheel = i_d_wheel,
			i_t_wheel = i_t_wheel,
			i_x_right = i_x_right,
			i_x_show_wheel = i_x_show_wheel,
			i_e_precision = i_e_precision
		);
	}

	translate([0,lo_pillar + l_servo / 2,0])
	servo_pillar
	(
		i_wm_hole = (i_li_hole-i_l_servo-i_lm_servo) / 2,
		iho_cutout = i_ho_servo
	);

	translate([0,lo_pillar - l_servo / 2,0])
	mirror([0,1,0])
	servo_pillar
	(
		i_wm_hole = (i_li_hole-i_l_servo-i_lm_servo) / 2,
		iho_cutout = i_ho_servo
	);
}

if (false)
servo_holder
(
	i_x_right = false,
	i_x_show_servo=true,
	i_x_show_wheel=true
);
