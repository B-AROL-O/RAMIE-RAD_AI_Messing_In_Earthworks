include <primitive_pillar.scad>

//M3 hole = 2.75
//M3 nut = 6.5

module sbc_support_pillars
(
	i_d_top = 6,
	i_d_bot = 10,
	i_h_pillar = 20,
	i_h_vertical = 4,
	//Interaxis between holes
	i_li_sbc = 50,
	i_wi_sbc = 30
)
{
	for (lo_temp = [-i_li_sbc/2,i_li_sbc/2])
	{
		for (wo_temp = [-i_wi_sbc/2,i_wi_sbc/2])
		{
			translate([lo_temp,wo_temp,0])
			pillar
			(
				i_d_hole = 2.75,
				i_d_top = i_d_top,
				i_d_base = i_d_bot,
				i_h_slot = 6,
				i_h_pillar = i_h_pillar,
				//Vertical sections
				i_h_vertical = i_h_vertical,
				i_n_resolution = 60,
				//NUT at the base
				i_d_nut = 6.5,
				i_h_nut = 3.0
			);
		}
	}

}

//sbc_support_pillars();