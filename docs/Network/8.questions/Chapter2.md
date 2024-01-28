# Chapter 2

## SECTION 2.1


!!! question  "List five nonproprietary Internet applications and the application-layer protocols that they use."


!!! question  "What is the difference between network architecture and application architecture?"
    1. **network architecture** is about layers(five-layer Internet architecture / OSI 7-layer), which set the rule for how messages communicate with each other. From the application developer’s perspective, the network architecture is fixed and provides a specific set of services to applications.
    2. **application architecture** is how applications are structured. Namely, who provide the services and how end system interact with the service providers. There are two main kind: Client-Server architecture and Peer-to-Per(P2P) architecture. "

!!! question  "For a communication session between a pair of processes, which process is the client and which is the server?"
    The process which initiates the communication is called client; the process that waits to be contacted is called server."


!!! question  "Why are the terms client and server still used in peer-to-peer applications?"


!!! question  "What information is used by a process running on one host to identify a process running on another host?"


!!! question  "What is the role of HTTP in a network application? What other components are needed to complete a Web application?"


!!! question  "Referring to Figure 2.4, we see that none of the applications listed in Figure 2.4 requires both no data loss and timing. Can you conceive of an application that requires no data loss and that is also highly time-sensitive?"


!!! question  "List the four broad classes of services that a transport protocol can provide. For each of the service classes, indicate if either UDP or TCP (or both) provides such a service."


!!! question  "Recall that TCP can be enhanced with TLS to provide process-to-process security services, including encryption. Does TLS operate at the transport layer or the application layer? If the application developer wants TCP to be enhanced with TLS, what does the developer have to do?"

## SECTIONS 2.2–2.5"


!!! question  "What is meant by a handshaking protocol?"


!!! question  "What does a stateless protocol mean? Is IMAP stateless? What about SMTP?"


!!! question  "How can websites keep track of users? Do they always need to use cookies?"


!!! question  "Describe how Web caching can reduce the delay in receiving a requested object. Will Web caching reduce the delay for all objects requested by a user or for only some of the objects? Why?"


!!! question  "Telnet into a Web server and send a multiline request message. Include in the request message the If-modified-since: header line to force a response message with the 304 Not Modified status code."


!!! question  "Are there any constraints on the format of the HTTP body? What about the email message body sent with SMTP? How can arbitrary data be transmitted over SMTP?"


!!! question  "Suppose Alice, with a Web-based e-mail account (such as Hotmail or Gmail), sends a message to Bob, who accesses his mail from his mail server using IMAP. Discuss how the message gets from Alice’s host to Bob’s host. Be sure to list the series of application-layer protocols that are used to move the message between the two hosts."


!!! question  "Print out the header of an e-mail message you have recently received. How many Received: header lines are there? Analyze each of the header lines in the message."


!!! question  "What is the HOL blocking issue in HTTP/1.1? How does HTTP/2 attempt to solve it?"


!!! question  "Why are MX records needed? Would it not be enough to use a CNAME record? (Assume the email client looks up email addresses through a Type A query and that the target host only runs an email server.)"


!!! question  "What is the difference between recursive and iterative DNS queries?"

## SECTION 2.5"


!!! question  "Under what circumstances is file downloading through P2P much faster than through a centralized client-server approach? Justify your answer using Equation 2.2."


!!! question  "Consider a new peer Alice that joins BitTorrent without possessing any chunks. Without any chunks, she cannot become a top-four uploader for any of the other peers, since she has nothing to upload. How then will Alice get her first chunk?"


!!! question  "Assume a BitTorrent tracker suddenly becomes unavailable. What are its consequences? Can files still be downloaded?"

## SECTION 2.6"


!!! question  "CDNs typically adopt one of two different server placement philosophies. Name and briefly describe them."


!!! question  "Besides network-related considerations such as delay, loss, and bandwidth performance, there are other important factors that go into designing a CDN server selection strategy. What are they?"

## SECTION 2.7"


!!! question  "In Section 2.7, the UDP server described needed only one socket, whereas the TCP server needed two sockets. Why? If the TCP server were to support n simultaneous connections, each from a different client host, how many sockets would the TCP server need?"


!!! question  "For the client-server application over TCP described in Section 2.7, why must the server program be executed before the client program? For the client-server application over UDP, why may the client program be executed before the server program?"
