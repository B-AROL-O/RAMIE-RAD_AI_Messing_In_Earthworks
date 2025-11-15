//		2025-09-11
//	Encapsulated functions in a convenient library

//This function computes the number of segments to achieve a maximum distance from the side of a regular polygon of N sides to the circle. It's meant to compute the minimum segments to achieve a target resolution with a shape.
fn_num_side_to_achieve_error = function( ir, in_error ) (180/(acos(1-in_error/ir)));


fn_min_round = function( in, in_min ) max( ceil(in), in_min ); 

fn_num_side_to_achieve_error_round = function( ir, in_error, in_min ) fn_min_round(fn_num_side_to_achieve_error(ir, in_error), in_min);
