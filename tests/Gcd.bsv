interface GCD#(type a);
    method Action set_n (a n);
    method Action set_m (a m);
    method a result;
endinterface: GCD

module GCD#(Bit#(32)) mkGCD();
   Reg#(Bit#(32)) n <- mkRegU();
   Reg #(Bit#(32)) m <- mkRegU();

   rule swap when (n > m && m != 0);
      n <= m;
      m <= n;
   endrule

   rule sub when (n <= m && m != 0);
      m <= m - n;
   endrule

   method Action set_n(Bit#(32) in_n) when (m == 0);
       n <= in_n;
   endmethod
   method Action set_m(Bit#(32) in_m) when (m == 0);
       m <= in_m;
   endmethod

   method Bit#(32) result() when (m == 0);
      return n;
   endmethod: result
endmodule: mkGCD

