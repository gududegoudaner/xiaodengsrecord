`timescale 1ns/1ns
module led_triggle_tb();
    reg CLK;
    reg RESENT_n;
    wire Led;

    // 实例化被测模块 led_rtiggle
    led_rtiggle u_led_rtiggle
    (
        .CLK       (CLK),
        .RESENT_n  (RESENT_n),                      
        .Led       (Led)
    );
    initial  CLK = 1;
    // 100MHz 时钟 (半周期 5ns)
    always #5 CLK = ~CLK;
    initial begin
        RESENT_n = 0;
        #201;
        RESENT_n = 1;
        #2000_000_000;
        $stop;
    end
endmodule