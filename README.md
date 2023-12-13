# Introduction of KETI-CSD-Identifier
--------------

A proxy module for communication between the Storage Engine and the CSD's Worker Module.


## Contents
--------------
[1. Requirement](#requirement)

[2. How To Install](#How-To-Install)

[3. Governance](#governance)

## Requirement
--------------
> g++-11

## How To Build
--------------
```bash
g++ CSDProxy.cc -o CSDProxy -lpthread
```

## How It Works
--------------
<img src = https://github.com/opencsd/KETI-CSD-Proxy/assets/57175313/c2fe40ff-70bc-40fd-9fdf-713ab60939b8 width="80%" height="80%">

## Governance
--------------
This work was supported by Institute of Information & communications Technology Planning & Evaluation (IITP) grant funded by the Korea government(MSIT) (No.2021-0-00862, Development of DBMS storage engine technology to minimize massive data movement)

