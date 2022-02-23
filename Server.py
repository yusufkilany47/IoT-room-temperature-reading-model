# assume the server can talk to N clients at the same time, N is declared in "listen()" fn.
import socket
import threading

# next create a socket object
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)		
print ("Socket successfully created")

# reserve a port on your computer
port = 3686			

# Next bind to the port
s.bind(('192.168.163.137', port))		

# put the socket into listening mode
s.listen(5)	
print ("Server is listening on port %s" %(port) + "\n")

#clients number counter
clientsCounter = 0 		

#an array for the clients' info
clientsInfo = [] 


def thread_fn(c, a):
 
 global clientsCounter 
 global clientsInfo 
   
 while True :
     
    inMsg = c.recv(1024).decode()
    if inMsg.upper() == 'CLOSE SOCKET' :
        break
    
    print("\n" + inMsg)
    #outMsg = inMsg.upper()
    #c.send(outMsg.encode())
    
 # Close the connection with the client
 clientsCounter -= 1 
 print("Connection terminated with the client with the address: ")
 print(a) 
 print("Current number of clients: %s" %(clientsCounter) + "\n" + "Current Clients' info: ")
 clientsInfo.pop(clientsInfo.index(a))
 
 for x in clientsInfo:
     print(x)
     
 print("\n")    
 c.close()		


# a forever loop until we interrupt it or an error occurs
while True:

 # Establish connection with client.
 c, addr = s.accept()	
 clientsCounter += 1 
 clientsInfo.append(addr) 
 
 print("Current number of clients= %s"  %(clientsCounter) )
 
 print("Clients' info: ") 
 for x in clientsInfo:
     print(x) 
 
 print("\n")     

 # send a thank you message to the client. encoding to send byte type.
 c.send('Thank you for connecting to my server !!'.encode())
 
 thread = threading.Thread(target=(thread_fn), args=(c,addr))
 thread.start()

# We don't close the server socket 's'