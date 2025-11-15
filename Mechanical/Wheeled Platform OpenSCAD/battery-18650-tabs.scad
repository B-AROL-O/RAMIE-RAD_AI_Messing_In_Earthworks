// Include necessary modules for shapes
include <libs/shape_cylinder.scad>;
include <libs/shape_truncated_cone_rounded.scad>;

//--------------------------------------------------------------------------------
//	Battery Spring Tab
//--------------------------------------------------------------------------------

/**
 * Module: battery_18650_tab_spring
 *
 * This module creates a 3D model of a spring tab for an 18650 battery. It includes 
 * features like metal plate, locking mechanisms, and holes for electrical contacts.
 *
 * https://it.rs-online.com/web/p/contatti-per-batterie/3247303
 *
 * Parameters:
 * - i_l_plate (number): Length of the metal plate.
 * - i_w_plate (number): Width of the metal plate.
 * - i_t_plate (number): Thickness of the metal plate.
 * - i_l_lock (number): Length of the locking feature.
 * - i_w_lock (number): Width of the locking feature.
 * - i_h_lock (number): Height of the locking feature.
 * - i_d_hole (number): Diameter of holes for electrical contacts.
 * - i_wi_hole (number): Interaxis between holes.
 * - i_h_tab (number): Total height with spring.
 * - i_d_base (number): Base diameter of the spring cone.
 * - i_d_tip (number): Tip diameter of the spring cone.
 * - i_e_precision (number): Precision for circle approximations.
 */
module battery_18650_tab_spring(
    i_l_plate = 11.5,
    i_w_plate = 11.5,
    i_t_plate = 0.5,
    i_l_lock = 2,
    i_w_lock = 1,
    i_h_lock = 1,
    i_d_hole = 1.75,
    i_wi_hole = 8,
    i_h_tab = 8,
    i_d_base = 6,
    i_d_tip = 5,
    i_e_precision = 0.01
) {
    color("#999999")
    difference()
	{
        union()
		{
            // Metal Plate
            translate([0, 0, i_t_plate / 2])
			cube
			(
				[
					i_l_plate,
					i_w_plate,
					i_t_plate
				],
				center = true
			);

            // Spring feature using a truncated cone shape
            translate([0, 0, i_t_plate / 2])
			truncated_cone
			(
				i_r_bot = i_d_base / 2,
				i_r_top_delta = (i_d_tip - i_d_base) / 2,
				i_h = i_h_tab,
				i_r_edge_rounding = i_d_base / 20 
			);

            // Locking features
            let(ap_lock_section =
			[
                [-i_l_lock / 2, 0],
                [i_l_lock / 2, 0],
                [i_l_lock / 2, i_h_lock]
            ])
            for (l_offset = [(i_l_plate / 2 - i_w_lock / 2), -(i_l_plate / 2 - i_w_lock / 2)])
            {
                translate([l_offset, 0, i_t_plate])
				rotate([-90, 180, 90])
				linear_extrude(i_w_lock, center = true)
				polygon(ap_lock_section);
            }
        } // END ADD

        union()
		{
            // Drill holes in the metal plate for electrical contacts
            for (w_offset = [i_wi_hole / 2, -i_wi_hole / 2])
			{
                translate([0, w_offset, 0])
				shape_cylinder
				(
					i_d = i_d_hole,
					i_h = i_t_plate,
					i_e = i_e_precision
				);
			}
        } // END SUB
    } // End difference
} // End Module: battery_18650_tab_spring

//--------------------------------------------------------------------------------
//	HOLDER 
//--------------------------------------------------------------------------------

//Two endcaps have the guides to hold spring contacts that can be soldered
//https://it.rs-online.com/web/p/contatti-per-batterie/3247303
module battery_18650_contact_holder
(
	//Size of the cylindrical endcap of the holder
	id_18650_cap = 18,
	it_18650_cap = 3.5,
	//Thickness of the material behind the cap
	it_cap_back = 1,
	//Size of the backplate of the tab with spring, this should include the tollerance
	il_tab = 11.5,
	iw_tab = 11.5,
	//Size of the spring or button
	it_tab_spring = 8,
	//Size of the rails where the tab slots in. This should leave space for the spring itself
	iw_tab_rail = 1.75,
	//Thickness of the plate of the tab with spring, this should include the tollerance
	//There are two tiny inserts to lock the plate and need to slide in
	it_tab = 1.25,
	//Slot carved in the back to expose the plate for soldering
	il_slot_wire = 6,
	iw_slot_wire = 5,
	//Show the model of the tab spring
	ib_show_tab = false,
	//Precision of the circle
	ie_error = 0.01
)
{
	color("cyan")
    difference()
    {
        union()
        {
            //Cylinder cap
            //rotate and move the cylinder inside the holder
            translate([0,0,id_18650_cap/2])
            rotate([0,90,0])
            //Construct cylinder
			shape_cylinder
			(
				i_d = id_18650_cap,
				i_h = it_18650_cap,
				i_e = ie_error
			);

        }
        //Drill a space for the contacts
        union()
        {
			//Drill the plate
			//if(false)
			translate([it_cap_back,0,id_18650_cap/2])
			rotate([0,-90,180])
			linear_extrude(it_tab)
			square([il_tab,iw_tab],center=true);

			//Drill another plate to reach the top of the cap
			//doesn't need to be precise, just extra drill
			translate([it_cap_back,0,id_18650_cap/2+iw_tab])
			rotate([0,-90,180])
			linear_extrude(it_tab)
			square([il_tab,iw_tab],center=true);

			//Drill the rails that reach the surface 
			translate([it_tab+it_cap_back,0,id_18650_cap/2])
			rotate([0,-90,180])
			linear_extrude(it_18650_cap-it_tab-it_cap_back)
			square([il_tab,iw_tab-2*iw_tab_rail],center=true);

			//Drill another to reach the top of the cap
			translate([it_tab+it_cap_back,0,id_18650_cap/2+iw_tab])
			rotate([0,-90,180])
			linear_extrude(it_18650_cap-it_tab-it_cap_back)
			square([il_tab,iw_tab-2*iw_tab_rail],center=true);

			//Drill from the back space to solder the wire
			translate([0,0,il_slot_wire/2])
			rotate([0,-90,180])
			linear_extrude(it_cap_back)
			square([il_slot_wire,iw_slot_wire],center=true);

			//Add an arc to make it printable without support
			//if (false)
			translate([0,0,il_slot_wire])
			rotate([0,-90,180])
			linear_extrude(it_cap_back)
			circle(d=iw_slot_wire,$fn=40);

        }
    }

	if (ib_show_tab == true)
	translate([it_cap_back*1.1,0,id_18650_cap/2])
	rotate([0,-90,180])
	rotate([0,0,90])
	battery_18650_tab_spring
	(
		//Size of the plate
		i_l_plate = il_tab,
		i_w_plate = iw_tab,
		//Size of the spring and tab
		i_h_tab = it_tab_spring
	);

}

//Cap without tab spring shown
//battery_18650_contact_holder();

//Cap with tab spring shown
//battery_18650_contact_holder(ib_show_tab=true);


// Test the module with specific parameters
//battery_18650_tab_spring();

// Optional: Uncomment to create female and male tabs for testing

if (false)
    battery_18650_tab_spring(
        i_l_plate = 10,
        i_w_plate = 10,
        i_t_plate = 0.1,
        i_l_lock = 2,
        i_w_lock = 1,
        i_h_lock = 1,
        i_d_hole = 1.75,
        i_wi_hole = 8,
        i_h_tab = 8, // Total height with spring
        i_d_base = 6,
        i_d_tip = 5,
        i_e_precision = 0.01
    );
