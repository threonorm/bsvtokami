
interface Frontend;
    method Action update_predictors(Bit#(32) el);
    method Action deq_ireq();
    method Bit#(32) first_ireq();
endinterface

module Frontend mkFrontend();
   Reg#(Bit#(32)) pc <- mkRegU();

   rule fetch_next;
	pc <= pc + 4;
   endrule

   method Action update_predictors(Bit#(32) el) when (True);
   	pc <= el;
   endmethod

   method Action deq_ireq() when (True) ;
   	noAction;
   endmethod

   method Bit#(32) first_ireq() when (True);
   	return 0;
   endmethod
endmodule 

