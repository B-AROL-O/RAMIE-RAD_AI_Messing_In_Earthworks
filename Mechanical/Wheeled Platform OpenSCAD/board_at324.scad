//Board outer dimensions
gc_l_at324 = 80.0;
gc_w_at324 = 45.0;
gc_t_at324 = 10.0;
//The LCD board is above and aligned
gc_w_at324_lcd = 36.0;
gc_t_at324_lcd = 35.0;
//Board Hole interaxis
gc_d_at324_hole = 2.5+0.0;
gc_li_at324_hole = 75.0;
gc_wi_at324_hole = 31.0;

module at324_board
(
	//PCB Connectors Components
	i_l_board = gc_l_at324,
	i_w_board = gc_w_at324,
	i_t_board = gc_t_at324,
	//LCD and separation
	i_w_lcd = gc_w_at324_lcd,
	i_t_lcd = gc_t_at324_lcd,
	//Hole diameter
	i_d_hole = gc_d_at324_hole,
	//Hole interaxis
	i_li_hole = gc_li_at324_hole,
	i_wi_hole = gc_wi_at324_hole
)
{

	difference()
	{
		union()
		{
			//PCB Connectors Components
			color("#00ff00")
			translate([-i_l_board/2,-i_w_board/2,0])
			cube
			([
				i_l_board,
				i_w_board,
				i_t_board
			]);
			//LCD and separation
			color("#00ffff")
			translate([-i_l_board/2,-i_w_board/2,0])
			translate([0,0,i_t_board])
			cube
			([
				i_l_board,
				i_w_lcd,
				i_t_lcd-i_t_board
			]);
		}
		union()
		{
			for (lo_temp=[-i_li_hole/2,i_li_hole/2])
			for (wo_temp=[-i_wi_hole/2,i_wi_hole/2])
			{
				translate
				([
					lo_temp,	
					wo_temp - (i_w_board-i_w_lcd) / 2,
					0
				])
				cylinder
				(
					d = i_d_hole,
					h = i_t_board +i_t_lcd,
					$fn = 50
				);
			}

		}
	}
}

if (false)
at324_board();
