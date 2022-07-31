   import socket
   import sys
   
   client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
   
   
   version = b"\x03"
   # 01 - begin 02 - terminate 03 - data
   packetType = b"\x03"
   
   # 01 - start 02 -end  04 - data 08 - info
   packetFlags = b"\x08"
   
   packetBool = b"\x01"
   
   # must be less than 200 bytes
   if len(sys.argv) ==1:
       payload = b"Hello there!"
   else:
       payload = sys.argv[1].encode()
   
   
   payloadLength = (len(payload)).to_bytes(4,'big')
   
   message = version + packetType + packetFlags + packetBool +  payloadLength +  payload  
   
   
   client.sendto(message, ('192.168.0.2',1234))
   client.close()
   
