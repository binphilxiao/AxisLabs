module indicator(
	clk,
	rst,
	level,      // A
	button,     // B
	usb,        // D 
	stat,       // F
	pos,        // E
	neg,        // C
	red,
	green);

input clk, rst, level, button, usb, stat, pos, neg;
output red,green;
wire a,b,c,d,e,f;

assign a = level;
assign b = button;
assign c = neg;
assign e = pos;
assign d = usb;
assign f = stat;

assign red = !a&!f | !a&b&!d | !c&!e&!f;
assign green = a&c | a&e&f | !a&c&d;

//assign red = ~(level|(~neg)) | ~(level|(~button)|usb) | ~(neg|pos|(~stat));     //Red table  R = !(A+!C) + !(A+!B+D) + !(C+E+!F)
//assign green = neg | pos&level&button | pos&level&usb;                 //Green table  G = C + !(!E+!A+!B) + !(!E+!A+!D)

/*always @(posedge clk or negedge rst)
begin
    if (!rst)
        red <= 0;
    else
        red <= (~level)&pos&(~stat) | (~level)&button&(~usb) | neg&pos&(~stat);
end

always @(posedge clk or negedge rst)
begin
    if (!rst)
        green <= 0;
    else
        green <= neg | pos&button | pos&level&usb; 
end*/

endmodule