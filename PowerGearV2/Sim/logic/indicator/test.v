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

	always @(clk or pos or neg)
	begin
		if (!neg && !pos)
			stat = clk;
		else if (!neg && pos)
			stat = 1;
		else if(neg && !pos)
			stat = 0;
	end

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
		#1		rst = 1; 
		#100	rst=0;
		#100	rst=1;

		//	full battery,	button released,	no usb plugged in,	not charging 		off
		#100    level=1;	button=0;		usb=0; 			neg=0; pos=1;
		//      full battery,	button pressed,		no usb plugged in,	not charging  		green
		#1000   level=1;	button=1;		usb=0; 			neg=0; pos=1;
		//      low battery,	button released,	no usb plugged in,	not charging  		off
		#1000   level=0;	button=0;		usb=0; 			neg=0; pos=1;
		//      low battery,	button pressed,		no usb plugged in,	not charging  		green,red
		#1000   level=0;	button=1; 		usb=0; 			neg=0; pos=1;

		//	full battery,	button released,	usb plugged in,		not charging  		green
		#1000   level=1;	button=0;		usb=1; 			neg=0; pos=1;
		//      full battery,	button pressed,		usb plugged in,		not charging  		green
		#1000   level=1;	button=1;		usb=1; 			neg=0; pos=1;
		//      low battery,	button released,	usb plugged in, 	charging		green,red	
		#1000   level=0;	button=0;		usb=1; 			neg=1; pos=0;
		//      low battery,	button pressed,		usb plugged in, 	charging		green,red
		#1000   level=0;	button=1;		usb=1; 			neg=1; pos=0;

		//	full battery,	button released,	usb plugged in,		fault 			red blk
		#1000   level=1;	button=0;		usb=1; 			neg=0; pos=0;
		//      full battery,	button pressed,		usb plugged in,		fault  			red blk
		#1000   level=1;	button=1;		usb=1; 			neg=0; pos=0;
		//      low battery,	button released,	usb plugged in, 	fault			red blk
		#1000   level=0;	button=0;		usb=1; 			neg=0; pos=0;
		//      low battery,	button pressed,		usb plugged in, 	fault			red blk
		#1000   level=0;	button=1;		usb=1; 			neg=0; pos=0;

		#1000 $finish;

	end
endmodule