module modMultiDivfreq ( ClkIn, ClockOut);

parameter NB_DIV = 24;				// Nombre de canaux de division de fréquence

input ClkIn;							// Clock d'entrée servant de référence pour les autres clocks

output[(NB_DIV-1):0] ClockOut;	// NB_DIV correspond au nombre de sorties de diviseur

genvar i;

generate for ( i = 0; i < NB_DIV ; i = i+1 )
    begin: 
		inst divfreq #(48,24,0,16) div_i(ClkIn, ClockOut[i]);
    end
endgenerate

endmodule
