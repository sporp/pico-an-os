thic = 4;
thic2 = thic * 2;
thic4 = thic2 * 2;

//cube([]);

module usb_blank(){
    difference(){
        cube([6 + thic2, 12 + thic2, 12 + thic2]);
        
        translate([-2,-5,2])
        rotate([45,0,0])
        cube(30);
        
        translate([thic,thic,thic +2.5/2])
        cube([4.5,30,12.5]);
        
        translate([thic - 1,0,thic +2.5/2])
        cube([7,12 + thic, 15]);
    }
}

module blank(){
    usb_blank();
    cube([80 + thic2, 1 + thic2, 3 + thic]);
    translate([80 + thic2 - 28 + thic2 + 2,1 + thic2,0])
    difference(){
        cube(14 + thic);
        translate([thic/2,thic/2,thic])
        cube(14);
        
        translate([thic2,thic,thic])
        cube([80 + thic2, 1.5, 20 + thic]);
    }
}

difference(){
    blank();
    translate([thic2,thic,thic])
    cube([80 + thic2, 1.5, 3 + thic]);
    
    translate([30,thic,thic])
    cube([80 + thic2, 1.5, 40]);
    
    translate([thic - 1, 0, thic])
    cube([10,14, 20]);
    
    translate([thic - 1, 0, thic + 3])
    cube([14,14, 20]);
}
