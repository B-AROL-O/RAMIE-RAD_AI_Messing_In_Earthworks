/**hexagon
 * 
 *
 * Parameters:
 * - i_d: Diameter of the cylinder. If provided, this will override i_r.
 * - i_h: Height of the cylinder (default = 10).
 */
 
module shape_hexagon
(
	i_d = 0,
	i_h = 10
)
{
	linear_extrude(i_h)
	circle(d = i_d, $fn = 6 );
}

//shape_hexagon(i_d = 20);

if (false)
shape_hexagon
(
	i_d = 20,
	i_h = 10,
);