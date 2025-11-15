/**
 * @file shape_truncated_cone_rounded.scad
 * @brief Module for generating a truncated cone with rounded edges.
 */

include <function_circle_error.scad>

//At higher angles, I need to account for the rounding that changes height otherwise the side will not be continuous with the rounding
//I need to define what an edge is
//A<90°
//If the rounding is shallow, I take the position of the edge as the meeting point between the side and the top/bottom. This is a meeting at half the angle A/2
//>= 90°
//Above 90°, the meeting point would be outside the shape.
//It's better to consider the rouncind circle as the edge position at 90°
// The algorithm calculate the angle of the side.
//If it's exactly 90° both will use the 90° radious
//If one is less than 90°, use half that angle to compute the meeting point
//The other must be more than 90° and the equations use the point at 90° with the excess computed so that the side will be continous
//I also use an equation to compute how many segment the rounding has to achieve a given error in mm
 
/**
 * @brief Generates a truncated cone with specified dimensions and rounding.
 *
 * This module creates a 3D model of a truncated cone with rounded edges at the base and top.
 *
 * @param i_r_bot The radius at the base of the truncated cone.
 * @param i_r_top_delta The difference in radius between the base and the top of the truncated cone.
 * @param i_h The height of the truncated cone.
 * @param i_r_edge_rounding The radius of the rounding at the edges.
 * @param cn_mm_per_segment The resolution of the volume in mm per segment. Default is 0.2.
 *
 * @note The module calculates the slope of the side wall and uses it to create rounded edges.
 * @note The `rotate_extrude` function is used to create the 3D shape from a 2D slice.
 *
 * @code
 * truncated_cone(i_r_bot = 10, i_r_top_delta = -2, i_h = 20);
 * @endcode
 */
 
//Number of segment should be based on error
 
module truncated_cone
(
	//Diameter at the base
	i_r_bot,
    //Diameter at the top, difference. Plus makes top bigger, Minus makes top smaller
	i_r_top_delta,
	//Height of the truncated cone
	i_h,
    //Rounding radious of the edges of the volume.
    //NOTE: rounding must be small compared to height, otherwise is a much more complex geometry requiring lots more calculations to make side continous with the rounded corners
    i_r_edge_rounding,
    //Maximum error from segment to circumpherence
    i_e_precision = 0.01,
    //Show the computed angles and segments
    b_debug = false
)
{
    //minimum segments that will be spawned for rouning
    n_min_segments = 3;
    
    //---------------------------------------------------------------------
    //  STAGE 1 - SIDE SLOPE
    //---------------------------------------------------------------------
    //  I compute the angle of the side, and the angle of top and bottom rounding

    //Compute slope of side wall, one side will always be more than 90°, I take that into account
    a_slope = atan2(i_r_top_delta,i_h-i_r_edge_rounding);
    //Compute arc of the bottom edge
    a_edge_bot = 90.0-a_slope;
    //Compute arc of the top edge
    a_edge_top = 90.0+a_slope;
    if (b_debug)
    echo("Slope of side: ", a_slope, " | Arc for bottom edge: ", a_edge_bot, " | Arc for top edge: ", a_edge_top );
    
    //---------------------------------------------------------------------
    //  STAGE 2 - EDGE CORRECTION
    //---------------------------------------------------------------------
    //  The geometry is more complicated than it seems
    //  One angle will be more than 90°, that angle has the edge computed at 90°
    //  and the rest of the arc computed to match the side
    //  The other angle will be less than 90°, and the meeting point will
    //  be at half the angle, that's where I compute the middle vars

    //@TODO: This sounds like a lot of math, it feels like a system of trascendental equations. And only matters if the rounding radious is comparable in size with the height, if it's a really soft shape. For now I just be lazy and demand rounding is small compared to height.
    
    //It works well for rounding 1 on height 8. Good enough!
 
    //---------------------------------------------------------------------
    //  STAGE 3 - NUMBER OF SEGMENTS
    //--------------------------------------------------------------------- 
    //  I'm being fancy here. Exactly how many segments are needed to achieve
    //  a target error? Knowing radious, error and arc it's easy to compute
    //  This way I generate only as many vertexes as I need

    n_points_rotation = fn_num_side_to_achieve_error_round
	(
		i_r_bot,
		i_e_precision,
		n_min_segments
	);

    if (b_debug)
    echo("Segments for rotation around Z: ", n_points_rotation);
    
    //Compute the segments to achieve target resolution. Since I need an arc, that futher reduces the number of segments needed
    n_points_edge = fn_num_side_to_achieve_error(i_r_edge_rounding,i_e_precision);
    n_points_edge_bot_arc = fn_num_side_to_achieve_error_round
	(
		n_points_edge * a_edge_bot / 360,
		i_e_precision,
		n_min_segments
	);

    n_points_edge_top_arc = fn_num_side_to_achieve_error_round
	(
		n_points_edge * a_edge_top / 360,
		i_e_precision,
		n_min_segments
	);

    if (b_debug)
    echo("Segments for edge rounding: ", n_points_edge, " | Bot: ", n_points_edge_bot_arc, " | Top: ", n_points_edge_top_arc);
    
    //List of points to build the 2D slice of the truncated cone
    //I use two for cycle
	aan_points_rounded = 
	([
		//Bottom
        [0, 0],
		//Scan the lower rounding
        let
        (
            //arc needed to do the rounded corner
            a_arc = a_edge_bot,
            //Points needed to achieve target resolution on rounded corner
            n_points = n_points_edge_bot_arc
        )            
        for (cnt_n=[0:n_points])
            let (a_temp = cnt_n/n_points*a_arc)
            [
                +i_r_bot+i_r_edge_rounding*(sin(a_temp)-1),
                +i_r_edge_rounding*(1-cos(a_temp))
            ],
		//Scan the upper rounding
        let
        (
            //arc needed to do the rounded corner
            a_arc = a_edge_top,
            //Points needed to achieve target resolution on rounded corner
            n_points = n_points_edge_top_arc
        )
		for (cnt_n=[n_points:-1:0])
			let (a_temp = cnt_n/n_points*a_arc)
			[
				+i_r_bot+i_r_top_delta+i_r_edge_rounding*(-1+sin(a_temp)),
				+i_h+i_r_edge_rounding*(-1+cos(a_temp))
			],
		//Top face
        [0, i_h]
    ]);
    //Extrude slice, compute segment to achieve resolution.
    //circumpherence divided by desired mm per segment
	rotate_extrude(angle=360, $fn=n_points_rotation)
	polygon( aan_points_rounded );
}

//EXAMPLE
if (false)
truncated_cone
(
    i_r_bot = 20,
    i_r_top_delta = -10,
    i_h = 30,
    i_r_edge_rounding = 4
);
