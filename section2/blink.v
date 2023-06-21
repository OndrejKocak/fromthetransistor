module blink(output led, input clk );
    reg [32:0] counter;
    reg state;
    assign led = state;
    always @(posedge clk) begin
        counter <= counter + 1;
        if(counter % 100000 <= 1) begin
            state <= !state;
        end
    end
endmodule