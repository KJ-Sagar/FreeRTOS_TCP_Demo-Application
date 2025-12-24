# FreeRTOS TCP Demo – Run Instructions (All Modes)

This project demonstrates **FreeRTOS+TCP** running on **QEMU (MPS2 Cortex-M3)** using a **Linux TAP interface**.
Linux and QEMU communicate using **real TCP sockets over Ethernet**.

Both the **host (Linux)** and **QEMU (FreeRTOS)** can act as a **TCP client or TCP server**.
This document explains **all four valid scenarios** using the **Heartbeat** and **Echo** demos, while keeping the setup consistent and predictable.

---

## IP Configuration (Common)

Linux (tap0): 10.0.0.1
QEMU (FreeRTOS): 10.0.0.2
TCP Port: 5001

---

## General Setup (Required for All Modes)

1. Build the project:

   ```
   make clean
   make
   ```

2. Open **two terminals**:

   * Terminal 1 → Linux host side
   * Terminal 2 → QEMU / FreeRTOS side

3. In `app_config.h`, ensure:

   * Only **one role** is enabled (client or server)
   * Only **one demo** is enabled (heartbeat or echo)

4. TAP networking and firewall handling are managed automatically by:

   ```
   sudo ./TCP_Demo.sh
   ```

---

## Host Programs

The Linux host provides two programs:

* `tcp_server`
  Linux listens for incoming TCP connections.

* `tcp_client`
  Linux actively connects to a TCP server.

The correct program must be used depending on the demo mode.

---

## Heartbeat Demo – QEMU Client, Host Server

In this mode, **FreeRTOS acts as a TCP client** and periodically sends heartbeat messages.
The **Linux host acts as a TCP server** and receives them.

Configuration in `app_config.h`:

```
#define APP_ROLE_CLIENT     1
#define APP_ROLE_SERVER     0

#define APP_DEMO_HEARTBEAT  1
#define APP_DEMO_ECHO       0
```

Run sequence:

Terminal 1 (Linux):

```
./tcp_server
```

Terminal 2 (QEMU):

```
sudo ./TCP_Demo.sh
```

Expected behavior:

Linux terminal shows:

```
[LINUX RX] HEARTBEAT 0
[LINUX RX] HEARTBEAT 1
```

QEMU terminal shows:

```
HB: Connected to 10.0.0.1:5001
```

---

## Heartbeat Demo – QEMU Server, Host Client

In this mode, **FreeRTOS acts as a TCP server** and generates heartbeat messages.
The **Linux host connects as a client** and receives them.

Configuration in `app_config.h`:

```
#define APP_ROLE_CLIENT     0
#define APP_ROLE_SERVER     1

#define APP_DEMO_HEARTBEAT  1
#define APP_DEMO_ECHO       0
```

Run sequence:

Terminal 2 (QEMU – start first):

```
sudo ./TCP_Demo.sh
```

Wait until QEMU indicates it is listening.

Terminal 1 (Linux):

```
./tcp_client
```

Expected behavior:

Linux terminal continuously prints heartbeat messages received from FreeRTOS.

---

## Echo Demo – QEMU Client, Host Server

In this mode, **FreeRTOS acts as a TCP client**.
The **Linux host acts as a TCP echo server**.

Configuration in `app_config.h`:

```
#define APP_ROLE_CLIENT     1
#define APP_ROLE_SERVER     0

#define APP_DEMO_ECHO       1
#define APP_DEMO_HEARTBEAT  0
```

Run sequence:

Terminal 1 (Linux):

```
./tcp_server
```

Terminal 2 (QEMU):

```
sudo ./TCP_Demo.sh
```

Expected behavior:

Data sent by FreeRTOS is received and echoed back by Linux.

---

## Echo Demo – QEMU Server, Host Client

In this mode, **FreeRTOS acts as a TCP echo server**.
The **Linux host connects as a client** and sends data.

Configuration in `app_config.h`:

```
#define APP_ROLE_CLIENT     0
#define APP_ROLE_SERVER     1

#define APP_DEMO_ECHO       1
#define APP_DEMO_HEARTBEAT  0
```

Run sequence:

Terminal 2 (QEMU – start first):

```
sudo ./TCP_Demo.sh
```

Wait until:

```
[ECHO SERVER] Waiting for client...
```

Terminal 1 (Linux):

```
./tcp_client
```

Type:

```
hello qemu
```

Expected behavior:

Linux receives the echoed message, and QEMU logs the received data.

---

## Common Rules

* Exactly **one side must listen** and **one side must connect**.
* Client must start **after** the server is ready.
* Do not enable multiple demos at the same time.
* Do not run client or server on both sides simultaneously.
* Firewall is temporarily disabled by `TCP_Demo.sh` and restored on exit.
* Stop QEMU using `Ctrl+C`.

---

## End of Document
