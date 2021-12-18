compile:
	g++ helper.cpp buffered_delivered_msg.cpp multicast_fifo_ordering.cpp -o fifo_ordering -std=c++11 -pthread 
#	g++ multicast_causal_ordering.cpp helper.cpp -o causal_ordering -std=c++11 -pthread 

fifo_ordering:
	g++ helper.cpp buffered_delivered_msg.cpp multicast_fifo_ordering.cpp -o fifo_ordering -std=c++11 -pthread 

#causal_ordering:
#	g++ multicast_causal_ordering.cpp helper.cpp -o causal_ordering -std=c++11 -pthread 

clean:
	rm   fifo_ordering