module SDRAM_HAL_Base(GPIO,CLOCK_50,KEY,DRAM_ADDR,DRAM_BA,DRAM_CAS_N,DRAM_CKE,DRAM_CS_N,DRAM_DQ,DRAM_DQM,DRAM_RAS_N,DRAM_WE_N,DRAM_CLK,LCD_RS,LCD_RW,LCD_DATA,LCD_EN,LEDG,LEDR,SW);

//Ajout des différentes entrées du système : le signal CLOCK_50, les boutons poussoirs et les interrupteurs
input CLOCK_50;
input[3:0] KEY;
input [17:0]SW;
//Ajout des sorties du système, les sorties liées au NIOS, au LCD, les LEDS et les GPIOS
output[12:0] DRAM_ADDR;
output[1:0] DRAM_BA;
output DRAM_CAS_N;
output DRAM_CKE;
output DRAM_CS_N;
output[3:0] DRAM_DQM;
output DRAM_RAS_N;
output DRAM_WE_N;
output DRAM_CLK;
output LCD_RS,LCD_RW,LCD_EN;
output [7:0] LEDG;
output [17:0] LEDR;
output[35:0] GPIO;
//Entrées/sorties nécessaires : 
inout [7:0] LCD_DATA;
inout[31:0] DRAM_DQ;
//"Fils" interne, interconnectants le module PWM ainsi que le module de division de fréquence
wire[9:0] Ton_PWM;
wire[23:0] Latch_PWM,OE_PWM;
wire[23:0] CLOCK_DIV;

//IP Core utilisé
    SDRAM_HAL_Base_sys u0 (
        .button_export         (KEY[3:0]),         //          button.export
        .clk_clk               (CLOCK_50),               //             clk.clk
        .lcd_16207_RS          (LCD_RS),          //       lcd_16207.RS
        .lcd_16207_RW          (LCD_RW),          //                .RW
        .lcd_16207_data        (LCD_DATA[7:0]),        //                .data
        .lcd_16207_E           (LCD_EN),           //                .E
        .led_export            ({LEDR[15:0],LEDG[7:0]}),            //             led.export
        .nlatch_pwm_export     (Latch_PWM[23:0]),     //      nlatch_pwm.export
        .oe_pwm_export         (OE_PWM[23:0]),         //          oe_pwm.export
        .reset_reset_n         (SW[17]),         //           reset.reset_n
        .sdram_clk_clk         (DRAM_CLK),         //       sdram_clk.clk
        .sdram_ctrl_wire_addr  (DRAM_ADDR),  // sdram_ctrl_wire.addr
        .sdram_ctrl_wire_ba    (DRAM_BA),    //                .ba
        .sdram_ctrl_wire_cas_n (DRAM_CAS_N), //                .cas_n
        .sdram_ctrl_wire_cke   (DRAM_CKE),   //                .cke
        .sdram_ctrl_wire_cs_n  (DRAM_CS_N),  //                .cs_n
        .sdram_ctrl_wire_dq    (DRAM_DQ),    //                .dq
        .sdram_ctrl_wire_dqm   (DRAM_DQM),   //                .dqm
        .sdram_ctrl_wire_ras_n (DRAM_RAS_N), //                .ras_n
        .sdram_ctrl_wire_we_n  (DRAM_WE_N),  //                .we_n
        .ton_pwm_export        (Ton_PWM[9:0])         //         ton_pwm.export
    );
//Ajout du module multi PWM
	 modMultiPWM #(24,10) pwm(CLOCK_DIV[23:0], GPIO[23:0], Latch_PWM[23:0], Ton_PWM[9:0], OE_PWM[23:0]);
//Ajout du module de division de fréquence
	 modMultiDivfreq #(25) div(CLOCK_50, {CLOCK_DIV[23:0],GPIO[35]});
endmodule


