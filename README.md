# network-programming-hw
Homework for the network programming course in university - DNS server/client in C

#### Compiling
`cc TCP_DNSserver_IPv6_IPv4.c -o server`

`cc TCP_DNSclient_IPv6.c -o client`

#### Running the apps
`./server <server_IPv6_addess>`

`./client <server_IPv6_addess> <domain name>`

#### Testing
`./client  ::ffff:c0a8:66 yahoo.com`  
