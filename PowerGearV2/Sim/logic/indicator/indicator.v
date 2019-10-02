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

//assign red = (~level)&neg | (~level)&button&(~usb) | ~(neg)&(~pos)&stat;     //Red table  R = !AC + !AB!D + !C!EF
//assign green = neg | pos&level&button | pos&level&usb;                 //Green table  G = C + EAB + EAD

assign red = ~(level|(~neg)) | ~(level|(~button)|usb) | ~(neg|pos|(~stat));     //Red table  R = !(A+!C) + !(A+!B+D) + !(C+E+!F)
assign green = neg | pos&level&button | pos&level&usb;                 //Green table  G = C + !(!E+!A+!B) + !(!E+!A+!D)

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