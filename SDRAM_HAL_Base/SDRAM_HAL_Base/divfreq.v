module divfreq(ClkIn, ClkOut);

input ClkIn;
output ClkOut;

parameter NB = 5;
parameter NBTON = 2;
parameter POLARITE = 0;
parameter BUS_SIZE = 8;
reg ClkOut;
reg[BUS_SIZE-1:0] i;

always @(negedge(ClkIn))
begin
	if (i < NB)
	begin
		i <= i + 1;
	end
	else
	begin
		i <= 0;
	end
end


always @(i)
begin
	if (i < NBTON)
	begin
		ClkOut <= POLARITE;
	end
	else
	begin
		ClkOut <= ~POLARITE;
	end	
end


endmodule
