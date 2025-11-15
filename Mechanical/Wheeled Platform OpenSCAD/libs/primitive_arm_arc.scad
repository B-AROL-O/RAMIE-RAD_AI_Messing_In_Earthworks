//	ORSO ERIC
//	2025-08-27


/**
 * arm_arc - Creates a thick arc (arm segment) with optional horizontal foot.
 *
 * This module generates a 2D arc (like an arm segment) with inner and outer radii,
 * defined start and end angles, and an optional horizontal foot for stability.
 * The arc can be extruded to 3D using linear_extrude.
 *
 * Parameters:
 *   i_r_inner: Inner radius of the arc (default: 20)
 *   i_r_outer: Outer radius of the arc (default: 40)
 *   i_a_arc_start: Start angle of the arc in degrees (default: 90)
 *   i_a_arc_end: End angle of the arc in degrees (default: 10)
 *   i_t: Thickness (height) of the extruded arc (default: 10)
 *   i_n_points: Number of points used to approximate the arc (default: 10)
 *
 * Notes:
 *   - Angles are measured counterclockwise from the positive X-axis.
 *   - If i_a_arc_end <= 0, the foot is horizontal at the bottom.
 *   - The arc is always drawn from start to end angle.
 */
module arm_arc
(
	i_r_inner = 20,   // Inner radius of the arc
    i_r_outer = 40,   // Outer radius of the arc
    i_a_arc_start = 90, // Start angle in degrees
    i_a_arc_end = -90,   // End angle in degrees
    i_t = 10,          // Thickness (height) of the extruded arc
    i_n_points = 10    // Number of points for arc approximation
)
{
	ap_arc_inner =
	[
		for (n_index = [0:i_n_points])
		[
            i_r_inner * cos(i_a_arc_start + (i_a_arc_end - i_a_arc_start) * n_index / i_n_points),
            i_r_inner * sin(i_a_arc_start + (i_a_arc_end - i_a_arc_start) * n_index / i_n_points)
        ]
	];

	ap_arc_outer = 
	[
		for (n_index = [i_n_points:-1:0])
		[
            i_r_outer * cos(i_a_arc_start + (i_a_arc_end - i_a_arc_start) * n_index / i_n_points),
            i_r_outer * sin(i_a_arc_start + (i_a_arc_end - i_a_arc_start) * n_index / i_n_points)
        ]

	];



    //Compute point to make foot Horizontal
    p_horizontal = (i_a_arc_end <= 0) ?
		//With angle < 0, the inner ends early. So i take the last Y of the inner, and the first X of the outer
		[[ap_arc_inner[i_n_points][0],ap_arc_outer[0][1]]] :
		//With angle > 0, the outer ends early. So i take the last Y of the inner, and the first X of the outer
		[[ap_arc_outer[0][0],ap_arc_inner[i_n_points][1]]];

	ap_arc = concat
	(
		ap_arc_inner,
		p_horizontal,
		ap_arc_outer
	);

	
	//echo(ap_arc);
	rotate([90,0,0])
	linear_extrude(h=i_t,center=true)
	polygon(ap_arc);

}
if(false)
{
	rotate([90,0,0])
	circle(d=40-0.5,$fn=100);
	arm_arc(i_n_points=100);
}