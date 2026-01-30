module led_rtiggle
    (
        CLK,
        RESENT_n,
        Led
    );
    input CLK;
    input  RESENT_n; 
    output reg Led;
    reg [25:0]counter ; // 增加位宽，26位才能存下 50,000,000
    // 修改为: 高电平复位 (假设 R11 是普通按键，平时为0，按下为1)
    always @(posedge CLK or posedge RESENT_n) begin
        if (RESENT_n) 
            counter <= 0;
        else if (counter == 50_000_000) // 100MHz 时钟下，计数到 50M 为 0.5秒
            counter <= 0;
        else 
            counter <= counter + 1'd1;
    end

    always @(posedge CLK or posedge RESENT_n) begin
        if(RESENT_n) 
            Led <= 1'b0;
        else if (counter == 50_000_000) // 保持一致
            Led <= ~Led;
    end
endmodule