include <libs/shape_cylinder.scad>

g_l_lpmu = 146;
g_w_lpmu = 102;
g_li_lpmu_hole = 139;
g_wi_lpmu_hole = 95;

module sbc_lattepanda_mu_lite_board
(
	//Green PCB dimensions
	i_l_pcb = 146,
	i_w_pcb = 102,
	i_t_pcb = 3,
	//Holes interaxis
	i_l_hole = 139, 
	i_w_hole = 95,
	i_d_hole = 3,
	//Ports
	i_l_port = 120,
	i_w_port = 30,
	i_h_port = 20,
	//SoC
	i_l_soc = 70,
	i_w_soc = 60,
	i_h_soc = 60,

	dummy
)
{
	translate
	([
		-i_l_pcb / 2,
		-i_w_pcb / 2,
		0
	])
	difference()
	{
		//Positive geometry
		union()
		{
			//PCB
			color("#00ff00")
			cube
			([
				i_l_pcb,
				i_w_pcb,
				i_t_pcb
			]);

			//ports
			color("#333333")
			translate
			([
				i_l_pcb / 2 - i_l_port / 2,
				0,
				0
			])
			cube
			([
				i_l_port,
				i_w_port,
				i_h_port
			]);

			//SoC
			color("#333333")
			translate
			([
				i_l_pcb -i_l_soc -10,
				i_w_pcb -i_w_soc -0,
				0
			])
			cube
			([
				i_l_soc,
				i_w_soc,
				i_h_soc
			]);

		} //Positive geometry
		//Negative Geometry
		union()
		{
			//PCB drill
			for (ol = [-i_l_hole/2, i_l_hole/2])
			for (ow = [-i_w_hole/2, i_w_hole/2])
			{
				translate
				([
					i_l_pcb/2 + ol,
					i_w_pcb/2 + ow,
					0
				])
				shape_cylinder
				(
					i_d = i_d_hole,
					i_h = i_t_pcb,
					i_e = 0.01
				);
			}

		} //Negative Geometry
	}	//Difference
}

if (false)
sbc_lattepanda_mu_lite_board();