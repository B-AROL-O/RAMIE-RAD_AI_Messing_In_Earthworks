//PCB Dimensions
gl_pi3 = 85.0;
gw_pi3 = 57.0;

//Size of the hole, M2.5 screw
gd_pi3_hole = 2.75;
//Distance from center hole to board margin in length
glm_pi3_hole = 3.5;
//Interaxis between holes in width and length
gli_pi3_hole = 58.0;
gwi_pi3_hole = 49.0;

//PCB
gt_pi3_pcb = 4.5;

//Ethernet Connector 20 width 14 height

gh_pi3_eth = 14.0;
gw_pi3_eth = 20.0;

//USB Connectors 33 width 16 height
gh_pi3_usb = 16.0;
gw_pi3_usb = 31.0;

//Make positive of a vertical raspbnerry pi
module raspberry_pi_3
(

)
{
	difference()
	{
		union()
		{
			//PCB
			color("#00ff00")
			cube
			([
				gl_pi3,
				gw_pi3,
				gt_pi3_pcb
			]);
			//ETH connector
			translate
			([
				gli_pi3_hole +2*glm_pi3_hole,
				2,
				gt_pi3_pcb
			])
			color("#000000")
			cube
			([
				gl_pi3 -gli_pi3_hole -2*glm_pi3_hole,
				gw_pi3_eth,
				gh_pi3_eth
			]);
			//ETH connector
			translate
			([
				gli_pi3_hole +2*glm_pi3_hole,
				gw_pi3-gw_pi3_usb-2,
				gt_pi3_pcb
			])
			color("#000000")
			cube
			([
				gl_pi3 -gli_pi3_hole -2*glm_pi3_hole,
				gw_pi3_usb,
				gh_pi3_usb
			]);

		}
		union()
		{
			//Rear Right
			translate
			([
				glm_pi3_hole,
				gw_pi3/2-gwi_pi3_hole/2,
				0
			])
			cylinder
			(
				h=gt_pi3_pcb,
				d=gd_pi3_hole,
				$fn = 20
			);
			//Rear Left
			translate
			([
				glm_pi3_hole,
				gw_pi3/2+gwi_pi3_hole/2,
				0
			])
			cylinder
			(
				h=gt_pi3_pcb,
				d=gd_pi3_hole,
				$fn = 20
			);
			//Forward Right
			translate
			([
				glm_pi3_hole+gli_pi3_hole,
				gw_pi3/2-gwi_pi3_hole/2,
				0
			])
			cylinder
			(
				h=gt_pi3_pcb,
				d=gd_pi3_hole,
				$fn = 20
			);
			//Forward Left
			translate
			([
				glm_pi3_hole+gli_pi3_hole,
				gw_pi3/2+gwi_pi3_hole/2,
				0
			])
			cylinder
			(
				h=gt_pi3_pcb,
				d=gd_pi3_hole,
				$fn = 20
			);

		}
	}
	

}

//raspberry_pi_3();