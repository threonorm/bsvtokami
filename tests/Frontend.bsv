import FIFO::*;

interface Frontend;
    method Action update_predictors(Bit#(32) el);
    method Action deq_ireq();
    method MemReq first_ireq();
endinterface

typedef struct {
   Bit#(1) op;
   Bit#(32) addr;
   Bit#(32) data;
   } MemReq deriving (Bits);


module Frontend mkFrontend();
   Reg#(Bit#(32)) pc <- mkRegU();
   FIFO#(MemReq) stream <- mkFIFO();

   rule fetch_next;
   	stream.enq(MemReq{op:0, addr:pc + 4, data: 0});
	pc <= pc + 4;
   endrule

   method Action update_predictors(Bit#(32) el) when (True);
   	pc <= el;
   endmethod

   method Action deq_ireq() when (True);
   	stream.deq();
   endmethod

   method MemReq first_ireq() when (True);
   	return stream.first();
   endmethod
endmodule 

