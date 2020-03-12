`timescale 1us/1ps

/*
STAT TABLE [C,neg(<1.0?),E,pos(>2.0?)]
00	blinking	stat=blk
01	not charging	stat=1
10	charging	stat=0
11	not possible	

BUTTON TABLE
1	pressed
0	released

USB TABLE
1	plugged in
0	not plugged in

LEVEL TABLE
1	full battery
0	not full battery
*/

module test();
	reg clk, rst, level, button, usb, stat, pos, neg;
	wire red,green;

	initial begin
		$dumpfile("my_dumpfile.vcd");
		$dumpvars(0, indicator);
	end

	indicator indicator(.clk(clk),.rst(rst),.level(level),.button(button),.usb(usb),.stat(stat),.pos(pos),.neg(neg),.red(red),.green(green));

	initial begin
			#1 clk = 0; 
		forever begin
			#100	clk =  ! clk; 
		end
	end

	initial begin
		#1	rst = 1; 
		#100	rst=0;
		#100	rst=1;


		//	D			A		B			C,E,F
		//	
		//      no usb plugged in,	low battery,	button released,	not charging  		off
		#1000   usb=0;			level=0;	button=0;		neg=0;pos=1;stat=1;
		//      no usb plugged in,	low battery,	button pressed,		not charging  		red
		#1000   usb=0;			level=0;	button=1; 		neg=0;pos=1;stat=1;
		//	no usb plugged in,	full battery,	button released,	not charging 		off
		#1000   usb=0;			level=1;	button=0;		neg=0;pos=1;stat=1;
		//      no usb plugged in,	full battery,	button pressed,		not charging  		green
		#1000   usb=0;			level=1;	button=1;		neg=0;pos=1;stat=1;

		//PLUG IN
		//      usb plugged in,		low battery,	button released,	charging		off	
		#1000   usb=1;			level=0;	button=0;		neg=0;pos=1;stat=0;
		//      usb plugged in,		low battery,	button pressed,		charging		off
		#1000   usb=1;			level=0;	button=1;		neg=0;pos=1;stat=0;
		//      usb plugged in,		low battery,	button released,	charging		off	
		#1000   usb=1;			level=0;	button=0;		neg=0;pos=0;stat=0;
		//      usb plugged in,		low battery,	button pressed,		charging		off
		#1000   usb=1;			level=0;	button=1;		neg=0;pos=0;stat=0;
		//      usb plugged in,		low battery,	button released,	charging		green,red	
		#1000   usb=1;			level=0;	button=0;		neg=1;pos=0;stat=0;
		//      usb plugged in,		low battery,	button pressed,		charging		green,red
		#1000   usb=1;			level=0;	button=1;		neg=1;pos=0;stat=0;

		//	CHARGE TO FULL
		//	usb plugged in,		full battery,	button released,	not charging  		green
		#1000   usb=1;			level=1;	button=0;		neg=0;pos=1;stat=1;
		//      usb plugged in,		full battery,	button pressed,		not charging  		green
		#1000   usb=1;			level=1;	button=1;		neg=0;pos=1;stat=1;
		
		//UNPLUG @ FULL
		//      usb plugged in,		low battery,	button released,	charging		green,red	
		#1000   usb=0;			level=1;	button=0;		neg=1;pos=0;stat=1;
		//      usb plugged in,		low battery,	button pressed,		charging		green,red
		#1000   usb=0;			level=1;	button=1;		neg=1;pos=0;stat=1;
		//      usb plugged in,		low battery,	button released,	charging		green,red	
		#1000   usb=0;			level=1;	button=0;		neg=0;pos=0;stat=1;
		//      usb plugged in,		low battery,	button pressed,		charging		green,red
		#1000   usb=0;			level=1;	button=1;		neg=0;pos=0;stat=1;
		//      usb plugged in,		low battery,	button released,	charging		green,red	
		#1000   usb=0;			level=1;	button=0;		neg=0;pos=1;stat=1;
		//      usb plugged in,		low battery,	button pressed,		charging		green,red
		#1000   usb=0;			level=1;	button=1;		neg=0;pos=1;stat=1;		

		//UNPLUG @ NOT FULL
		//      usb plugged in,		low battery,	button released,	not charging  		green,red	
		#1000   usb=0;			level=0;	button=0;		neg=1;pos=0;stat=1;
		//      usb plugged in,		low battery,	button pressed,		not charging  		green,red
		#1000   usb=0;			level=0;	button=1;		neg=1;pos=0;stat=1;
		//      usb plugged in,		low battery,	button released,	not charging  		green,red	
		#1000   usb=0;			level=0;	button=0;		neg=0;pos=0;stat=1;
		//      usb plugged in,		low battery,	button pressed,		not charging  		green,red
		#1000   usb=0;			level=0;	button=1;		neg=0;pos=0;stat=1;
		//      usb plugged in,		low battery,	button released,	not charging  		green,red	
		#1000   usb=0;			level=0;	button=0;		neg=0;pos=1;stat=1;
		//      usb plugged in,		low battery,	button pressed,		not charging  		green,red
		#1000   usb=0;			level=0;	button=1;		neg=0;pos=1;stat=1;	



		//	usb plugged in,		full battery,	button released,	fault 			red blk
		#1000   usb=1;			level=1;	button=0;		neg=0; pos=0;
			repeat(100)
				stat=!stat;
		//      usb plugged in,		full battery,	button pressed,		fault  			red blk
		 	usb=1;			level=1;	button=1;		neg=0; pos=0;
			repeat(100)
				stat=!stat;
		//      usb plugged in,		low battery,	button released,	fault			red blk
		        usb=1;			level=0;	button=0;		neg=0; pos=0;
			repeat(100)
				stat=!stat;
		//      usb plugged in,		low battery,	button pressed,		fault			red blk
		        usb=1;			level=0;	button=1;		neg=0; pos=0;
			repeat(100)
				stat=!stat;

		#1000 $finish;

	end
endmodule