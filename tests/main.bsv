import Gcd::*;

module Empty mkMain(Bit#(32) nn, Bit#(32) mm);
   GCD#(Bit#(32)) gcd <- mkGCD();
   Reg#(Bit#(1)) started <- mkRegU();
   Reg#(Bit#(32)) dv <- mkRegU();
   rule rl_start when (started == 0);
      gcd.set_n(nn);
      gcd.set_m(mm);
      started <= 1;
   endrule
   rule rl_display;
      //let v = gcd.result();
      //dv <= v;
      $finish();
   endrule
endmodule

