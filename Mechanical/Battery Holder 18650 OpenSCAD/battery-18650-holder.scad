include <libs/polyround.scad>

include <libs/shape_petal.scad>

include <battery-18650.scad>

//Include the geometry for the tab that contact with the battery, and the endcap of the battery holder
include <battery-18650-tabs.scad>

//	2024-06-23
//	Printed but too short for the batteries

//ia_section = controls the strength of the support. 0 is none. 90 comes up to half battery. the more up the more it retains, but the harder it is to insert
//Fixed the shape of the base to be more scalable with wall thickness, now works from 1 to 3
module half_support_18650
(
	ir_inner,
	ir_thickness,
	il_support,
	ia_section
)
{
    aan_points =
	([
        //Origin
        [0,0,0],
        [0,ir_thickness,0],
		[-ir_inner*0.5 -ir_thickness*0.7,ir_thickness,3],
        [-ir_inner*0.6 -ir_thickness*0.9,-0.2*ir_inner,0],
		[-ir_inner*0.3 -ir_thickness*0.4,-0.0*ir_inner,0],
        [-ir_inner*0.5 -ir_thickness*0.7,-0.0*ir_inner,0],
    ]);  

    //Place the base under the battery housing
    translate([0,0,ir_thickness])
    rotate([-90,0,-90])
    //Construct base
    linear_extrude(il_support)
    polygon(polyRound(aan_points,100));

	shape_petal
	(
		i_r_inner = ir_inner,
		i_r_thickness = ir_thickness,
		i_l_length =il_support,
		i_a_angle = ia_section,
		i_e = 0.01
	);
}

if (false)
half_support_18650
(
	ir_inner = (18.4+0.5)/2,
	ir_thickness = 1.0,
	il_support = 20,
	ia_section = 60
);


//Support around a full battery
module full_support_18650
(
	ir_inner,
	ir_thickness,
	il_support,
	ia_section,
)
{
    half_support_18650(ir_inner, ir_thickness, il_support, ia_section);

    //left wing
    translate([il_support,0,0])
    rotate([0,0,180])
    half_support_18650(ir_inner, ir_thickness, il_support, ia_section);
}

//Support around a full battery
module full_support_18650_asymmetric
(
	ir_inner,
	ir_thickness,
	il_support,
	ia_section_right,
	ia_section_left
)
{
    half_support_18650(ir_inner, ir_thickness, il_support, ia_section_right);

    //left wing
    translate([il_support,0,0])
    rotate([0,0,180])
    half_support_18650(ir_inner, ir_thickness, il_support, ia_section_left);
}

//Instance holder for a signle 18650 battery
module holder_18650_1s1p
(
	//	BATTERY
	//Diameter of the 18650 battery plus tollerance
	id_18650 = 18.4+0.5,
	//Length of the 18650 battery from base to button
	il_18650 = 71.0,

	//	TAB SPRING/BUTTON
	//Dimensions of the tab plate, plus tollerance
	il_tab = 11.0 +0.5,
	iw_tab = 11.0 +0.5,
	//Thickness of plate slot
	it_tab = 1.25,
	//Size of the button of the tab
	it_tab_button_positive = 1.5,
	//Length of the tab spring, uncompressed
	il_tab_spring_unloaded = 8.0,
	//Length of the tab spring, fully compressed
	il_tab_spring_loaded = 3.0,
	//How much to compress the spring 0 = unloaded, 1 = loaded
	ilk_tab_spring_compression = 0.75,
	//Rails
	iw_tab_rail = 1.75,

	//	STRUCTURE
	//Thickness of the Holder walls
	it_wall = 2.0,
	//Length factor of the wings 10 means 1/10 of length. 5 mean 1/5 of length
	ilk_wing = 6,


	//Angle of the cradle where the battery rests
	ia_cradle = 90,
	//Angle of the wings keeping the battery in place
	ia_wing = 110,
	
	//Thickness of the cap plus rails
	it_cap = 4.0,
	//thickness of the cap back
	it_cap_back = 2.0,
	


	//	WIRE
	//Lentgh of the wire slot
	il_slot_wire = 6,
	//Width of the wire slot
	iw_slot_wire = 5,

	//	SHOW EXTRA ELEMENTS
	//Show the battery model
	ix_show_battery = false,
	//Show battery tab spring
	ix_show_tab = false
)
{
    nl_base = 50;

	//Compute the spring compression length when battery is placed
	il_tab_spring = il_tab_spring_loaded + (il_tab_spring_unloaded -il_tab_spring_loaded)*(1-ilk_tab_spring_compression);
	
	echo("Spring loading",il_tab_spring);
	//Total length of the battery holder. account for the full stack
	l_total = il_18650 + it_cap_back + it_tab_button_positive + il_tab_spring + it_cap_back;

    //battery
    if (ix_show_battery == true)
    {
        //translate([(l_total-il_18650)/2,0,it_wall])
		translate([it_cap_back+it_tab_button_positive,0,it_wall])
        battery_18650
		(
			// Barrel Dimensions
			i_l_18650 = il_18650,
			i_d_18650 = id_18650-0.5,
			ix_sideway = 1,
			in_invert_poles = true
		);
    }
    
    //I use two sets of wings
    //short wings to retain the battery aligned
    //Tall wings to clip the battery in place

	difference()
	{
		union()
		{
			//Retaining wing butt
			color("red")
			full_support_18650
			(
				id_18650 / 2,
				it_wall,
				l_total / ilk_wing,
				ia_wing
			);

			//Retaining wing front
			color("red")
			translate([l_total *(1 -1/ilk_wing),0,0])
			full_support_18650
			(
				id_18650 / 2,
				it_wall,
				l_total / ilk_wing,
				ia_wing
			);

			//Retaining wing center
			if (false)
			color("red")
			translate([l_total *(2.5/6),0,0])
			full_support_18650
			(
				id_18650 / 2,
				it_wall,
				l_total / 6,
				ia_wing
			);

			//Support wing
			color("red")
			full_support_18650
			(
				id_18650/2,
				it_wall,
				l_total,
				ia_cradle
			);

			//POSITIVE TAB BUTTON 
			translate([0,0,it_wall])
			battery_18650_contact_holder
			(
				//Size of the cylindrical endcap of the holder
				id_18650_cap = id_18650,
				it_18650_cap = it_cap,
				//Thickness of the material behind the cap
				it_cap_back = it_cap_back,
				//Size of the backplate of the tab with spring, this should include the tollerance
				il_tab = il_tab,
				iw_tab = iw_tab,
				//Size of the spring or button
				it_tab_spring = it_tab_button_positive,
				//Size of the rails where the tab slots in. This should leave space for the spring itself
				iw_tab_rail = iw_tab_rail,
				//Thickness of the plate of the tab with spring, this should include the tollerance
				//There are two tiny inserts to lock the plate and need to slide in
				it_tab = it_tab,
				//Slot carved in the back to expose the plate for soldering
				il_slot_wire = il_slot_wire,
				iw_slot_wire = iw_slot_wire,
				//Show the model of the tab spring
				ib_show_tab = ix_show_tab,
				//Precision of the circle
				ie_error = 0.01
			);

			//NEGATIVE TABSPRING
			translate([l_total,0,it_wall])
			rotate([0,0,180])
			battery_18650_contact_holder
			(
				//Size of the cylindrical endcap of the holder
				id_18650_cap = id_18650,
				it_18650_cap = it_cap,
				//Thickness of the material behind the cap
				it_cap_back = it_cap_back,
				//Size of the backplate of the tab with spring, this should include the tollerance
				il_tab = il_tab,
				iw_tab = iw_tab,
				//Size of the spring or button
				it_tab_spring = il_tab_spring_unloaded,
				//Size of the rails where the tab slots in. This should leave space for the spring itself
				iw_tab_rail = iw_tab_rail,
				//Thickness of the plate of the tab with spring, this should include the tollerance
				//There are two tiny inserts to lock the plate and need to slide in
				it_tab = it_tab,
				//Slot carved in the back to expose the plate for soldering
				il_slot_wire = 5.5,
				iw_slot_wire = iw_slot_wire,
				//Show the model of the tab spring
				ib_show_tab = ix_show_tab,
				//Precision of the circle
				ie_error = 0.01
			);
		}	//End Sum

		union()
		{
			//FRONT
			//Drill from the back space to solder the wire
			translate([0,0,it_wall*1.5/2])
			rotate([0,-90,180])
			linear_extrude(it_cap_back)
			square([it_wall*1.5,iw_slot_wire],center=true);

			//REAR
			//Drill from the back space to solder the wire
			translate([l_total+0.01,0,it_wall*1.5/2])
			rotate([0,-90,0])
			linear_extrude(it_cap_back)
			square([it_wall*1.5,iw_slot_wire],center=true);

		} //End Subtract

	} //End difference
	
}

//----------------------------------------------------------------------------
//	2S 1P
//----------------------------------------------------------------------------

//Instance holder for a signle 18650 battery
module holder_18650_2s1p
(
	//	BATTERY
	//Diameter of the 18650 battery plus tollerance
	id_18650 = 18.4+0.5,
	//Length of the 18650 battery from base to button
	il_18650 = 71.0,

	//	TAB SPRING/BUTTON
	//Dimensions of the tab plate, plus tollerance
	il_tab = 11.0 +0.5,
	iw_tab = 11.0 +0.5,
	//Thickness of plate slot
	it_tab = 1.25,
	//Size of the button of the tab
	it_tab_button_positive = 1.5,
	//Length of the tab spring, uncompressed
	il_tab_spring_unloaded = 8.0,
	//Length of the tab spring, fully compressed
	il_tab_spring_loaded = 3.0,
	//How much to compress the spring 0 = unloaded, 1 = loaded
	ilk_tab_spring_compression = 0.75,
	//Rails
	iw_tab_rail = 1.75,

	//	STRUCTURE
	//Thickness of the Holder walls
	it_wall = 2.0,
	//Length factor of the wings 10 means 1/10 of length. 5 mean 1/5 of length
	ilk_wing = 10,

	//Angle of the cradle where the battery rests
	ia_cradle = 90,
	//Angle of the wings keeping the battery in place
	ia_wing = 110,
	
	//Thickness of the cap plus rails
	it_cap = 4.0,
	//thickness of the cap back
	it_cap_back = 2.0,

	//	WIRE
	//Lentgh of the wire slot
	il_slot_wire = 6,
	//Width of the wire slot
	iw_slot_wire = 5,

	//	SHOW EXTRA ELEMENTS
	//Show the battery model
	ix_show_battery = false,
	//Show battery tab spring
	ix_show_tab = false
)
{
    nl_base = 50;

	//Compute the spring compression length when battery is placed
	il_tab_spring = il_tab_spring_loaded + (il_tab_spring_unloaded -il_tab_spring_loaded)*(1-ilk_tab_spring_compression);
	
	echo("Spring loading",il_tab_spring);
	//Total length of the battery holder. account for the full stack
	l_total = il_18650 * 2 + it_cap_back + it_tab_button_positive + il_tab_spring + it_cap_back;

    //battery
    if (ix_show_battery == true)
    {
        //translate([(l_total-il_18650)/2,0,it_wall])
		for (ol = [0, il_18650])
		{
			translate([it_cap_back+it_tab_button_positive+ol,0,it_wall])
			battery_18650
			(
				// Barrel Dimensions
				i_l_18650 = il_18650,
				i_d_18650 = id_18650-0.5,
				ix_sideway = 1,
				in_invert_poles = true
			);
		}
    }
    
    //I use two sets of wings
    //short wings to retain the battery aligned
    //Tall wings to clip the battery in place

	difference()
	{
		union()
		{
			//Retaining wing butt
			color("red")
			full_support_18650
			(
				id_18650 / 2,
				it_wall,
				l_total / ilk_wing,
				ia_wing
			);

			//Retaining wing front
			color("red")
			translate([l_total *(1 -1/ilk_wing),0,0])
			full_support_18650
			(
				id_18650 / 2,
				it_wall,
				l_total / ilk_wing,
				ia_wing
			);

			//Retaining wing center
			color("red")
			translate([l_total * (1 -1/ilk_wing) / 2,0,0])
			full_support_18650
			(
				id_18650 / 2,
				it_wall,
				l_total / ilk_wing,
				ia_wing
			);

			//Support Structure
			color("red")
			full_support_18650
			(
				id_18650/2,
				it_wall,
				l_total,
				ia_cradle
			);

			//POSITIVE TAB BUTTON 
			translate([0,0,it_wall])
			battery_18650_contact_holder
			(
				//Size of the cylindrical endcap of the holder
				id_18650_cap = id_18650,
				it_18650_cap = it_cap,
				//Thickness of the material behind the cap
				it_cap_back = it_cap_back,
				//Size of the backplate of the tab with spring, this should include the tollerance
				il_tab = il_tab,
				iw_tab = iw_tab,
				//Size of the spring or button
				it_tab_spring = it_tab_button_positive,
				//Size of the rails where the tab slots in. This should leave space for the spring itself
				iw_tab_rail = iw_tab_rail,
				//Thickness of the plate of the tab with spring, this should include the tollerance
				//There are two tiny inserts to lock the plate and need to slide in
				it_tab = it_tab,
				//Slot carved in the back to expose the plate for soldering
				il_slot_wire = il_slot_wire,
				iw_slot_wire = iw_slot_wire,
				//Show the model of the tab spring
				ib_show_tab = ix_show_tab,
				//Precision of the circle
				ie_error = 0.01
			);

			//NEGATIVE TABSPRING
			translate([l_total,0,it_wall])
			rotate([0,0,180])
			battery_18650_contact_holder
			(
				//Size of the cylindrical endcap of the holder
				id_18650_cap = id_18650,
				it_18650_cap = it_cap,
				//Thickness of the material behind the cap
				it_cap_back = it_cap_back,
				//Size of the backplate of the tab with spring, this should include the tollerance
				il_tab = il_tab,
				iw_tab = iw_tab,
				//Size of the spring or button
				it_tab_spring = il_tab_spring_unloaded,
				//Size of the rails where the tab slots in. This should leave space for the spring itself
				iw_tab_rail = iw_tab_rail,
				//Thickness of the plate of the tab with spring, this should include the tollerance
				//There are two tiny inserts to lock the plate and need to slide in
				it_tab = it_tab,
				//Slot carved in the back to expose the plate for soldering
				il_slot_wire = il_slot_wire,
				iw_slot_wire = iw_slot_wire,
				//Show the model of the tab spring
				ib_show_tab = ix_show_tab,
				//Precision of the circle
				ie_error = 0.01
			);
		}	//End Sum

		union()
		{
			//FRONT
			//Drill from the back space to solder the wire
			translate([0,0,it_wall*1.5/2])
			rotate([0,-90,180])
			linear_extrude(it_cap_back)
			square([it_wall*1.5,iw_slot_wire],center=true);

			//REAR
			//Drill from the back space to solder the wire
			translate([l_total+0.01,0,it_wall*1.5/2])
			rotate([0,-90,0])
			linear_extrude(it_cap_back)
			square([it_wall*1.5,iw_slot_wire],center=true);

		} //End Subtract
	} //End difference
}

//----------------------------------------------------------------------------
//	2S 2P
//----------------------------------------------------------------------------

//Instance holder for a signle 18650 battery
module holder_18650_2s2p
(
	//	BATTERY
	//Diameter of the 18650 battery plus tollerance
	id_18650 = 18.4+0.5,
	//Length of the 18650 battery from base to button
	il_18650 = 71.0,

	//	TAB SPRING/BUTTON
	//Dimensions of the tab plate, plus tollerance
	il_tab = 11.0 +0.5,
	iw_tab = 11.0 +0.5,
	//Thickness of plate slot
	it_tab = 1.25,
	//Size of the button of the tab
	it_tab_button_positive = 1.5,
	//Length of the tab spring, uncompressed
	il_tab_spring_unloaded = 8.0,
	//Length of the tab spring, fully compressed
	il_tab_spring_loaded = 3.0,
	//How much to compress the spring 0 = unloaded, 1 = loaded
	ilk_tab_spring_compression = 0.75,
	//Rails
	iw_tab_rail = 1.75,

	//	STRUCTURE
	//Thickness of the Holder walls
	it_wall = 2.0,
	//Length factor of the wings 10 means 1/10 of length. 5 mean 1/5 of length
	ilk_wing = 10,

	//Angle of the wings keeping the battery in place
	ia_wing = 116,
	//Angle of the cradle where the battery rests
	ia_cradle = 90,
	//Angle for the guide between batteries
	ia_guide = 65,
	
	//Thickness of the cap plus rails
	it_cap = 4.0,
	//thickness of the cap back
	it_cap_back = 2.0,

	//	WIRE
	//Lentgh of the wire slot
	il_slot_wire = 6,
	//Width of the wire slot
	iw_slot_wire = 5,

	//	SHOW EXTRA ELEMENTS
	//Show the battery model
	ix_show_battery = false,
	//Show battery tab spring
	ix_show_tab = false
)
{
    nl_base = 50;

	//Compute the spring compression length when battery is placed
	il_tab_spring = il_tab_spring_loaded + (il_tab_spring_unloaded -il_tab_spring_loaded)*(1-ilk_tab_spring_compression);
	
	echo("Spring loading",il_tab_spring);
	//Total length of the battery holder. account for the full stack
	l_total = il_18650 * 2 + it_cap_back + it_tab_button_positive + il_tab_spring + it_cap_back;

	//battery
    if (ix_show_battery == true)
    {

		for (ol = [0, il_18650])
		for (ow = [-id_18650/2, id_18650/2])
		{
			translate
			([
				it_cap_back+it_tab_button_positive+ol,
				ow,
				it_wall
			])
			battery_18650
			(
				// Barrel Dimensions
				i_l_18650 = il_18650,
				i_d_18650 = id_18650-0.5,
				ix_sideway = 1,
				in_invert_poles = ow > 0
			);

		}
    }

    
    //I use two sets of wings
    //short wings to retain the battery aligned
    //Tall wings to clip the battery in place
	difference()
	{
		union()
		{

			//----------------------------------------------------------
			//	REAR SECTION
			//----------------------------------------------------------
			//	Composed of two wings that retain the battery
			//	and two caps that accept the contacts

			//REAR LEFT CAP BUTTON
			translate
			([
				0,
				+id_18650/2,
				it_wall
			])
			battery_18650_contact_holder
			(
				//Size of the cylindrical endcap of the holder
				id_18650_cap = id_18650,
				it_18650_cap = it_cap,
				//Thickness of the material behind the cap
				it_cap_back = it_cap_back,
				//Size of the backplate of the tab with spring, this should include the tollerance
				il_tab = il_tab,
				iw_tab = iw_tab,
				//Size of the spring or button
				it_tab_spring = it_tab_button_positive,
				//Size of the rails where the tab slots in. This should leave space for the spring itself
				iw_tab_rail = iw_tab_rail,
				//Thickness of the plate of the tab with spring, this should include the tollerance
				//There are two tiny inserts to lock the plate and need to slide in
				it_tab = it_tab,
				//Slot carved in the back to expose the plate for soldering
				il_slot_wire = il_slot_wire,
				iw_slot_wire = iw_slot_wire,
				//Show the model of the tab spring
				ib_show_tab = ix_show_tab,
				//Precision of the circle
				ie_error = 0.01
			);

			//READ LEFT WING
			color("red")
			translate
			([
				0,
				+id_18650/2,
				0
			])
			full_support_18650_asymmetric
			(
				ir_inner = id_18650 / 2,
				ir_thickness = it_wall,
				il_support = l_total / ilk_wing,
				ia_section_right = ia_wing,
				ia_section_left = ia_guide
			);

			//REAR RIGHT CAP SPRING
			translate
			([
				0,
				-id_18650/2,
				it_wall
			])
			battery_18650_contact_holder
			(
				//Size of the cylindrical endcap of the holder
				id_18650_cap = id_18650,
				it_18650_cap = it_cap,
				//Thickness of the material behind the cap
				it_cap_back = it_cap_back,
				//Size of the backplate of the tab with spring, this should include the tollerance
				il_tab = il_tab,
				iw_tab = iw_tab,
				//Size of the spring or button
				it_tab_spring = il_tab_spring_unloaded,
				//Size of the rails where the tab slots in. This should leave space for the spring itself
				iw_tab_rail = iw_tab_rail,
				//Thickness of the plate of the tab with spring, this should include the tollerance
				//There are two tiny inserts to lock the plate and need to slide in
				it_tab = it_tab,
				//Slot carved in the back to expose the plate for soldering
				il_slot_wire = il_slot_wire,
				iw_slot_wire = iw_slot_wire,
				//Show the model of the tab spring
				ib_show_tab = ix_show_tab,
				//Precision of the circle
				ie_error = 0.01
			);

			//REAR RIGHT WING
			color("red")
			translate
			([
				0,
				-id_18650/2,
				0
			])
			full_support_18650_asymmetric
			(
				ir_inner = id_18650 / 2,
				ir_thickness = it_wall,
				il_support = l_total / ilk_wing,
				ia_section_right = ia_guide,
				ia_section_left = ia_wing
			);

			//REAR LEFT FILL
			color("red")
			translate
			([
				0,
				+id_18650/2,
				0
			])
			full_support_18650_asymmetric
			(
				ir_inner = id_18650 / 2,
				ir_thickness = it_wall,
				il_support = it_cap,
				ia_section_right = ia_wing,
				ia_section_left = ia_wing
			);


			//----------------------------------------------------------
			//	CORE
			//----------------------------------------------------------
			//	Long right and left rest of the batteries
			//	Guide that retains the front and rear of the batterire in the middle

			//CORE RIGHT
			color("red")
			translate
			([
				0,
				+id_18650/2,
				0
			])
			full_support_18650_asymmetric
			(
				id_18650/2,
				it_wall,
				l_total,
				ia_section_right = ia_cradle,
				ia_section_left = ia_guide
			);

			//CORE LEFT
			color("red")
			translate
			([
				0,
				-id_18650/2,
				0
			])
			full_support_18650_asymmetric
			(
				id_18650/2,
				it_wall,
				l_total,
				ia_section_right = ia_guide,
				ia_section_left = ia_cradle
			);

			//CENTER LEFT WING
			color("red")
			translate
			([
				l_total * (1 -1/ilk_wing) / 2,
				+id_18650/2,
				0
			])
			full_support_18650_asymmetric
			(
				ir_inner = id_18650 / 2,
				ir_thickness = it_wall,
				il_support = l_total / ilk_wing,
				ia_section_right = ia_wing,
				ia_section_left = ia_guide
			);

			//CENTER RIGH WING
			color("red")
			translate
			([
				l_total * (1 -1/ilk_wing) / 2,
				-id_18650/2,
				0
			])
			full_support_18650_asymmetric
			(
				ir_inner = id_18650 / 2,
				ir_thickness = it_wall,
				il_support = l_total / ilk_wing,
				ia_section_right = ia_guide,
				ia_section_left = ia_wing
			);



			//----------------------------------------------------------
			//	FRONT SECTION
			//----------------------------------------------------------
			//	Composed of two wings that retain the battery
			//	and two caps that accept the contacts


			//FRONT LEFT CAP SPRING
			translate
			([
				l_total,
				+id_18650 / 2,
				it_wall
			])
			rotate([0,0,180])
			battery_18650_contact_holder
			(
				//Size of the cylindrical endcap of the holder
				id_18650_cap = id_18650,
				it_18650_cap = it_cap,
				//Thickness of the material behind the cap
				it_cap_back = it_cap_back,
				//Size of the backplate of the tab with spring, this should include the tollerance
				il_tab = il_tab,
				iw_tab = iw_tab,
				//Size of the spring or button
				it_tab_spring = il_tab_spring_unloaded,
				//Size of the rails where the tab slots in. This should leave space for the spring itself
				iw_tab_rail = iw_tab_rail,
				//Thickness of the plate of the tab with spring, this should include the tollerance
				//There are two tiny inserts to lock the plate and need to slide in
				it_tab = it_tab,
				//Slot carved in the back to expose the plate for soldering
				il_slot_wire = il_slot_wire,
				iw_slot_wire = iw_slot_wire,
				//Show the model of the tab spring
				ib_show_tab = ix_show_tab,
				//Precision of the circle
				ie_error = 0.01
			);

			//FRONT LEFT GUIDE
			color("red")
			translate
			([
				l_total *(1 -1/ilk_wing),
				+id_18650/2,
				0
			])
			full_support_18650_asymmetric
			(
				id_18650 / 2,
				it_wall,
				l_total / ilk_wing,
				ia_section_right = ia_wing,
				ia_section_left = ia_guide
			);

			//FRONT RIGHT CAP BUTTON
			translate
			([
				l_total,
				-id_18650 / 2,
				it_wall
			])
			rotate([0,0,180])
			battery_18650_contact_holder
			(
				//Size of the cylindrical endcap of the holder
				id_18650_cap = id_18650,
				it_18650_cap = it_cap,
				//Thickness of the material behind the cap
				it_cap_back = it_cap_back,
				//Size of the backplate of the tab with spring, this should include the tollerance
				il_tab = il_tab,
				iw_tab = iw_tab,
				//Size of the spring or button
				it_tab_spring = it_tab_button_positive,
				//Size of the rails where the tab slots in. This should leave space for the spring itself
				iw_tab_rail = iw_tab_rail,
				//Thickness of the plate of the tab with spring, this should include the tollerance
				//There are two tiny inserts to lock the plate and need to slide in
				it_tab = it_tab,
				//Slot carved in the back to expose the plate for soldering
				il_slot_wire = il_slot_wire,
				iw_slot_wire = iw_slot_wire,
				//Show the model of the tab spring
				ib_show_tab = ix_show_tab,
				//Precision of the circle
				ie_error = 0.01
			);

			//FRONT RIGHT GUIDE
			color("red")
			translate
			([
				l_total *(1 -1/ilk_wing),
				-id_18650/2,
				0
			])
			full_support_18650_asymmetric
			(
				id_18650 / 2,
				it_wall,
				l_total / ilk_wing,
				ia_section_right = ia_guide,
				ia_section_left = ia_wing
			);

			//FRONT LEFT FILL
			color("red")
			translate
			([
				l_total -it_cap,
				+id_18650/2,
				0
			])
			full_support_18650_asymmetric
			(
				ir_inner = id_18650 / 2,
				ir_thickness = it_wall,
				il_support = it_cap,
				ia_section_right = ia_wing,
				ia_section_left = ia_wing
			);

		}	//End Sum

		union()
		{
			//FRONT
			//Drill from the back space to solder the wire
			translate([0,0,it_wall*1.5/2])
			rotate([0,-90,180])
			linear_extrude(it_cap_back)
			square([it_wall*1.5,iw_slot_wire],center=true);

			//REAR
			//Drill from the back space to solder the wire
			translate([l_total+0.01,0,it_wall*1.5/2])
			rotate([0,-90,0])
			linear_extrude(it_cap_back)
			square([it_wall*1.5,iw_slot_wire],center=true);

		} //End Subtract
	} //End difference
}


//----------------------------------------------------------------------------
//	2S 2P INVERTED
//----------------------------------------------------------------------------

//Instance holder for a signle 18650 battery
module holder_18650_2s2p_inverted
(
	//	BATTERY
	//Diameter of the 18650 battery plus tollerance
	id_18650 = 18.4+0.5,
	//Length of the 18650 battery from base to button
	il_18650 = 71.0,

	//	TAB SPRING/BUTTON
	//Dimensions of the tab plate, plus tollerance
	il_tab = 11.0 +0.5,
	iw_tab = 11.0 +0.5,
	//Thickness of plate slot
	it_tab = 1.25,
	//Size of the button of the tab
	it_tab_button_positive = 1.5,
	//Length of the tab spring, uncompressed
	il_tab_spring_unloaded = 8.0,
	//Length of the tab spring, fully compressed
	il_tab_spring_loaded = 3.0,
	//How much to compress the spring 0 = unloaded, 1 = loaded
	ilk_tab_spring_compression = 0.75,
	//Rails
	iw_tab_rail = 1.75,

	//	STRUCTURE
	//Thickness of the Holder walls
	it_wall = 2.0,
	//Length factor of the wings 10 means 1/10 of length. 5 mean 1/5 of length
	ilk_wing = 10,

	//Angle of the wings keeping the battery in place
	ia_wing = 116,
	//Angle of the cradle where the battery rests
	ia_cradle = 90,
	//Angle for the guide between batteries
	ia_guide = 65,
	
	//Thickness of the cap plus rails
	it_cap = 4.0,
	//thickness of the cap back
	it_cap_back = 2.0,

	//	WIRE
	//Lentgh of the wire slot
	il_slot_wire = 6,
	//Width of the wire slot
	iw_slot_wire = 5,

	//	SHOW EXTRA ELEMENTS
	//Show the battery model
	ix_show_battery = false,
	//Show battery tab spring
	ix_show_tab = false
)
{
    nl_base = 50;

	//Compute the spring compression length when battery is placed
	il_tab_spring = il_tab_spring_loaded + (il_tab_spring_unloaded -il_tab_spring_loaded)*(1-ilk_tab_spring_compression);
	
	echo("Spring loading",il_tab_spring);
	//Total length of the battery holder. account for the full stack
	l_total = il_18650 * 2 + it_cap_back + it_tab_button_positive + il_tab_spring + it_cap_back;


    
    //I use two sets of wings
    //short wings to retain the battery aligned
    //Tall wings to clip the battery in place
	translate([0,0,id_18650+it_wall])
	rotate([180,0,0])
	difference()
	{
		union()
		{

			//battery
			if (ix_show_battery == true)
			{

				for (ol = [0, il_18650])
				for (ow = [-id_18650/2, id_18650/2])
				{
					translate
					([
						it_cap_back+it_tab_button_positive+ol,
						ow,
						it_wall
					])
					battery_18650
					(
						// Barrel Dimensions
						i_l_18650 = il_18650,
						i_d_18650 = id_18650-0.5,
						ix_sideway = 1,
						in_invert_poles = ow > 0
					);

				}
			}


			//----------------------------------------------------------
			//	REAR SECTION
			//----------------------------------------------------------
			//	Composed of two wings that retain the battery
			//	and two caps that accept the contacts

			//REAR LEFT CAP BUTTON
			translate
			([
				0,
				+id_18650/2,
				it_wall
			])
			battery_18650_contact_holder
			(
				//Size of the cylindrical endcap of the holder
				id_18650_cap = id_18650,
				it_18650_cap = it_cap,
				//Thickness of the material behind the cap
				it_cap_back = it_cap_back,
				//Size of the backplate of the tab with spring, this should include the tollerance
				il_tab = il_tab,
				iw_tab = iw_tab,
				//Size of the spring or button
				it_tab_spring = it_tab_button_positive,
				//Size of the rails where the tab slots in. This should leave space for the spring itself
				iw_tab_rail = iw_tab_rail,
				//Thickness of the plate of the tab with spring, this should include the tollerance
				//There are two tiny inserts to lock the plate and need to slide in
				it_tab = it_tab,
				//Slot carved in the back to expose the plate for soldering
				il_slot_wire = il_slot_wire,
				iw_slot_wire = iw_slot_wire,
				//Show the model of the tab spring
				ib_show_tab = ix_show_tab,
				//Precision of the circle
				ie_error = 0.01
			);

			//READ LEFT WING
			color("red")
			translate
			([
				0,
				+id_18650/2,
				0
			])
			full_support_18650_asymmetric
			(
				ir_inner = id_18650 / 2,
				ir_thickness = it_wall,
				il_support = l_total / ilk_wing,
				ia_section_right = ia_wing,
				ia_section_left = ia_guide
			);

			//REAR RIGHT CAP SPRING
			translate
			([
				0,
				-id_18650/2,
				it_wall
			])
			battery_18650_contact_holder
			(
				//Size of the cylindrical endcap of the holder
				id_18650_cap = id_18650,
				it_18650_cap = it_cap,
				//Thickness of the material behind the cap
				it_cap_back = it_cap_back,
				//Size of the backplate of the tab with spring, this should include the tollerance
				il_tab = il_tab,
				iw_tab = iw_tab,
				//Size of the spring or button
				it_tab_spring = il_tab_spring_unloaded,
				//Size of the rails where the tab slots in. This should leave space for the spring itself
				iw_tab_rail = iw_tab_rail,
				//Thickness of the plate of the tab with spring, this should include the tollerance
				//There are two tiny inserts to lock the plate and need to slide in
				it_tab = it_tab,
				//Slot carved in the back to expose the plate for soldering
				il_slot_wire = il_slot_wire,
				iw_slot_wire = iw_slot_wire,
				//Show the model of the tab spring
				ib_show_tab = ix_show_tab,
				//Precision of the circle
				ie_error = 0.01
			);

			//REAR RIGHT WING
			color("red")
			translate
			([
				0,
				-id_18650/2,
				0
			])
			full_support_18650_asymmetric
			(
				ir_inner = id_18650 / 2,
				ir_thickness = it_wall,
				il_support = l_total / ilk_wing,
				ia_section_right = ia_guide,
				ia_section_left = ia_wing
			);

			//REAR LEFT FILL
			color("red")
			translate
			([
				0,
				+id_18650/2,
				0
			])
			full_support_18650_asymmetric
			(
				ir_inner = id_18650 / 2,
				ir_thickness = it_wall,
				il_support = it_cap,
				ia_section_right = ia_wing,
				ia_section_left = ia_wing
			);

			//----------------------------------------------------------
			//	CORE
			//----------------------------------------------------------
			//	Long right and left rest of the batteries
			//	Guide that retains the front and rear of the batterire in the middle

			//CORE RIGHT
			color("red")
			translate
			([
				0,
				+id_18650/2,
				0
			])
			full_support_18650_asymmetric
			(
				id_18650/2,
				it_wall,
				l_total,
				ia_section_right = ia_cradle,
				ia_section_left = ia_guide
			);

			//CORE LEFT
			color("red")
			translate
			([
				0,
				-id_18650/2,
				0
			])
			full_support_18650_asymmetric
			(
				id_18650/2,
				it_wall,
				l_total,
				ia_section_right = ia_guide,
				ia_section_left = ia_cradle
			);

			//CENTER LEFT WING
			color("red")
			translate
			([
				l_total * (1 -1/ilk_wing) / 2,
				+id_18650/2,
				0
			])
			full_support_18650_asymmetric
			(
				ir_inner = id_18650 / 2,
				ir_thickness = it_wall,
				il_support = l_total / ilk_wing,
				ia_section_right = ia_wing,
				ia_section_left = ia_guide
			);

			//CENTER RIGH WING
			color("red")
			translate
			([
				l_total * (1 -1/ilk_wing) / 2,
				-id_18650/2,
				0
			])
			full_support_18650_asymmetric
			(
				ir_inner = id_18650 / 2,
				ir_thickness = it_wall,
				il_support = l_total / ilk_wing,
				ia_section_right = ia_guide,
				ia_section_left = ia_wing
			);

			//CORE LEFT
			color("#777777")
			translate
			([
				0,
				id_18650 * (3/2) + it_wall,
				id_18650 *1 + it_wall * 1
			])
			rotate([180,0,0])
			half_support_18650
			(
				ir_inner = id_18650 / 2,
				ir_thickness = it_wall,
				il_support = l_total,
				ia_section = 90
			);

			//CORE RIGHT
			color("#777777")
			translate
			([
				l_total,
				- id_18650 * (3/2) - it_wall,
				id_18650 *1 + it_wall * 1
			])
			rotate([180,0,180])
			half_support_18650
			(
				ir_inner = id_18650 / 2,
				ir_thickness = it_wall,
				il_support = l_total,
				ia_section = 90
			);

			//----------------------------------------------------------
			//	FRONT SECTION
			//----------------------------------------------------------
			//	Composed of two wings that retain the battery
			//	and two caps that accept the contacts


			//FRONT LEFT CAP SPRING
			translate
			([
				l_total,
				+id_18650 / 2,
				it_wall
			])
			rotate([0,0,180])
			battery_18650_contact_holder
			(
				//Size of the cylindrical endcap of the holder
				id_18650_cap = id_18650,
				it_18650_cap = it_cap,
				//Thickness of the material behind the cap
				it_cap_back = it_cap_back,
				//Size of the backplate of the tab with spring, this should include the tollerance
				il_tab = il_tab,
				iw_tab = iw_tab,
				//Size of the spring or button
				it_tab_spring = il_tab_spring_unloaded,
				//Size of the rails where the tab slots in. This should leave space for the spring itself
				iw_tab_rail = iw_tab_rail,
				//Thickness of the plate of the tab with spring, this should include the tollerance
				//There are two tiny inserts to lock the plate and need to slide in
				it_tab = it_tab,
				//Slot carved in the back to expose the plate for soldering
				il_slot_wire = il_slot_wire,
				iw_slot_wire = iw_slot_wire,
				//Show the model of the tab spring
				ib_show_tab = ix_show_tab,
				//Precision of the circle
				ie_error = 0.01
			);

			//FRONT LEFT GUIDE
			color("red")
			translate
			([
				l_total *(1 -1/ilk_wing),
				+id_18650/2,
				0
			])
			full_support_18650_asymmetric
			(
				id_18650 / 2,
				it_wall,
				l_total / ilk_wing,
				ia_section_right = ia_wing,
				ia_section_left = ia_guide
			);

			//FRONT RIGHT CAP BUTTON
			translate
			([
				l_total,
				-id_18650 / 2,
				it_wall
			])
			rotate([0,0,180])
			battery_18650_contact_holder
			(
				//Size of the cylindrical endcap of the holder
				id_18650_cap = id_18650,
				it_18650_cap = it_cap,
				//Thickness of the material behind the cap
				it_cap_back = it_cap_back,
				//Size of the backplate of the tab with spring, this should include the tollerance
				il_tab = il_tab,
				iw_tab = iw_tab,
				//Size of the spring or button
				it_tab_spring = it_tab_button_positive,
				//Size of the rails where the tab slots in. This should leave space for the spring itself
				iw_tab_rail = iw_tab_rail,
				//Thickness of the plate of the tab with spring, this should include the tollerance
				//There are two tiny inserts to lock the plate and need to slide in
				it_tab = it_tab,
				//Slot carved in the back to expose the plate for soldering
				il_slot_wire = il_slot_wire,
				iw_slot_wire = iw_slot_wire,
				//Show the model of the tab spring
				ib_show_tab = ix_show_tab,
				//Precision of the circle
				ie_error = 0.01
			);

			//FRONT RIGHT GUIDE
			color("red")
			translate
			([
				l_total *(1 -1/ilk_wing),
				-id_18650/2,
				0
			])
			full_support_18650_asymmetric
			(
				id_18650 / 2,
				it_wall,
				l_total / ilk_wing,
				ia_section_right = ia_guide,
				ia_section_left = ia_wing
			);

			//FRONT LEFT FILL
			color("red")
			translate
			([
				l_total -it_cap,
				+id_18650/2,
				0
			])
			full_support_18650_asymmetric
			(
				ir_inner = id_18650 / 2,
				ir_thickness = it_wall,
				il_support = it_cap,
				ia_section_right = ia_wing,
				ia_section_left = ia_wing
			);


			//----------------------------------------------------------
			//	INVERTED CAP 
			//----------------------------------------------------------

			aan_cap =
			[
				[-id_18650, id_18650/2+it_wall],
				[id_18650, id_18650/2+it_wall],
				[id_18650*1.5, id_18650*1.0+it_wall],
				[-id_18650*1.5, id_18650*1.0+it_wall],
			];

			//BACK
			rotate([0,0,90])
			rotate([90,0,0])
			linear_extrude(it_cap_back)
			polygon(aan_cap);

			//FRONT
			translate
			([
				l_total - it_cap_back,
				0,
				0,
			])
			rotate([0,0,90])
			rotate([90,0,0])
			linear_extrude(it_cap_back)
			polygon(aan_cap);

		}	//End Sum

		union()
		{
			//REAR RIGHT
			//Drill from the back space to solder the wire
			translate
			([
				it_cap_back,
				id_18650 * 0.5,
				it_wall*1.5/2
			])
			rotate([0,-90,0])
			linear_extrude(it_cap_back)
			square
			(
				[
					it_wall*1.5,
					iw_slot_wire
				],
				center=true
			);

			//REAR LEFT
			//Drill from the back space to solder the wire
			translate
			([
				it_cap_back,
				-id_18650 * 0.5,
				it_wall*1.5/2
			])
			rotate([0,-90,0])
			linear_extrude(it_cap_back)
			square
			(
				[
					it_wall*1.5,
					iw_slot_wire
				],
				center=true
			);


			//FRONT LEFT
			//Drill from the back space to solder the wire
			translate
			([
				l_total+0.01,
				-id_18650 * 0.5,
				it_wall*1.5/2
			])
			rotate([0,-90,0])
			linear_extrude(it_cap_back)
			square
			(
				[
					it_wall*1.5,
					iw_slot_wire
				],
				center=true
			);

			//FRONT RIGHT
			//Drill from the back space to solder the wire
			translate
			([
				l_total+0.01,
				id_18650 * 0.5,
				it_wall*1.5/2
			])
			rotate([0,-90,0])
			linear_extrude(it_cap_back)
			square
			(
				[
					it_wall*1.5,
					iw_slot_wire
				],
				center=true
			);
		} //End Subtract
	} //End difference


}

//----------------------------------------------------------------------------
//	TESTS
//----------------------------------------------------------------------------

///	HOLDER ONE BATTERY 1S 1P

//holder_18650_1s1p();

//holder_18650_1s1p( ix_show_battery = true, ix_show_tab = true );

///	HOLDER TWO BATTERIES IN SERIES 2S 1P

//holder_18650_2s1p();

//holder_18650_2s1p( ix_show_battery = true, ix_show_tab = true );

///	HOLDER FOUR BATTERIES 2S 2P

//holder_18650_2s2p();

//holder_18650_2s2p( ix_show_battery = true, ix_show_tab = true );

///	HOLDER FOUR BATTERIES 2S 2P INVERTED

holder_18650_2s2p_inverted();

//holder_18650_2s2p_inverted( ix_show_battery = true, ix_show_tab = true );